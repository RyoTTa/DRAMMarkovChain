#ifndef NAIVEMARKOVCHAIN_CONFIG_H
#define NAIVEMARKOVCHAIN_CONFIG_H

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "INIReader.h"
#include "common.h"

namespace naivemarkov {

    class Config {

    public:
        //Basic Function
        Config(std::string config_file, std::string out_dir, std::string out_prefix);

        ~Config();

        void PrintInfo();

        //String type is parsing temporary variable
        double Rg;
        double Rn;
        double Rw;
        double Rb; // All Ratio Sum == 1
        std::string TEMP_G_Rn;
        std::string TEMP_G_Rw;
        std::vector<double> G_Rn; // All Ratio Sum == 1
        std::vector<double> G_Rw; // All Ratio Sum == 1

        std::string TEMP_G_Pnw;
        std::string TEMP_G_Pnn;
        std::string TEMP_G_Pwn;
        std::string TEMP_G_Pww;
        std::vector<double> G_Pnw;
        std::vector<double> G_Pnn;
        std::vector<double> G_Pwn;
        std::vector<double> G_Pww;

        double Pg;
        std::string TEMP_G_Pn;
        std::string TEMP_G_Pw;
        double Pb;

        std::vector<double> G_Pn;
        std::vector<double> G_Pw;

        //For Test Start
        double Pnw;
        double Pwn;
        double Pn;
        double Pw;
        //For Test Start

        int ROW;
        int COL;
        int DEVICE_WIDTH;
        int GROUP;

        std::string output_dir;
        std::string output_prefix;


    private:
        INIReader *reader_;

        void CalculateSize();

        std::string Get(const std::string &sec, const std::string &opt, std::string default_val) const;

        int GetInteger(const std::string &sec, const std::string &opt,
                       int default_val) const;

        double GetDouble(const std::string &sec, const std::string &opt,
                         double default_val) const;

        void InitDeviceParams();
        void InitFaultParams();
        void ParsingFaultParams();

    };


}
#endif //DRAMMARKOV_CONFIG_H
