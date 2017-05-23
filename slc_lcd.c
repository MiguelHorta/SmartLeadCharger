#include "slc_lcd.h"
#include "slc_adc.h"

typedef enum LcdStates
{
    STATUS = 0x0,
    START_CHARGE = 0x1,
    START_DISCHARGE = 0x2,
    SENSORS = 0x3,
    CUSTOM_OUTPUT = 0x4,
    SENSORS_IOUT = 0x5,
    SENSORS_VOUT = 0x6,
    SENSORS_BATT = 0x7,
    SENSORS_TEMP_INT = 0x8,
    SENSORS_TEMP_EXT = 0x9,
}LcdStates;

static unsigned int current_lcd = STATUS;
void ChangeState(LcdStates state);
/* START CHARGE STATE */
void OnStatus(void)
{
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "STATUS STATUS   STATUS STATUS   ");
}
void OnStatusTick(void)
{
    0;
}
void StatusOk(void)
{
    0;
}
void StatusCancel(void)
{
    0;
}
void StatusMinus(void)
{
    ChangeState(CUSTOM_OUTPUT);
}
void StatusPlus(void)
{
    ChangeState(START_CHARGE);
}
/*  END** START CHARGE STATE */
/* START CHARGE STATE */
void OnStartCharge(void)
{
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "New charge plan Charge battery--");
}
void OnStartChargeTick(void)
{
    0;
}
void StartChargeOk(void)
{
    0;
}
void StartChargeCancel(void)
{
    0;
}
void StartChargeMinus(void)
{
    ChangeState(STATUS);
}
void StartChargePlus(void)
{
    ChangeState(START_DISCHARGE);
}
/*  END** START CHARGE STATE */
/* START DISCHARGE STATE */
void OnStartDischarge(void)
{
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Discharge plan  Discharge batter");
}
void OnStartDischargeTick(void)
{
    0;
}
void StartDischargeOk(void)
{
    0;
}
void StartDischargeCancel(void)
{
    0;
}
void StartDischargeMinus(void)
{
    ChangeState(START_CHARGE);
}
void StartDischargePlus(void)
{
    ChangeState(SENSORS);
}
/*  END** START DISCHARGE STATE */
/* START DISCHARGE STATE */
void OnSensors(void)
{
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Sensors         Sensors state");
}
void OnSensorsTick(void)
{
    0;
}
void SensorsOk(void)
{
    ChangeState(SENSORS_IOUT);
}
void SensorsCancel(void)
{
    0;
}
void SensorsMinus(void)
{
    ChangeState(START_DISCHARGE);
}
void SensorsPlus(void)
{
    ChangeState(CUSTOM_OUTPUT);
}
/*  END** START DISCHARGE STATE */
/* START DISCHARGE STATE */
void OnCustomOutput(void)
{
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Custom output   CUSTOM OUTPUT");
}
void OnCustomOutputTick(void)
{
    0;
}
void CustomOutputOk(void)
{
    0;
}
void CustomOutputCancel(void)
{
    0;
}
void CustomOutputMinus(void)
{
    ChangeState(SENSORS);
}
void CustomOutputPlus(void)
{
    ChangeState(STATUS);
}
/*  END** START DISCHARGE STATE */
/* START CHARGE STATE */
void OnSensorsIOUTTick(void)
{
    static char str[33];
    sprintf(str, "IOUT            IOUT: %d", slc_CurrentValue());
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, str);
}
void OnSensorsIOUT(void)
{
    OnSensorsIOUTTick();
}
void SensorsIOUTOk(void)
{
    0;
}
void SensorsIOUTCancel(void)
{
    ChangeState(SENSORS);
}
void SensorsIOUTMinus(void)
{
    ChangeState(SENSORS_TEMP_EXT);
}
void SensorsIOUTPlus(void)
{
    ChangeState(SENSORS_VOUT);
}
/*  END** START CHARGE STATE */
/* START CHARGE STATE */
void OnSensorsVOUTTick(void)
{
    static char str[33];
    sprintf(str, "VOUT            VOUT: %d", slc_VredValue());
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, str);
}
void OnSensorsVOUT(void)
{
    OnSensorsVOUTTick();
}
void SensorsVOUTOk(void)
{
    0;
}
void SensorsVOUTCancel(void)
{
    ChangeState(SENSORS);
}
void SensorsVOUTMinus(void)
{
    ChangeState(SENSORS_IOUT);
}
void SensorsVOUTPlus(void)
{
    ChangeState(SENSORS_BATT);
}
/*  END** START CHARGE STATE */
/* START CHARGE STATE */
void OnSensorsBATTTick(void)
{
    static char str[33];
    sprintf(str, "BATT            BATT: %d", slc_BattValue());
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, str);
}
void OnSensorsBATT(void)
{
    OnSensorsBATTTick();
}
void SensorsBATTOk(void)
{
    0;
}
void SensorsBATTCancel(void)
{
    ChangeState(SENSORS);
}
void SensorsBATTMinus(void)
{
    ChangeState(SENSORS_VOUT);
}
void SensorsBATTPlus(void)
{
    ChangeState(SENSORS_TEMP_INT);
}
/*  END** START CHARGE STATE */
/* START CHARGE STATE */
void OnSensorsTEMPINTTick(void)
{
    static char str[33];
    sprintf(str, "TEMPINT         TEMP: %d", slc_TempIntValue());
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, str);
}
void OnSensorsTEMPINT(void)
{
    OnSensorsTEMPINTTick();
}
void SensorsTEMPINTOk(void)
{
    0;
}
void SensorsTEMPINTCancel(void)
{
    ChangeState(SENSORS);
}
void SensorsTEMPINTMinus(void)
{
    ChangeState(SENSORS_BATT);
}
void SensorsTEMPINTPlus(void)
{
    ChangeState(SENSORS_TEMP_EXT);
}
/*  END** START CHARGE STATE */
/* START CHARGE STATE */
void OnSensorsTEMPEXTTick(void)
{
    static char str[33];
    sprintf(str, "TEMPEXT         TEMP: %d", slc_TempBattValue());
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, str);
}
void OnSensorsTEMPEXT(void)
{
    OnSensorsTEMPEXTTick();
}
void SensorsTEMPEXTOk(void)
{
    0;
}
void SensorsTEMPEXTCancel(void)
{
    ChangeState(SENSORS);
}
void SensorsTEMPEXTMinus(void)
{
    ChangeState(SENSORS_TEMP_INT);
}
void SensorsTEMPEXTPlus(void)
{
    ChangeState(SENSORS_IOUT);
}
/*  END** START CHARGE STATE */
void DoNop(void)
{
 0;   
}
static LcdScreen lcd_screens[] =
{
    {
      *OnStatus,
      *OnStatusTick,
      *StatusMinus,
      *StatusPlus,
      *StatusCancel,
      *StatusOk,
    },
    {
      *OnStartCharge,
      *OnStartChargeTick,
      *StartChargeMinus,
      *StartChargePlus,
      *StartChargeCancel,
      *StartChargeOk,
    },
    {
      *OnStartDischarge,
      *OnStartDischargeTick,
      *StartDischargeMinus,
      *StartDischargePlus,
      *StartDischargeCancel,
      *StartDischargeOk
    },
    {
      *OnSensors,
      *OnSensorsTick,
      *SensorsMinus,
      *SensorsPlus,
      *SensorsCancel,
      *SensorsOk
    },
    {
      *OnCustomOutput,
      *OnCustomOutputTick,
      *CustomOutputMinus,
      *CustomOutputPlus,
      *CustomOutputCancel,
      *CustomOutputOk
    },
    {
      *OnSensorsIOUT,
      *OnSensorsIOUTTick,
      *SensorsIOUTMinus,
      *SensorsIOUTPlus,
      *SensorsIOUTCancel,
      *SensorsIOUTOk
    },
    {
      *OnSensorsVOUT,
      *OnSensorsVOUTTick,
      *SensorsVOUTMinus,
      *SensorsVOUTPlus,
      *SensorsVOUTCancel,
      *SensorsVOUTOk
    },
    {
      *OnSensorsBATT,
      *OnSensorsBATTTick,
      *SensorsBATTMinus,
      *SensorsBATTPlus,
      *SensorsBATTCancel,
      *SensorsBATTOk
    },
    {
      *OnSensorsTEMPINT,
      *OnSensorsTEMPINTTick,
      *SensorsTEMPINTMinus,
      *SensorsTEMPINTPlus,
      *SensorsTEMPINTCancel,
      *SensorsTEMPINTOk
    },
    {
      *OnSensorsTEMPEXT,
      *OnSensorsTEMPEXTTick,
      *SensorsTEMPEXTMinus,
      *SensorsTEMPEXTPlus,
      *SensorsTEMPEXTCancel,
      *SensorsTEMPEXTOk
    },
    
};
void ChangeState(LcdStates state)
{
    current_lcd = state;
    (*lcd_screens[current_lcd].new_state)();
}
static void getConstants(int freq,int *prescaler, int *value)
{
	const int MAX_VALUE = 65535;
	const int prescaler_values[] = {1,2,4,8,16,32,64,256};
	int i;
	for(i = 0; i < 8; i++) 
	{
		*value = PBUSCLK/prescaler_values[i]/freq -1 ;
		if(*value < MAX_VALUE)
			break;
	}
	*prescaler = i;
}

