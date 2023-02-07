//
// Created by ryotta205 on 2/2/23.
//
#include "dram.h"
namespace naivemarkov{
    DRAM::DRAM(Config &config) :
            config_(config) {
        SIZE = config_.ROW * config_.COL * config_.DEVICE_WIDTH;
        SIZE_G = SIZE * config_.Rg;
        SIZE_N = SIZE * config_.Rn;
        SIZE_W = SIZE * config_.Rw;
        SIZE_B = SIZE * config_.Rb;

        if ((config_.Rg + config_.Rn + config_.Rw + config_.Rb != (double)1.0)) {
            std::cerr << "The sum of the cell ratios is not 1, adjust the good cell ratio " << std::endl;
        }

        fault_map.reserve(SIZE);
        good_cell.reserve(SIZE);
        normal_cell.reserve(SIZE);
        weak_cell.reserve(SIZE);
        bad_cell.reserve(SIZE);

        probability_v.reserve(SIZE);
        indices_to_erase_or_fault.reserve(SIZE);
        normal_to_weak.reserve(SIZE);
        weak_to_normal.reserve(SIZE);

        //Initialized G,N,W,B cell vector
        for (int i = 0; i < SIZE; i++) {
            fault_map.push_back(i);
        }

        //Deletion dupliacte indices
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(fault_map.begin(), fault_map.end(), g);

        std::cout << "Bad cell selecting..." << std::endl;
        bad_cell.insert(bad_cell.end(), fault_map.begin(), fault_map.begin() + SIZE_B);
        fault_map.erase(fault_map.begin(), fault_map.begin() + SIZE_B);

        std::cout << "Normal cell selecting..." << std::endl;
        normal_cell.insert(normal_cell.end(), fault_map.begin(), fault_map.begin() + SIZE_N);
        fault_map.erase(fault_map.begin(), fault_map.begin() + SIZE_N);

        std::cout << "Weak cell selecting..." << std::endl;
        weak_cell.insert(weak_cell.end(), fault_map.begin(), fault_map.begin() + SIZE_W);
        fault_map.erase(fault_map.begin(), fault_map.begin() + SIZE_W);

        std::cout << "Good cell selecting..." << std::endl;
        good_cell.insert(good_cell.end(), fault_map.begin(), fault_map.end());
        fault_map.erase(fault_map.begin(), fault_map.end());

        std::cout << "DONE" << std::endl;
        fault_map.clear();

        //Shrink G,N,W,B cell vector
        good_cell.shrink_to_fit();
        normal_cell.shrink_to_fit();
        weak_cell.shrink_to_fit();
        bad_cell.shrink_to_fit();

        SIZE_G = good_cell.size();
        SIZE_N = normal_cell.size();
        SIZE_W = weak_cell.size();
        SIZE_B = bad_cell.size();

        //Initialized fault-map
        fault_map.assign(SIZE, 0);

    }

    void DRAM::Good_state() {
        std::cout << "Good_State" << std::endl;
        std::cout << good_cell.size() << std::endl;
        for (int i = 0; i < SIZE_G; i++) {
            std::cout << good_cell[i] << ' ';
        }
        std::cout << std::endl << std::endl;
    }

    void DRAM::Normal_state() {
        std::cout << "Normal_State" << std::endl;
        std::cout << normal_cell.size() << std::endl;
        for (int i = 0; i < SIZE_N; i++) {
            std::cout << normal_cell[i] << ' ';
        }
        std::cout << std::endl << std::endl;
    }

    void DRAM::Week_state() {
        std::cout << "Weak_State" << std::endl;
        std::cout << weak_cell.size() << std::endl;
        for (int i = 0; i < SIZE_W; i++) {
            std::cout << weak_cell[i] << ' ';
        }
        std::cout << std::endl << std::endl;
    }

    void DRAM::Bad_state() {
        std::cout << "Bad_State" << std::endl;
        std::cout << bad_cell.size() << std::endl;
        for (int i = 0; i < SIZE_B; i++) {
            std::cout << bad_cell[i] << ' ';
        }
        std::cout << std::endl << std::endl;
    }

    void DRAM::State_change() {

        //Generate random state transition probability
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0, 1);

        weak_to_normal.clear();
        normal_to_weak.clear();
        indices_to_erase_or_fault.clear();
        probability_v.clear();

        //Normal to Weak State Transition
        for(int i = 0; i < SIZE_N; i++){
            probability_v.push_back(dis(gen));
        }
        auto condition = [temp = config_.Pnw](double x){ return x < temp; };

        for (auto it = probability_v.begin(); it != probability_v.end(); ++it){
            if (condition(*it)){
                indices_to_erase_or_fault.push_back(std::distance(probability_v.begin(), it));
            }
        }

