#include "utils.h"

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