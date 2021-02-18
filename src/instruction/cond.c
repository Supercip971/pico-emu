#include "cond.h"
// for more information see ARMv6 architecture
int is_condition_met(enum condition_set cond, struct APS_Register aps_reg)
{
    switch (cond)
    {

    case COND_EQUAL:
        return aps_reg.zero_condition == 1;

    case COND_NOT_EQUAL:
        return aps_reg.zero_condition != 1;

    case COND_CARRY_SET:
        return aps_reg.carry_flag == 1;

    case COND_CARRY_CLEAR:
        return aps_reg.carry_flag != 1;

    case COND_MINUS_NEGATIVE:
        return aps_reg.negative_condition == 1;

    case COND_PLUS_POSITIVE:
        return aps_reg.negative_condition != 1;

    case COND_OVERFLOW:
        return aps_reg.overflow_flag == 1;

    case COND_NO_OVERFLOW:
        return aps_reg.overflow_flag != 1;

    case COND_UNSIGNED_HIGHER:
        return aps_reg.carry_flag == 1 && aps_reg.zero_condition == 0;

    case COND_UNSIGNED_LOWER_OR_SAME:
        return aps_reg.carry_flag == 0 && aps_reg.zero_condition == 1;

    case COND_SIGNED_GREATER_OR_EQUAL:
        return aps_reg.negative_condition == aps_reg.overflow_flag;

    case COND_SIGNED_LESS_THAN:
        return aps_reg.negative_condition != aps_reg.overflow_flag;

    case COND_SIGNED_GREATER_THAN:
        return (aps_reg.negative_condition) == 0 && (aps_reg.overflow_flag == aps_reg.negative_condition);

    case COND_SIGNED_LESS_OR_EQUAL:
        return (aps_reg.zero_condition) == 1 && (aps_reg.overflow_flag != aps_reg.negative_condition);

    case COND_NONE_CONDITION:
        return 1;

    default:
        return -1;
    }
}