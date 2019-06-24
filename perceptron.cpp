//
// Created by toby on 26/02/19.
//

#include <cstdlib>
#include "perceptron.h"

perceptron::perceptron(int address_bits_, int history_bits_, int weight_bits_, int threshold_) {
    address_bits = address_bits_;
    history_bits = history_bits_;
    weight_bits = weight_bits_;
    weight_max = (1<<(weight_bits-1))-1;
    weight_min = -(1<<(weight_bits-1));
    threshold = threshold_;
    weight_table_size = (1<<address_bits);
    size = weight_bits*weight_table_size*(history_bits_+1);
    weight_table = new short[(history_bits+1)*weight_table_size];
    mask = mask = (1<<(history_bits-1))-1;
    global_history = 0;
    num_previous = 0;
}

int perceptron::predict(unsigned int addr, int taken) {
    //make prediction
    int entry = (addr/2)%weight_table_size;
    entry *= (history_bits+1);
    int y = weight_table[entry];
    for(int i=0; i<num_previous; i++) {
        int x = (global_history >> i) & 1;
        if(x == 0 ) x = -1;
        y += weight_table[entry+i+1]*x;
    }
    int predict_taken = 0;
    if(y>= 0) predict_taken = 1;

    //train perceptron
    //threshold prevents overtraining
    if(abs(y) <= threshold || predict_taken != taken) {
        if (taken && weight_table[entry] < weight_max) {
            weight_table[entry]++;
        } else if (weight_table[entry] > weight_min) {
            weight_table[entry]--;
        }
        for (int i = 0; i < num_previous; i++) {
            int x = (global_history >> i) & 1;
            if (taken == x && weight_table[entry + i + 1] < weight_max) {
                weight_table[entry + i + 1]++;
            } else if (taken != x && weight_table[entry + i + 1] > weight_min)
                weight_table[entry + i + 1]--;
        }
    }

    //update global history
    global_history &= mask;
    global_history = (global_history << 1) + taken;
    if(num_previous < history_bits) {
        num_previous++;
    }

    return predict_taken;
}

perceptron::~perceptron() {

}
