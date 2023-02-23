//
// Created by ryotta205 on 8/17/22.
//
#include "configuration.h"

#include <vector>
#include <bitset>
#include <numeric>

namespace naivemarkov {

    Config::Config(std::string config_file, std::string out_dir, std::string out_prefix)
            : output_dir(out_dir), output_prefix(out_prefix), reader_(new INIReader(config_file)) {
        if (reader_->ParseError() < 0) {
            std::cerr << "Can't load config file - " << config_file << std::endl;
            AbruptExit(__FILE__, __LINE__);
        }

        // The initialization of the parameters has to be strictly in this order
        // because of internal dependencies
        InitDeviceParams();
        InitFaultParams();
        ParsingFaultParams();

        delete (reader_);
    }

    Config::~Config() {

    }

    int Config::GetInteger(const std::string &sec, const std::string &opt,
                           int default_val) const {
        return static_cast<int>(reader_->GetInteger(sec, opt, default_val));
    }

    double Config::GetDouble(const std::string &sec, const std::string &opt,
                             double default_val) const {
        return static_cast<double>(reader_->GetReal(sec, opt, default_val));
    }

    std::string Config::Get(const std::string &sec, const std::string &opt,
                            std::string default_val) const {
        return static_cast<std::string>(reader_->Get(sec, opt, default_val));
    }

    void Config::InitDeviceParams() {
        const auto &reader = *reader_;
        ROW = GetInteger("device_parameters", "ROW", 100);
        COL = GetInteger("device_parameters", "COL", 100);
        DEVICE_WIDTH = GetInteger("device_parameters", "DEVICE_WIDTH", 4);
//Example
//        bus_width = GetInteger("system", "bus_width", 64);
//        address_mapping = reader.Get("system", "address_mapping", "chrobabgraco");
    }

    void Config::InitFaultParams() {
        const auto &reader = *reader_;
        GROUP = GetInteger("fault_parameters", "number_of_group", 1);

        Rg = GetDouble("fault_parameters", "R_good", 0.001);
        Rn = GetDouble("fault_parameters", "R_normal", 0.001);
        Rw = GetDouble("fault_parameters", "R_weak", 0.001);
        Rb = GetDouble("fault_parameters", "R_bad", 0.001);
        TEMP_G_Rn = Get("fault_parameters", "G_R_normal", "1.0");
        TEMP_G_Rw = Get("fault_parameters", "G_R_weak", "1.0");

        TEMP_G_Pnw = Get("fault_parameters", "G_P_nw", "0.001");
        TEMP_G_Pwn = Get("fault_parameters", "G_P_wn", "0.001");

        Pg = GetDouble("fault_parameters", "P_g", 0.001);
        TEMP_G_Pn = Get("fault_parameters", "G_P_n", "0.001");
        TEMP_G_Pw = Get("fault_parameters", "G_P_w", "0.001");
        Pb = GetDouble("fault_parameters", "P_b", 0.001);

        //For Test Start
        Pnw = GetDouble("fault_parameters", "P_nw", 0.001);
        Pwn = GetDouble("fault_parameters", "P_wn", 0.001);

        Pn = GetDouble("fault_parameters", "P_n", 0.001);
        Pw = GetDouble("fault_parameters", "P_w", 0.001);
        //For Test Start
//Example
//        bus_width = GetInteger("system", "bus_width", 64);
//        address_mapping = reader.Get("system", "address_mapping", "chrobabgraco");
    }

    void Config::ParsingFaultParams(){
        string part;
        std::vector<double>::iterator iter;
        std::stringstream ss;

        //Ratio of normal cell per group
        ss.clear();
        ss.str(TEMP_G_Rn);
        while(ss >> part){
            G_Rn.push_back(std::stod(part));
        }
        //Ratio of weak cell per group
        ss.clear();
        ss.str(TEMP_G_Rw);
        while(ss >> part){
            G_Rw.push_back(std::stod(part));
        }

        //State transition probability of normal to weak per group
        ss.clear();
        ss.str(TEMP_G_Pnw);
        while(ss >> part){
            G_Pnw.push_back(std::stod(part));
        }
        for(iter = G_Pnw.begin(); iter != G_Pnw.end(); iter++){
            G_Pnn.push_back((double)1.0 - *iter);
        }

        //State transition probability of weak to normal per group
        ss.clear();
        ss.str(TEMP_G_Pwn);
        while(ss >> part){
            G_Pwn.push_back(std::stod(part));
        }
        for(iter = G_Pwn.begin(); iter != G_Pwn.end(); iter++){
            G_Pww.push_back((double)1.0 - *iter);
        }

        //Error probability of normal cell per group
        ss.clear();
        ss.str(TEMP_G_Pn);
        while(ss >> part){
            G_Pn.push_back(std::stod(part));
        }

        //Error probability of weak cell per group
        ss.clear();
        ss.str(TEMP_G_Pw);
        while(ss >> part){
            G_Pw.push_back(std::stod(part));
        }

        //Error Check
        if(G_Rn.size() != (long unsigned int)GROUP ||
            G_Rw.size() != (long unsigned int)GROUP ||
            G_Pnw.size() != (long unsigned int)GROUP ||
            G_Pwn.size() != (long unsigned int)GROUP ||
            G_Pn.size() != (long unsigned int)GROUP ||
            G_Pw.size() != (long unsigned int)GROUP){
            std::cerr << "[ERROR]The number of parameters and group do not match" << std::endl;
            AbruptExit(__FILE__, __LINE__);
        }

        if ((Rg + Rn + Rw + Rb != (double)1.0)) {
            std::cerr << "[Warning]The sum of the cell ratios is not 1, adjust the good cell ratio" << std::endl;
            Rg = (double)1.0 - (Rn + Rw + Rb);
        }

        if(std::accumulate(G_Rn.begin(), G_Rn.end(), 0.0) != (double)1.0 ||
            std::accumulate(G_Rw.begin(), G_Rw.end(), 0.0) != (double)1.0){
            std::cerr << "[Warning]The sum of the group of cell ratios is not 1, adjust the last cell ratio" << std::endl;
            *(G_Rn.end()-1) = (double)1.0 - std::accumulate(G_Rn.begin(), G_Rn.end() - 1, 0.0);
            *(G_Rw.end()-1)  = (double)1.0 - std::accumulate(G_Rw.begin(), G_Rw.end() - 1, 0.0);
        }

        //For Test
        std::cout << "G_Rn : ";
        for(iter = G_Rn.begin(); iter != G_Rn.end(); iter++){
            std::cout << *iter << " ";
        }
        std::cout << std::endl;

        std::cout << "G_Rw : ";
        for(iter = G_Rw.begin(); iter != G_Rw.end(); iter++){
            std::cout << *iter << " ";
        }
        std::cout << std::endl;

        std::cout << "G_Pnw : ";
        for(iter = G_Pnw.begin(); iter != G_Pnw.end(); iter++){
            std::cout << *iter << " ";
        }
        std::cout << std::endl;

        std::cout << "G_Pwn : ";
        for(iter = G_Pwn.begin(); iter != G_Pwn.end(); iter++){
            std::cout << *iter << " ";
        }
        std::cout << std::endl;

        std::cout << "G_Pn : ";
        for(iter = G_Pn.begin(); iter != G_Pn.end(); iter++){
            std::cout << *iter << " ";
        }
        std::cout << std::endl;

        std::cout << "G_Pw : ";
        for(iter = G_Pw.begin(); iter != G_Pw.end(); iter++){
            std::cout << *iter << " ";
        }
        std::cout << std::endl;

    }

    void Config::PrintInfo() {


    }

} //namespace naivemarkov
