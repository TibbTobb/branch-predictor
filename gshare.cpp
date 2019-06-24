//
// Created by toby on 26/02/19.
//

#include <cstdio>
#include "gshare.h"

gshare::gshare(int bits_) {
    bits = bits_;
    pattern_table_size = (1<<bits);
    size = 2*pattern_table_size + bits;
    pattern_table = new char[pattern_table_size];
    for(int i=0; i<pattern_table_size; i++) {
        pattern_table[i] = 1;
    }
    global_history = 0;
    mask = (1<<(bits-1))-1;
}

int gshare::predict(unsigned int addr, int taken) {
    int entry = (addr/2)%pattern_table_size;
    entry = entry ^ global_history;
    if((entry<0)||entry>pattern_table_size) printf("error!\n");
    int pred_taken = (pattern_table[entry]>=2);

    //2-bit saturating counter
    int mod = 0;
    if(taken && pattern_table[entry] <3) mod = 1;
    if(!taken && pattern_table[entry] > 0) mod = -1;
    pattern_table[entry] += mod;

    //update global history
    global_history &= mask;
    global_history = (global_history << 1) + taken;

    return pred_taken;
}

gshare::~gshare() {

}
