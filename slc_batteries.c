#include "slc_batteries.h"

static BatteryVoltage active_voltage = BV_NONE;
static BatteryCapacity active_capacity = BC_NONE;

static float battery_voltage[] = {
    /*BV_NONE*/-1,
    /*BV_14V7*/12.0    
};

static float battery_capacity[] = {
    /*BC_NONE*/-1,
    /*BC_2200*/2200    
};
float getActualVoltagePercentage(float v)
{
    return v/battery_voltage[active_voltage];
}