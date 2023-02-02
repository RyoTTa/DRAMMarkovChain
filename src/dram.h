//
// Created by ryotta205 on 2/2/23.
//

#ifndef NAIVEMARKOVCHAIN_DRAM_H
#define NAIVEMARKOVCHAIN_DRAM_H

#include "common.h"
#include "configuration.h"
#include <vector>
#include <list>
#include <algorithm>
#include <random>
#define SIZE 1000
namespace naivemarkov{

    class DRAM {
    public:
        DRAM(Config &config);
        ~DRAM() { good_cell.clear(); normal_cell.clear(); weak_cell.clear(), bad_cell.clear(); }

        int SIZE_G;
        int SIZE_N;
        int SIZE_W;
        int SIZE_B;

        std::vector<int> fault_map;

        std::vector<int> good_cell;
        std::vector<int> normal_cell;
        std::vector<int> weak_cell;
        std::vector<int> bad_cell;

        std::vector<double> probability_v;
        std::vector<int> indices_to_erase_or_fault;
        std::vector<int> normal_to_weak;
        std::vector<int> weak_to_normal;

        std::list<std::vector<int>> list_of_normal_cell;
        std::list<std::vector<int>> list_of_weak_cell;

        void Good_state();
        void Normal_state();
        void Week_state();
        void Bad_state();

        void State_change();
        void Fault_injection();
        void PrintFaultmap();
    private:
        Config &config_;
    };
}

#endif //NAIVEMARKOVCHAIN_DRAM_H
