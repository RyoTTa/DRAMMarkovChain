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

        double Rg;
        double Rn;
        double Rw;
        double Rb; // All Ratio Sum == 1

        double Pnw;
        double Pnn;
        double Pwn;
        double Pww;

        double Pg;
        double Pn;
        double Pw;
        double Pb;

        std::string output_dir;
        std::string output_prefix;


    private:
        INIReader *reader_;

        void CalculateSize();

        int GetInteger(const std::string &sec, const std::string &opt,
                       int default_val) const;

        double GetDouble(const std::string &sec, const std::string &opt,
                         double default_val) const;

        void InitFaultParams();


    };


}
#endif //DRAMMARKOV_CONFIG_H