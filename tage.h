//
// Created by toby on 27/02/19.
//

#ifndef BRANCH_PREDICTOR_TAGE_H
#define BRANCH_PREDICTOR_TAGE_H


#include <stdint-gcc.h>
#include "predictor.h"
#include "tagged_line.h"
#include "tagged_component.h"

class tage : public predictor {
public:
    tage();
    ~tage();
    int predict(unsigned int addr, int taken);
    //TODO: set size
    unsigned int get_size() {return 0;}

private:
    char *base_predictor;
    tagged_component *tagged_components;
    unsigned int tagged_entries;
    unsigned int base_predictor_entries;
    unsigned int hash2(int i, int j, int n);
    unsigned int hash3(unsigned int i, uint64_t j, uint64_t k, unsigned int n);
    uint64_t global_history1;
    uint64_t global_history2;
    unsigned int history_lengths[7]  = {5, 9, 15, 25, 44, 76, 127};
};


#endif //BRANCH_PREDICTOR_TAGE_H
