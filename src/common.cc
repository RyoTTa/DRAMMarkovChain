#include "common.h"

namespace naivemarkov {

    void AbruptExit(const std::string &file, int line) {
        std::cerr << "Exiting Abruptly - " << file << ":" << line << std::endl;
        std::exit(-1);
    }

    int LogBase2(int power_of_two) {
        int i = 0;
        while (power_of_two > 1) {
            power_of_two /= 2;
            i++;
        }
        return i;
    }

}
