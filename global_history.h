//
// Created by toby on 26/02/19.
//

#ifndef BRANCH_PREDICTOR_GLOBAL_HISTORY_H
#define BRANCH_PREDICTOR_GLOBAL_HISTORY_H


#include "predictor.h"

class global_history : public predictor {
public:
    explicit global_history(unsigned int history_bits_);
    ~global_history() override;
    int predict(unsigned int addr, int taken) override;
    unsigned int get_size() override {return size;};
private:
    unsigned int size;
    unsigned int global_history_value;
    unsigned int history_bits;
    unsigned int pattern_table_size;
    char *pattern_table;
    unsigned int mask;
};


#endif //BRANCH_PREDICTOR_GLOBAL_HISTORY_H
