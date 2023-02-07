//
// Created by ryotta205 on 8/17/22.
//
#include "configuration.h"

#include <vector>
#include <bitset>

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
        Rg = GetDouble("fault_parameters", "R_good", 0.001);
        Rn = GetDouble("fault_parameters", "R_normal", 0.001);
        Rw = GetDouble("fault_parameters", "R_weak", 0.001);
        Rb = GetDouble("fault_parameters", "R_bad", 0.001);

        Pnw = GetDouble("fault_parameters", "P_nw", 0.001);
        Pnn = 1 - Pnw;
        Pwn = GetDouble("fault_parameters", "P_wn", 0.001);
        Pww = 1 - Pwn;

        Pg = GetDouble("fault_parameters", "P_g", 0.001);
        Pn = GetDouble("fault_parameters", "P_n", 0.001);
        Pw = GetDouble("fault_parameters", "P_w", 0.001);
        Pb = GetDouble("fault_parameters", "P_b", 0.001);
//Example
//        bus_width = GetInteger("system", "bus_width", 64);
//        address_mapping = reader.Get("system", "address_mapping", "chrobabgraco");
    }

    void Config::PrintInfo() {


    }

} //namespace naivemarkov