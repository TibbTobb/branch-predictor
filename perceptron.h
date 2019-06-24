//
// Created by toby on 26/02/19.
//

#ifndef BRANCH_PREDICTOR_PERCEPTRON_H
#define BRANCH_PREDICTOR_PERCEPTRON_H


#include "predictor.h"

class perceptron : public predictor {
public:
    perceptron(int address_bits, int history_bits_, int weight_bits, int threshold);
    ~perceptron();
    int predict(unsigned int addr, int taken);
    unsigned int get_size(){return size;};
private:
    int size;
    int address_bits;
    int history_bits;
    int threshold;
    int weight_bits;
    int weight_min;
    int weight_max;
    int weight_table_size;
    short *weight_table;
    int global_history;
    int mask;
    int num_previous;
};


#endif //BRANCH_PREDICTOR_PERCEPTRON_H
