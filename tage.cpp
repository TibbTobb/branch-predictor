//
// Created by toby on 27/02/19.
//

#include <vector>
#include <cstdlib>
#include "tage.h"

//Tagged Geometric
tage::tage() {
    tagged_entries = 1024;
    unsigned int tag_length = 11;
    unsigned int useful_length = 2;
    unsigned int pred_length = 3;
    tagged_components = new tagged_component[7];
    for(int i=0; i<7;i++) {
        tagged_components[i].initilize(tagged_entries, history_lengths[i], tag_length, pred_length, useful_length);
    }
    base_predictor_entries = 8192;
    base_predictor = new char[base_predictor_entries];
    for(int i=0; i<base_predictor_entries; i++) {
        base_predictor[i] = 1;
    }
    global_history1 = 0;
    global_history2 = 0;
}

tage::~tage() {

}

int tage::predict(unsigned int addr, int taken) {
    int entry = addr%base_predictor_entries;
    int base_prediction = (base_predictor[entry]>=2);
    int alternative_prediction = 0;
    int best_prediction = base_prediction;
    int longest_match = -1;

    unsigned int hash_vals[7];
    unsigned int tag[7];
    for(int i=0; i<7; i++) {
        uint64_t masked1;
        uint64_t masked2;
        if(history_lengths[i]<64) {
            uint64_t mask1 = ((UINT64_C(1) << (history_lengths[i])) - 1);
            masked1 = global_history1 & mask1;
            masked2 = 0;
        } else {
             uint64_t mask2 = (((UINT64_C(1)<<(history_lengths[i]-64))-1));
             masked1 = global_history1;
             masked2 = global_history2 & mask2;
        }
        hash_vals[i] = hash3(addr, masked1, masked2, tagged_entries);
        tagged_line line = tagged_components[i].tagged_lines[hash_vals[i]];
        unsigned int tag_max = ((1<<11));
        tag[i] = hash3(addr, masked1, masked2, tag_max);
        if(line.tag == tag[i]) {
            //hit, find longest match
            alternative_prediction = best_prediction;
            best_prediction = (line.prediction >=4);
            longest_match = i;
        }
    }

    //2-bit saturating counter
    int mod = 0;
    if(taken && base_predictor[entry] <3) mod = 1;
    if(!taken && base_predictor[entry] > 0) mod = -1;
    base_predictor[entry] += mod;

    //update global history
    uint64_t mask = (UINT64_C(1)<<(63));
    int copy = static_cast<int>((global_history1 & mask)>>63);
    global_history1 = (global_history1 << 1) + taken;
    global_history2 = (global_history2 << 1) + copy;

    if(longest_match >= 0) {
        tagged_line *longest_line = &(tagged_components[longest_match].tagged_lines[hash_vals[longest_match]]);
        if (alternative_prediction != best_prediction) {
            //update useful 2-bit saturating counter
            int mod = 0;
            if ((best_prediction == taken) && (longest_line->useful < 3)) mod = 1;
            if ((best_prediction != taken) && (longest_line->useful > 0)) mod = -1;
            longest_line->useful += mod;
        }
        //update prediction 3-bit saturating counter
        int mod = 0;
        if (taken && (longest_line->prediction < 7)) mod = 1;
        if (!taken && (longest_line->prediction > 0)) mod = -1;
        longest_line->prediction += mod;
    }
        if(best_prediction != taken) {
            //allocate new line on incorrect prediction
            bool free_lines[7];
            int num_free = 0;
            for (int i = longest_match + 1; i < 7; i++) {
                tagged_line *line = &tagged_components[i].tagged_lines[hash_vals[i]];
                if (line->useful == 0) {
                    free_lines[i] = true;
                    num_free++;
                } else{
                    free_lines[i] = false;
                }
            }
            if (num_free>0) {
                int position = 0;
                if(num_free > 1) {
                    int rand_num = rand() % 100;
                    if(rand_num < 33) {
                        position = 1;
                    }
                }
                int free_position = 0;
                for(int i=longest_match+1; i<7; i++) {
                    if(free_lines[i]) {
                        if(position == 0) {
                            free_position = i;
                            break;
                        } else {
                            position = 0;
                        }
                    }

                }
                tagged_line *line = &tagged_components[free_position].tagged_lines[hash_vals[free_position]];
                line->tag = tag[free_position];
                line->useful = 0;
                if (taken) {
                    line->prediction = 4;
                } else {
                    line->prediction = 3;
                }

            } else {
                for (int i = longest_match + 1; i < 7; i++) {
                    tagged_line *line = &tagged_components[i].tagged_lines[hash_vals[i]];
                    line->useful--;
                }
            }
        }

    return best_prediction;
}

unsigned int tage::hash2(int i, int j, int n) {
    long hi = i*511387U;
    long hj = j*660509U;
    return static_cast<unsigned int>((hi ^ hj) % long(n));
}

unsigned int tage::hash3(unsigned int i, uint64_t j, uint64_t k, unsigned int n) {
    uint64_t result = (j ^ k) ^ i;
    unsigned int result2 = static_cast<unsigned int>(result % n);
    return result2;
    /*
    long hi = i*511387U;
    long hj = j*660509U;
    long hk = k*1289381U;
    return static_cast<unsigned int>((hi ^ hj ^ hk) % long(n));
     */
}
