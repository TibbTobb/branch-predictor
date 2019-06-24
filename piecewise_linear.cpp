//
// Created by toby on 26/02/19.
//

#include <cstdlib>
#include <algorithm>
#include "piecewise_linear.h"

piecewise_linear::piecewise_linear(int global_history_length_, int weight_bits_, int n_bits_, int m_bits_, int threshold_) {
    global_history_length = global_history_length_;
    weight_bits = weight_bits_;
    weight_max = (1<<(weight_bits-1))-1;
    weight_min = -(1<<(weight_bits-1));
    n_bits = n_bits_;
    n = (1<<n_bits);
    m_bits = m_bits_;
    m = (1<<m_bits);
    threshold = threshold_;

    num_previous = 0;
    global_history = 0;
    weight_table_size = n*m*(global_history_length+1);
    prev_addrs = new unsigned int[global_history_length];
    for(int i=0; i<global_history_length_; i++) {
        prev_addrs[i] = 0;
    }
    weight_table = new int[weight_table_size];
    for(int i=0; i<weight_table_size; i++) {
        weight_table[i] = 0;
    }

    mask = (1<<(global_history_length-1))-1;

    size = weight_table_size*weight_bits + global_history_length + global_history_length*m;
}

int piecewise_linear::predict(unsigned int addr, int taken) {
    //prediction
    //int entry = (addr/2)%n;
    int entry = addr;
    //entry *= m*(global_history_length+1);
    int hash_val = hash(entry, 0, 0, weight_table_size);
    int output = weight_table[hash_val];
    for(int i=0; i<num_previous; i++) {
        //int prev_addr_index = prev_addrs[i]%m;
        int prev_addr_index = prev_addrs[i];
        int hash_val_2 = hash(entry, prev_addr_index, i+1, weight_table_size);
        int x = (global_history >> i) & 1;
        if(x) {
            output += weight_table[hash_val_2];
        } else {
            output -= weight_table[hash_val_2];
        }
    }
    int predict_taken = (output >= 0);

    //training
    if(abs(output) < threshold || ((output >= 0) != taken)) {
        if (taken && weight_table[hash_val] < weight_max) {
            weight_table[hash_val]++;
        } else if (weight_table[hash_val] > weight_min) {
            weight_table[hash_val]--;
        }
        for (int i = 0; i < num_previous; i++) {
            //int prev_addr_index = prev_addrs[i] % m;
            int prev_addr_index = prev_addrs[i];
            int hash_val_2 = hash(entry, prev_addr_index, i+1, weight_table_size);
            int x = (global_history >> i) & 1;
            if(taken == x && weight_table[hash_val_2] < weight_max) {
                    weight_table[hash_val_2]++;
            } else if (taken != x && weight_table[hash_val_2] > weight_min) {
                weight_table[hash_val_2]--;
            }
        }
    }

    //update address history
    std::rotate(prev_addrs, prev_addrs+m-1, prev_addrs+m-1);
    prev_addrs[0] = addr;
    if(num_previous < global_history_length) {
        num_previous++;
    }
    //update global history
    global_history &= mask;
    global_history = (global_history << 1) + taken;

    return predict_taken;
}

piecewise_linear::~piecewise_linear() {

}

int piecewise_linear::hash(int i, int j, int k, int n) {
    long hi = i*511387U;
    long hj = j*660509U;
    long hk = k*1289381U;
    int hash = static_cast<int>((hi ^ hj ^ hk) % long(n));
    return hash;
}