void slc_InitLCD(void)
{
    TM_HD44780_Init(16, 2);
    MINUS_BTN_CFG = 1;
    PLUS_BTN_CFG = 1;
    OK_BTN_CFG = 1;
    CANCEL_BTN_CFG = 1;
    int prescaler,value;
	getConstants(6,&prescaler,&value);
	
	T5CONbits.ON = 0; // Stop timer
	IFS0bits.T5IF=0; // Reset interrupt flag 
	T5CONbits.TCKPS = prescaler;
	PR5 = value;
	TMR5 = 0;
	T5CONbits.TON = 1;
	// INterrupçoes para o timer "
	IFS0bits.T5IF = 0;
	IEC0bits.T5IE = 1;
	IPC5bits.T5IP = 2;
    ChangeState(STATUS);
}
void __attribute__( (interrupt(IPL2AUTO), vector(_TIMER_5_VECTOR))) isr_buttons(void)
{
    static unsigned int refresh = 0;
    
    if(CANCEL_BTN)
        (*lcd_screens[current_lcd].cancel)();
    else if(OK_BTN)
        (*lcd_screens[current_lcd].ok)();
    else if(MINUS_BTN)
        (*lcd_screens[current_lcd].minus)();
    else if(PLUS_BTN)
        (*lcd_screens[current_lcd].plus)();
    
    (*lcd_screens[current_lcd].on_tick)();  
	IFS0bits.T5IF = 0;
}