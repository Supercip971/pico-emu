#include "utils.h"
#include <stdio.h>

uint32_t add_with_carry(uint32_t x, uint32_t y, bool carry, struct add_with_carry_result *target)
{
    int32_t signed_sum = (int32_t)x + (int32_t)y + (uint32_t)(carry);
    uint32_t unsigned_sum = x + y + (uint32_t)(carry);
    target->result = unsigned_sum & (0b01111111111111111111111111111111);
    if ((uint32_t)target->result != unsigned_sum)
    {
        target->carry = 1;
    }
    else
    {
        target->carry = 0;
    }

    if ((int32_t)target->result != signed_sum)
    {
        target->overflow = 1;
    }
    else
    {
        target->overflow = 0;
    }
    return target->result;
}

uint32_t lsl_c(uint32_t value, int amount, struct shift_c_result* res){
    res->result = value << amount;
    res->carry = bit_get(value,32-amount);
    return res->result;
}
uint32_t lsr_c(uint32_t value, int amount, struct shift_c_result* res){
    printf("error: lsr_c is not implemented \n");
    return 0;
}
uint32_t asr_c(uint32_t value, int amount, struct shift_c_result* res){
    printf("error: asr_c is not implemented \n");
    return 0;

}
uint32_t ror_c(uint32_t value, int amount, struct shift_c_result* res){
    printf("error: ror_c is not implemented \n");
    return 0;

}
uint32_t rrx_c(uint32_t value, bool carry_in, struct shift_c_result* res){
    printf("error: rrx_c is not implemented \n");
    return 0;

}

uint32_t shift_c(uint32_t value, int shift_type, int amount, bool carry_in, struct shift_c_result* res){
    if(amount == 0)
    {
        res->carry = carry_in;
        res->result = value;
        return value;
    }   
    switch (shift_type)
    {
    case SRType_LSL:
        return lsl_c(value, amount, res);
    case SRType_LSR:
        return lsr_c(value, amount, res);
    case SRType_ASR:
        return asr_c(value, amount, res);
    case SRType_ROR:
        return ror_c(value, amount, res);
    case SRType_RRX:
        return rrx_c(value, amount, res);
    
    default:
        printf("invalid shift_c shift type %i", shift_type);
        return 0;
    }
}
