//
// Created by toby on 26/02/19.
//

#include "global_history.h"
//GAg global branch history register and global pattern history table
int global_history::predict(unsigned int addr, int taken) {
    int predict_taken = (pattern_table[global_history_value]>=2);

    //2-bit saturating counter
    int mod = 0;
    if(taken && pattern_table[global_history_value] <3) mod = 1;
    if(!taken && pattern_table[global_history_value] > 0) mod = -1;
    pattern_table[global_history_value] += mod;

    //update global history
    global_history_value &= mask;
    global_history_value = (global_history_value << 1) + taken;

    return predict_taken;
}

global_history::global_history(unsigned int history_bits_) {
    history_bits = history_bits_;
    pattern_table_size = (1U<<history_bits);
    size = pattern_table_size*2+history_bits;
    pattern_table = new char[pattern_table_size];
    global_history_value = 0;
    mask = (1U<<(history_bits-1))-1;
    for(int i=0; i<pattern_table_size; i++) {
        pattern_table[i] = 1;
    }
}

global_history::~global_history() {
    //delete[] pattern_table;
}
