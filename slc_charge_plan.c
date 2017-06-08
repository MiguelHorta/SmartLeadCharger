#include "slc_charge_plan.h"
#include "slc_oscilators.h"
#include "slc_lcd.h"

static int3float f_target_voltage = 14700;
static unsigned int f_target_capacity = 2200;
static uint8_t f_active_plan = CHARGE_TYPE_FIRST;
 uint8_t f_charge_step = 0;
static uint32_t start_time = 0;
static uint32_t duration = 0;
static bool is_finished = true;
static bool has_errors = false;
static bool changed_step = true;

const char ChargeTypeDesc[][14] = {
    "Smart charge ",
    "Fast charge  ",
    "Slow charge  "
};

const uint8_t ChargeTypeSteps[PLAN_COUNT] =
{
    SMART_CHARGE_STEPS,
    FAST_CHARGE_STEPS,
    SLOW_CHARGE_STEPS
};

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
    slc_EnableOscilators();
    start_time = getCurrentTick();
    is_finished = false;
    has_errors = false;
    f_charge_step = 0;
    changed_step = true;
}
void stopCharge(void)
{
    slc_DisableOscilators();
}
bool SmartChargeStep1(int3float iout,int3float vout,int3float text)
{
    if(vout > f_target_voltage)
        return true;
    return false;
}
bool SmartChargeStep2(int3float iout,int3float vout,int3float text)
{
    if(iout < 250)
        return true;
    return false;
}
bool SmartChargeStep3(int3float iout,int3float vout,int3float text)
{
    if(iout > 500)
        return true;
    return false;
    
}
ChargeStep smart_charge[SMART_CHARGE_STEPS] = {
    {
        "0.2A to battery.",
        400,
        0,
        &SmartChargeStep1,
    },
    {
        "14.7V to battery",
        0,
        14700,
        &SmartChargeStep2,
    },
    {
        "12V to battery.",
        0,
        12000,
        &SmartChargeStep3,
    }
};

bool FastChargeStep1(int3float iout,int3float vout,int3float text){
    
}

ChargeStep fast_charge[FAST_CHARGE_STEPS] = {
        {
        "0.5A to battery.",
        0500,
        0,
        *FastChargeStep1,
    }
};

bool SlowChargeStep1(int3float iout,int3float vout,int3float text){
    
}

ChargeStep slow_charge[SLOW_CHARGE_STEPS] = {
        {
        "0.1A to battery.",
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
    if(is_finished)
        return 0;
    return stored_plans[f_active_plan][f_charge_step].target_current > 0 ? 
        stored_plans[f_active_plan][f_charge_step].target_current : 
        stored_plans[f_active_plan][f_charge_step].target_voltage;
}

char const * getChargeTypeDesc(ChargeType ct)
{
    return ChargeTypeDesc[ct];
}
bool OnCheckFaultyConditions(void)
{
    /*if(slc_Vred() < 1000 && slc_Current() < 100)
    {
        return true;
    }*/
    return false;
}
ChargeType getType(void){
    return f_active_plan;
}

bool isFinished(void)
{
    return is_finished;
}

bool hasErrors()
{
    return has_errors;
}
void OnControlTick(void)
{
    if((stored_plans[f_active_plan][f_charge_step].next_step)(slc_Current(), slc_Vred(), slc_TempBatt()))
    {
        f_charge_step++;
        changed_step = true;
        if( f_charge_step >= ChargeTypeSteps[f_active_plan])
        {
            is_finished = true;
            duration = getCurrentTick() - start_time;
            stopCharge();
        }
    }
    if(OnCheckFaultyConditions())
    {
        has_errors = true;
        stopCharge();
    }
    changed_step = false;
}

bool changedStep()
{
    return changed_step;
}