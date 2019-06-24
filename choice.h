//
// Created by toby on 26/02/19.
//

#ifndef BRANCH_PREDICTOR_CHOICE_H
#define BRANCH_PREDICTOR_CHOICE_H


#include "predictor.h"
#include "global_history.h"
#include "local_history.h"

class choice : public predictor{
public:
    choice(unsigned int choice_history_bits_, unsigned int local_address_bits_, unsigned int local_history_bits_,
           unsigned int global_history_bits_);
    ~choice();
    int predict(unsigned int addr, int taken);
    unsigned int get_size(){return size;};
private:
    unsigned int choice_history_bits, local_address_bits, local_history_bits, global_history_bits, pattern_table_size, size;
    global_history *global_predictor;
    local_history *local_predictor;
    char *pattern_table;
    unsigned int global_history_value;
    unsigned int mask;
};


#endif //BRANCH_PREDICTOR_CHOICE_H
