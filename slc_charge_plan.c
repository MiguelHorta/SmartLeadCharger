#include "slc_charge_plan.h"
#include "slc_oscilators.h"

static int3float f_target_voltage;
static unsigned int f_target_capacity;
static uint8_t f_active_plan;
static uint8_t f_charge_step;

void setType(ChargeType t)
{
    f_active_plan = (uint8_t)t;
}
void setBatteryVoltage(int3float v)
{
    f_target_voltage = v;
}
void setCapacity(unsigned int c)
{
    f_target_capacity  = c;
}
void startCharge(void)
{
    //XXX REMOVE AFTER IMPLEMENTED
    f_target_voltage = 14.7;
    f_target_capacity = 2200;
    //XXX
    slc_EnableOscilators();
}
void stopCharge(void)
{
    //XXX REMOVE AFTER IMPLEMENTED
    f_target_voltage = 14.7;
    f_target_capacity = 2200;
    //XXX
    slc_DisableOscilators();
}
bool SmartChargeStep1(int3float iout,int3float vout,int3float text){
    
}
bool SmartChargeStep2(int3float iout,int3float vout,int3float text){
    
}
bool SmartChargeStep3(int3float iout,int3float vout,int3float text){
    
}

ChargeStep smart_charge[SMART_CHARGE_STEPS] = {
    {
        "Feeding 0.2A to battery.",
        200,
        0,
        *SmartChargeStep1,
    },
    {
        "Feeding 14.7V to battery.",
        0,
        14700,
        *SmartChargeStep2,
    },
    {
        "Feeding 12V to battery.",
        0,
        12000,
        *SmartChargeStep3,
    }
};

bool FastChargeStep1(int3float iout,int3float vout,int3float text){
    
}

ChargeStep fast_charge[FAST_CHARGE_STEPS] = {
        {
        "Feeding 0.5A to battery.",
        0500,
        0,
        *FastChargeStep1,
    }
};

bool SlowChargeStep1(int3float iout,int3float vout,int3float text){
    
}

ChargeStep slow_charge[SLOW_CHARGE_STEPS] = {
        {
        "Feeding 0.1A to battery.",
        0100,
        0,
        *SlowChargeStep1,
    }
};

ChargeStep* stored_plans[PLAN_COUNT] = {smart_charge, fast_charge, slow_charge}; 

int3float getBatteryVoltage(void)
{
    return f_target_voltage;
}
unsigned int getCapacity(void)
{
    return f_target_capacity;
}
ChargeStep* getActiveStep()
{
    return &stored_plans[f_active_plan][f_charge_step];
}
ControlType getControlType(void)
{
    return stored_plans[f_active_plan][f_charge_step].target_current > 0 ? CURRENT: VOLTAGE;
}

int3float getControlValue(void)
{
    return stored_plans[f_active_plan][f_charge_step].target_current > 0 ? 
        stored_plans[f_active_plan][f_charge_step].target_current : 
        stored_plans[f_active_plan][f_charge_step].target_voltage;
}

void OnCheckFaultyConditions(void)
{
    
}
ChargeType getType(void){
    return f_active_plan;
}