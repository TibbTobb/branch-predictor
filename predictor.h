//
// Created by toby on 26/02/19.
//

#ifndef BRANCH_PREDICTOR_PREDICTOR_H
#define BRANCH_PREDICTOR_PREDICTOR_H


class predictor {
public:
    predictor(){}
    virtual ~predictor() = default;
    virtual int predict(unsigned int addr, int taken) = 0;
    virtual unsigned int get_size() = 0;
};


#endif //BRANCH_PREDICTOR_PREDICTOR_H
