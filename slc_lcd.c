#include "slc_lcd.h"
#include "slc_adc.h"
#include "slc_charge_plan.h"

typedef enum LcdStates
{
    FIRST_LCD = 0x0,
    DIAGNOSIS = FIRST_LCD,
    START_CHARGE = 0x1,
    START_DISCHARGE = 0x2,
    SENSORS = 0x3,
    CUSTOM_OUTPUT = 0x4,
    LAST_LCD = CUSTOM_OUTPUT,
    SENSORS_IOUT = 0x5,
    SENSORS_VOUT = 0x6,
    SENSORS_BATT = 0x7,
    SENSORS_TEMP_INT = 0x8,
    SENSORS_TEMP_EXT = 0x9,
    DIAGNOSIS_DETAIL = 0xA,
    START_CHARGE_CHOOSE_TYPE = 0xB,
    START_CHARGE_CONFIRM = 0xC,
    START_DISCHARGE_INPUT = 0xD,    
    CUSTOM_OUTPUT_CHOOSER = 0xE,
    CUSTOM_OUTPUT_INPUT = 0xF,
    CHARGING_STATE_0 = 0x10,
    CHARGING_STATE_1 = 0x11,
}LcdStates;

#define LCD_INPUT_LENGHT 5
#define LCD_INPUT_FIRST 3
static char lcd_input[LCD_INPUT_LENGHT+1] = {"00000"};
static int8_t lcd_input_pivot = 0;
static bool lcd_edit = false;
static unsigned int current_lcd = FIRST_LCD;
void ChangeState(LcdStates state);
/* START CHARGE STATE */
void OnDiagnosis(void)
{
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Diagnosis       Get battery info");
}
void OnDiagnosisTick(void)
{
    0;
}
void DiagnosisOk(void)
{
    ChangeState(DIAGNOSIS_DETAIL);
}
void DiagnosisCancel(void)
{
    0;
}
void DiagnosisMinus(void)
{
    ChangeState(LAST_LCD);
}
void DiagnosisPlus(void)
{
    ChangeState(++current_lcd);
}
/*  END** START CHARGE STATE */
/* START CHARGE STATE */
void OnStartCharge(void)
{
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Charge          Start new charge");
}
void OnStartChargeTick(void)
{
    0;
}
void StartChargeOk(void)
{
    ChangeState(START_CHARGE_CHOOSE_TYPE);
}
void StartChargeCancel(void)
{
    0;
}
void StartChargeMinus(void)
{
    ChangeState(DIAGNOSIS);
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
    TM_HD44780_Puts(0, 0, "Discharge       Start discharge ");
}
void OnStartDischargeTick(void)
{
    0;
}
void StartDischargeOk(void)
{
    ChangeState(START_DISCHARGE_INPUT);
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
    TM_HD44780_Puts(0, 0, "Sensors         Get sensor state");
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
    TM_HD44780_Puts(0, 0, "Custom output   Force output    ");
}
void OnCustomOutputTick(void)
{
    0;
}
void CustomOutputOk(void)
{
    ChangeState(CUSTOM_OUTPUT_CHOOSER);
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
    ChangeState(DIAGNOSIS);
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
/* START CHARGE STATE */
void OnDiagnosisDetail(void)
{
    static char str[33];
    sprintf(str, "Battery         %02.1fV  -  %03.1f%%", slc_TempBattValue(), slc_TempBattValue()/12.8);
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, str);
}
void OnDiagnosisDetailTick(void)
{
    0;
}
void DiagnosisDetailOk(void)
{
    0;
}
void DiagnosisDetailCancel(void)
{
    ChangeState(DIAGNOSIS);
}
void DiagnosisDetailMinus(void)
{
    0;;
}
void DiagnosisDetailPlus(void)
{
    0;
}
/*  END** START CHARGE STATE */
/* START CHARGE STATE */
char ChargeTypeDesc[][14] = {
    "Smart charge ",
    "Fast charge  ",
    "Slow charge  "
};
void OnStartChargeChooseType(void)
{
    lcd_input_pivot = CHARGE_TYPE_FIRST;
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "New charge plan:");
    TM_HD44780_PutCustom(0, 1, 0x7F);
    TM_HD44780_PutCustom(15, 1, 0x7E);
    TM_HD44780_Puts(1,1,ChargeTypeDesc[lcd_input_pivot]);
    
}
void OnStartChargeChooseTypeTick(void)
{
    0;
}
void StartChargeChooseTypeOk(void)
{
    setType(lcd_input_pivot);
    ChangeState(START_CHARGE_CONFIRM);
}
void StartChargeChooseTypeCancel(void)
{
    ChangeState(START_CHARGE);
}
void StartChargeChooseTypeMinus(void)
{
    if(--lcd_input_pivot < 0)
        lcd_input_pivot = CHARGE_TYPE_LAST;
    TM_HD44780_Puts(1,1,ChargeTypeDesc[lcd_input_pivot]);
}
void StartChargeChooseTypePlus(void)
{
    if(++lcd_input_pivot > CHARGE_TYPE_LAST)
        lcd_input_pivot = 0;
    TM_HD44780_Puts(1,1,ChargeTypeDesc[lcd_input_pivot]);
}
/*  END** START CHARGE STATE */
/* START CHARGE STATE */
void OnStartChargeConfirm(void)
{
    static char str[33];
    sprintf(str, "%-16s%02.1fV  -  %05.0fC", ChargeTypeDesc[getType()], getBatteryVoltage(), getCapacity());
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, str);
}
void OnStartChargeConfirmTick(void)
{
    0;
}
void StartChargeConfirmOk(void)
{
    startCharge();
    ChangeState(CHARGING_STATE_0);
}
void StartChargeConfirmCancel(void)
{
    ChangeState(START_CHARGE);
}
void StartChargeConfirmMinus(void)
{
    0;
}
void StartChargeConfirmPlus(void)
{
    0;
}
/*  END** START CHARGE STATE */
/* START CHARGE STATE */
void OnStartDischargeInput(void)
{
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Max current:    I: 00000");
    lcd_input_pivot = 0;
    lcd_edit = false;
    //lcd_input = {'0', '0', '2', '0', '0'};
    TM_HD44780_PutCustom(LCD_INPUT_FIRST+LCD_INPUT_LENGHT, 1, 0x7E);
    TM_HD44780_CursorOn();
    TM_HD44780_CursorSet(LCD_INPUT_FIRST,1);
}
void OnStartDischargeInputTick(void)
{
    0;
}
void StartDischargeInputOk(void)
{
    if(lcd_input_pivot == LCD_INPUT_LENGHT)
    {
            // done
        TM_HD44780_CursorOff();
        0;
    }else if(lcd_edit)
    {
        TM_HD44780_CursorSet(LCD_INPUT_FIRST + ++lcd_input_pivot,1);
        if(lcd_input_pivot == LCD_INPUT_LENGHT)
            lcd_edit = false;
    }else if(!lcd_edit)
    {
        TM_HD44780_CursorSet(LCD_INPUT_FIRST+lcd_input_pivot,1);
        TM_HD44780_BlinkOn();
        lcd_edit = true;
    }
}
void StartDischargeInputCancel(void)
{
    if(lcd_edit)
    {
        TM_HD44780_BlinkOff();
        lcd_edit = false;
    }else if(!lcd_edit)
    {
        TM_HD44780_CursorOff();
        ChangeState(START_DISCHARGE);
    }
}
void StartDischargeInputMinus(void)
{
    if(lcd_edit)
    {
        if(--lcd_input[lcd_input_pivot] < 0x30)
            lcd_input[lcd_input_pivot] = 0x39;
        TM_HD44780_PutCustom(LCD_INPUT_FIRST+lcd_input_pivot, 1, lcd_input[lcd_input_pivot]);
    }else
    {
        if(--lcd_input_pivot < 0)
        {
            lcd_input_pivot = LCD_INPUT_LENGHT;
        }
    }
    TM_HD44780_CursorSet(LCD_INPUT_FIRST+lcd_input_pivot,1);
}
void StartDischargeInputPlus(void)
{
    if(lcd_edit)
    {
        if(++lcd_input[lcd_input_pivot] > 0x39)
            lcd_input[lcd_input_pivot] = 0x30;
        TM_HD44780_PutCustom(LCD_INPUT_FIRST+lcd_input_pivot, 1, lcd_input[lcd_input_pivot]);
    }else
    {
        if(++lcd_input_pivot > LCD_INPUT_LENGHT)
        {
            lcd_input_pivot = 0;
        }
    }
    TM_HD44780_CursorSet(LCD_INPUT_FIRST+lcd_input_pivot,1);
}
/*  END** START CHARGE STATE */
/* START CHARGE STATE */
void OnCustomOutputChooser(void)
{
    TM_HD44780_Clear();
    TM_HD44780_PutCustom(0, 0, 0x7E);
    TM_HD44780_Puts(1, 0, "Current");
    TM_HD44780_PutCustom(0, 1, ' ');
    TM_HD44780_Puts(1, 1, "Voltage");
    lcd_input_pivot = 0;
    TM_HD44780_BlinkOn();
    TM_HD44780_CursorSet(0,0);
}
void OnCustomOutputChooserTick(void)
{
    0;
}
void CustomOutputChooserOk(void)
{
    //GO
    //lcd_input_pivot
    0;
}
void CustomOutputChooserCancel(void)
{
    ChangeState(CUSTOM_OUTPUT);
}
void CustomOutputChooserMinus(void)
{
    TM_HD44780_PutCustom(0, lcd_input_pivot, ' ');
    lcd_input_pivot = !lcd_input_pivot;
    TM_HD44780_PutCustom(0, lcd_input_pivot, 0x7E);
    TM_HD44780_CursorSet(0,lcd_input_pivot);
}
void CustomOutputChooserPlus(void)
{
    TM_HD44780_PutCustom(0, lcd_input_pivot, ' ');
    lcd_input_pivot = !lcd_input_pivot;
    TM_HD44780_PutCustom(0, lcd_input_pivot, 0x7E);
    TM_HD44780_CursorSet(0,lcd_input_pivot);
}
/*  END** START CHARGE STATE */
/* START CHARGE STATE */
void OnChargingState0(void)
{
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Charging");
}
void OnChargingState0Tick(void)
{
    TM_HD44780_Puts(0, 1, "QQ");;
}
void ChargingState0Ok(void)
{
    0;
}
void ChargingState0Cancel(void)
{
    ChangeState(START_CHARGE);
    stopCharge();
}
void ChargingState0Minus(void)
{
    0;
}
void ChargingState0Plus(void)
{
    0;
}
/*  END** START CHARGE STATE */
void DoNop(void)
{
 0;
}
static LcdScreen lcd_screens[] =
{
    {
      &OnDiagnosis,
      &OnDiagnosisTick,
      &DiagnosisMinus,
      &DiagnosisPlus,
      &DiagnosisCancel,
      &DiagnosisOk,
    },
    {
      &OnStartCharge,
      &OnStartChargeTick,
      &StartChargeMinus,
      &StartChargePlus,
      &StartChargeCancel,
      &StartChargeOk,
    },
    {
      &OnStartDischarge,
      &OnStartDischargeTick,
      &StartDischargeMinus,
      &StartDischargePlus,
      &StartDischargeCancel,
      &StartDischargeOk
    },
    {
      &OnSensors,
      &OnSensorsTick,
      &SensorsMinus,
      &SensorsPlus,
      &SensorsCancel,
      &SensorsOk
    },
    {
      &OnCustomOutput,
      &OnCustomOutputTick,
      &CustomOutputMinus,
      &CustomOutputPlus,
      &CustomOutputCancel,
      &CustomOutputOk
    },
    {
      &OnSensorsIOUT,
      &OnSensorsIOUTTick,
      &SensorsIOUTMinus,
      &SensorsIOUTPlus,
      &SensorsIOUTCancel,
      &SensorsIOUTOk
    },
    {
      &OnSensorsVOUT,
      &OnSensorsVOUTTick,
      &SensorsVOUTMinus,
      &SensorsVOUTPlus,
      &SensorsVOUTCancel,
      &SensorsVOUTOk
    },
    {
      &OnSensorsBATT,
      &OnSensorsBATTTick,
      &SensorsBATTMinus,
      &SensorsBATTPlus,
      &SensorsBATTCancel,
      &SensorsBATTOk
    },
    {
      &OnSensorsTEMPINT,
      &OnSensorsTEMPINTTick,
      &SensorsTEMPINTMinus,
      &SensorsTEMPINTPlus,
      &SensorsTEMPINTCancel,
      &SensorsTEMPINTOk
    },
    {
      &OnSensorsTEMPEXT,
      &OnSensorsTEMPEXTTick,
      &SensorsTEMPEXTMinus,
      &SensorsTEMPEXTPlus,
      &SensorsTEMPEXTCancel,
      &SensorsTEMPEXTOk
    },
    {
      &OnDiagnosisDetail,
      &OnDiagnosisDetailTick,
      &DiagnosisDetailMinus,
      &DiagnosisDetailPlus,
      &DiagnosisDetailCancel,
      &DiagnosisDetailOk
    },
    {
      &OnStartChargeChooseType,
      &OnStartChargeChooseTypeTick,
      &StartChargeChooseTypeMinus,
      &StartChargeChooseTypePlus,
      &StartChargeChooseTypeCancel,
      &StartChargeChooseTypeOk
    },
    {
      &OnStartChargeConfirm,
      &OnStartChargeConfirmTick,
      &StartChargeConfirmMinus,
      &StartChargeConfirmPlus,
      &StartChargeConfirmCancel,
      &StartChargeConfirmOk
    },
    {
      &OnStartDischargeInput,
      &OnStartDischargeInputTick,
      &StartDischargeInputMinus,
      &StartDischargeInputPlus,
      &StartDischargeInputCancel,
      &StartDischargeInputOk
    },
    {
      &OnCustomOutputChooser,
      &OnCustomOutputChooserTick,
      &CustomOutputChooserMinus,
      &CustomOutputChooserPlus,
      &CustomOutputChooserCancel,
      &CustomOutputChooserOk
    },
    {
      &DoNop,
      &DoNop,
      &DoNop,
      &DoNop,
      &DoNop,
      &DoNop,
    },
    {
      &OnChargingState0,
      &OnChargingState0Tick,
      &ChargingState0Ok,
      &ChargingState0Cancel,
      &ChargingState0Minus,
      &ChargingState0Plus,
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
    MINUS_BTN = 0;
    PLUS_BTN = 0;
    OK_BTN = 0;
    CANCEL_BTN = 0;
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
    ChangeState(FIRST_LCD);
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