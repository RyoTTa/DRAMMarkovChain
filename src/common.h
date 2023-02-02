#ifndef NAIVEMARKOVCHAIN_COMMON_H
#define NAIVEMARKOVCHAIN_COMMON_H

#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <tuple>


namespace naivemarkov{


struct Address {
    Address()
            : channel(-1), rank(-1), bankgroup(-1), bank(-1), row(-1), column(-1) {}

    Address(int channel, int rank, int bankgroup, int bank, int row, int column, uint64_t hex_addr)
            : channel(channel),
              rank(rank),
              bankgroup(bankgroup),
              bank(bank),
              row(row),
              column(column),
              hex_addr(hex_addr) {}

    Address(const Address &addr)
            : channel(addr.channel),
              rank(addr.rank),
              bankgroup(addr.bankgroup),
              bank(addr.bank),
              row(addr.row),
              column(addr.column),
              hex_addr(addr.hex_addr) {}

    int channel;
    int rank;
    int bankgroup;
    int bank;
    int row;
    int column;
    uint64_t hex_addr;
};

void AbruptExit(const std::string &file, int line);

int LogBase2(int power_of_two);

}
#endif //NAIVEMARKOV_COMMON_H