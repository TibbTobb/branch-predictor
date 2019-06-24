//
// Created by toby on 26/02/19.
//

#ifndef BRANCH_PREDICTOR_LOCAL_HISTORY_H
#define BRANCH_PREDICTOR_LOCAL_HISTORY_H

#include "predictor.h"

class local_history : public predictor {
public:
    local_history(unsigned int address_bits, unsigned int history_bits);
    ~local_history();
    int predict(unsigned int addr, int taken) override;
    unsigned int get_size(){return size;};
private:
    unsigned int history_table_size;
    unsigned int pattern_table_size;
    unsigned short int *local_history_table;
    char *pattern_table;
    unsigned int size;
    unsigned int mask;
};

#endif //BRANCH_PREDICTOR_LOCAL_HISTORY_H
