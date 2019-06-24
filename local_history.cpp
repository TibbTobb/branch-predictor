//
// Created by toby on 26/02/19.
//

#include <cstdio>
#include "local_history.h"
//PAg per address (local) branch history and global pattern history table
int local_history::predict(unsigned int addr, int taken) {
    unsigned int entry = (addr/2)%history_table_size;
    if((entry<0)||entry>history_table_size) printf("error!\n");

    unsigned int history = local_history_table[entry];
    int pred_taken = (pattern_table[history]>=2);

    //2-bit saturating counter
    int mod = 0;
    if(taken && pattern_table[history] <3) mod = 1;
    if(!taken && pattern_table[history] > 0) mod = -1;
    pattern_table[history] += mod;

    //shift register
    history &= mask;
    history = (history << 1) + taken;
    local_history_table[entry] = static_cast<short>(history);

    return pred_taken;
}

local_history::local_history(unsigned int address_bits, unsigned int history_bits) {
    pattern_table_size = 1U << history_bits;
    history_table_size = 1U << address_bits;
    size = history_table_size *history_bits + pattern_table_size * 2;
    local_history_table = new unsigned short[history_table_size];
    pattern_table = new char[pattern_table_size];
    mask = (1U<<(history_bits-1))-1;

    for(int i=0; i < history_table_size; i++) {
        local_history_table[i] = 0;
    }
    for(int i=0; i < pattern_table_size; i++) {
        pattern_table[i] = 1;
    }
}

local_history::~local_history() {
    //delete[] local_history_table;
    //delete[] pattern_table;
}