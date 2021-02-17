#ifndef COND_H
#define COND_H
#include "../pico_cpu.h"
enum condition_set{

    COND_EQUAL = 0b0000,
    COND_NOT_EQUAL = 0b0001,
    COND_CARRY_SET = 0b0010,
    COND_CARRY_CLEAR = 0b0011,
    COND_MINUS_NEGATIVE = 0b0100,
    COND_PLUS_POSITIVE = 0b0101,
    COND_OVERFLOW = 0b0110,
    COND_NO_OVERFLOW = 0b0111,
    COND_UNSIGNED_HIGHER = 0b1000,
    COND_UNSIGNED_LOWER_OR_SAME = 0b1001,
    COND_SIGNED_GREATER_OR_EQUAL = 0b1010,
    COND_SIGNED_LESS_THAN = 0b1011,
    COND_SIGNED_GREATER_THAN = 0b1100,
    COND_SIGNED_LESS_OR_EQUAL = 0b1101,
    COND_NONE_CONDITION = 0b1110,

};
// negative : error
// positive : true
// zero     : false
int is_condition_met(enum condition_set cond, struct APS_Register aps_reg);
#endif