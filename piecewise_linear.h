//
// Created by toby on 26/02/19.
//

#ifndef BRANCH_PREDICTOR_PIECEWISE_LINEAR_H
#define BRANCH_PREDICTOR_PIECEWISE_LINEAR_H


#include "predictor.h"

class piecewise_linear : public predictor{
public:
    piecewise_linear(int global_history_length, int weight_bits, int n_bits, int m_bits, int threshold);
    ~piecewise_linear();
    int predict(unsigned int addr, int taken);
    unsigned int get_size(){return size;};
private:
    int hash(int i, int j, int k, int n);
    int global_history_length;
    int weight_bits;
    int weight_max;
    int weight_min;
    int n_bits;
    int m_bits;
    int n;
    int m;
    int size;
    int global_history;
    int weight_table_size;
    unsigned int *prev_addrs;
    int *weight_table;
    int threshold;
    int mask;
    int num_previous;
};


#endif //BRANCH_PREDICTOR_PIECEWISE_LINEAR_H