        std::sort(indices_to_erase_or_fault.rbegin(), indices_to_erase_or_fault.rend());
        for (int index : indices_to_erase_or_fault) {
            normal_to_weak.push_back(normal_cell[index]);
            normal_cell.erase(normal_cell.begin() + index);
        }

        //Weak to Normal State Transition
        probability_v.clear();
        indices_to_erase_or_fault.clear();

        for(int i = 0; i < SIZE_W; i++){
            probability_v.push_back(dis(gen));
        }
        auto condition2 = [temp = config_.Pwn](double x){ return x < temp; };

        for (auto it = probability_v.begin(); it != probability_v.end(); ++it){
            if (condition2(*it)){
                indices_to_erase_or_fault.push_back(std::distance(probability_v.begin(), it));
            }
        }

        std::sort(indices_to_erase_or_fault.rbegin(), indices_to_erase_or_fault.rend());
        for (int index : indices_to_erase_or_fault) {
            weak_to_normal.push_back(weak_cell[index]);
            weak_cell.erase(weak_cell.begin() + index);
        }

        //Merge normal cell with weak to normal cell, weak cell with normal to weak cell
        normal_cell.insert(normal_cell.end(), weak_to_normal.begin(), weak_to_normal.end());
        weak_cell.insert(weak_cell.end(), normal_to_weak.begin(), normal_to_weak.end());

        SIZE_G = good_cell.size();
        SIZE_N = normal_cell.size();
        SIZE_W = weak_cell.size();
        SIZE_B = bad_cell.size();

        //Initialized vector capacity
        weak_to_normal.clear();
        normal_to_weak.clear();
        indices_to_erase_or_fault.clear();
        probability_v.clear();
    }

    void DRAM::Fault_injection(){
        //Generate random fault injection probability
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0, 1);

        indices_to_erase_or_fault.clear();
        probability_v.clear();

        //Good cell fault injection probability
        for(int i = 0; i < SIZE_G; i++){
            probability_v.push_back(dis(gen));
        }
        auto condition = [temp = config_.Pg](double x){ return x < temp; };
        for (auto it = probability_v.begin(); it != probability_v.end(); ++it){
            if (condition(*it)){
                indices_to_erase_or_fault.push_back(std::distance(probability_v.begin(), it));
            }
        }
        for (auto it = indices_to_erase_or_fault.begin(); it != indices_to_erase_or_fault.end(); ++it){
            fault_map[good_cell[*it]]++;
        }
        indices_to_erase_or_fault.clear();
        probability_v.clear();

        //Normal cell fault injection probability
        for(int i = 0; i < SIZE_N; i++){
            probability_v.push_back(dis(gen));
        }
        auto condition2 = [temp = config_.Pn](double x){ return x < temp; };

        for (auto it = probability_v.begin(); it != probability_v.end(); ++it){
            if (condition2(*it)){
                indices_to_erase_or_fault.push_back(std::distance(probability_v.begin(), it));
            }
        }

        for (auto it = indices_to_erase_or_fault.begin(); it != indices_to_erase_or_fault.end(); ++it){
            fault_map[normal_cell[*it]]++;
        }

        indices_to_erase_or_fault.clear();
        probability_v.clear();

        //Weak cell fault injection probability
        for(int i = 0; i < SIZE_W; i++){
            probability_v.push_back(dis(gen));
        }
        auto condition3 = [temp = config_.Pw](double x){ return x < temp; };

        for (auto it = probability_v.begin(); it != probability_v.end(); ++it){
            if (condition3(*it)){
                indices_to_erase_or_fault.push_back(std::distance(probability_v.begin(), it));
            }
        }

        for (auto it = indices_to_erase_or_fault.begin(); it != indices_to_erase_or_fault.end(); ++it){
            fault_map[weak_cell[*it]]++;
        }

        indices_to_erase_or_fault.clear();
        probability_v.clear();

        //Bad cell fault injection probability
        for(int i = 0; i < SIZE_B; i++){
            probability_v.push_back(dis(gen));
        }
        auto condition4 = [temp = config_.Pb](double x){ return x < temp; };

        for (auto it = probability_v.begin(); it != probability_v.end(); ++it){
            if (condition4(*it)){
                indices_to_erase_or_fault.push_back(std::distance(probability_v.begin(), it));
            }
        }

        for (auto it = indices_to_erase_or_fault.begin(); it != indices_to_erase_or_fault.end(); ++it){
            fault_map[bad_cell[*it]]++;
        }

        indices_to_erase_or_fault.clear();
        probability_v.clear();
    }

    void DRAM::PrintFaultmap(){
        for (auto it = fault_map.begin(); it != fault_map.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;
    }
}