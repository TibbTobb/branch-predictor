//
// Created by toby on 26/02/19.
//

#include "choice.h"

choice::choice(unsigned int choice_history_bits_, unsigned int local_address_bits_, unsigned int local_history_bits_,
               unsigned int global_history_bits_) {
    choice_history_bits = choice_history_bits_;
    local_address_bits = local_address_bits_;
    local_history_bits = local_history_bits_;
    global_history_bits = global_history_bits_;

    local_predictor = new local_history(local_address_bits, local_history_bits);
    global_predictor = new global_history(global_history_bits);

    pattern_table_size = (1U<<choice_history_bits);
    pattern_table = new char[pattern_table_size];
    size = pattern_table_size*2 + local_predictor->get_size() + global_predictor->get_size();
    global_history_value = 0;
    mask = (1U<<(choice_history_bits-1))-1;
    for(int i=0; i<pattern_table_size; i++) {
        pattern_table[i] = 1;
    }
}

int choice::predict(unsigned int addr, int taken) {
    int global_predict_taken = global_predictor->predict(addr, taken);
    int local_predict_taken = local_predictor->predict(addr, taken);
    int predict_taken;
    if(pattern_table[global_history_value]>=2) {
        predict_taken = global_predict_taken;
    } else {
        predict_taken = local_predict_taken;
    }

    //2-bit saturating counter
    int global_correct = (global_predict_taken == taken);
    int local_correct = (local_predict_taken == taken);
    int mod = 0;
    if(global_correct && !local_correct && pattern_table[global_history_value] <3) mod = 1;
    if(!global_correct && local_correct && pattern_table[global_history_value] > 0) mod = -1;
    pattern_table[global_history_value] += mod;

    //update global history
    global_history_value &= mask;
    global_history_value = (global_history_value << 1) + taken;

    return predict_taken;
}

choice::~choice() {

}
