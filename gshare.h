//
// Created by toby on 26/02/19.
//

#ifndef BRANCH_PREDICTOR_GSHARE_H
#define BRANCH_PREDICTOR_GSHARE_H


#include "predictor.h"

class gshare : public predictor{
public:
    gshare(int bits_);
    ~gshare();
    int predict(unsigned int addr, int taken);
    unsigned int get_size(){return size;};
private:
    int size;
    int bits;
    int pattern_table_size;
    char *pattern_table;
    int global_history;
    int mask;
};


#endif //BRANCH_PREDICTOR_GSHARE_H
