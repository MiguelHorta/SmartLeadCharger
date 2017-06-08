#include "slc_lcd.h"
#include "slc_adc.h"
#include "slc_batteries.h"
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
    DIAGNOSIS_DETAIL = 0xA, //xxx
    START_CHARGE_CHOOSE_TYPE = 0xB,
    START_CHARGE_CONFIRM = 0xC,
    START_DISCHARGE_INPUT = 0xD,    
    CUSTOM_OUTPUT_CHOOSER = 0xE,
    CUSTOM_OUTPUT_INPUT = 0xF, //xxx
    CUSTOM_OUTPUT_INPUT_PWM = 0x10,//xxx
    CHARGING_STATE_0 = 0x11,//xxx
    CHARGING_STATE_1 = 0x12,//xxx
    CONFIRM_CANCEL_CHARGE = 0x13,
    POST_CHARGE_STATS = 0x14, //xxx
    ERROR_OCCURRED = 0x15,//xxx
    START_CHARGE_CELLS = 0x16, //xxx
    START_CHARGE_CAPACITY = 0x17,//xxx Nao da para baterias de > 10_000
}LcdStates;

#define LCD_INPUT_LENGHT 5
#define LCD_INPUT_FIRST 3
static char lcd_input[LCD_INPUT_LENGHT+1] = {"00000"};
static int8_t lcd_input_pivot = 0;
static bool lcd_edit = false;
static unsigned int current_lcd = FIRST_LCD;
void ChangeState(LcdStates state);
void ScreenScrollUpate(int first, const int last, char (*str)[last][16])
{
    TM_HD44780_Clear();
    TM_HD44780_Puts(1, 0, (*str)[first]);
    if(++first >= last)
        first = 0;
    TM_HD44780_Puts(1, 1, (*str)[first]);
    TM_HD44780_PutCustom(0, 1, 0xAF);
    TM_HD44780_PutCustom(0, 0, 0x7E);
}
// <editor-fold defaultstate="collapsed" desc="Diagnosis callbacks">
void OnDiagnosis(void) {
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Diagnosis       Get battery info");
}

void OnDiagnosisTick(void) {
    0;
}

void DiagnosisOk(void) {
    ChangeState(DIAGNOSIS_DETAIL);
}

void DiagnosisCancel(void) {
    0;
}

void DiagnosisMinus(void) {
    ChangeState(LAST_LCD);
}

void DiagnosisPlus(void) {
    ChangeState(++current_lcd);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="StartCharge callbacks">

void OnStartCharge(void) {
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Charge          Start new charge");
}

void OnStartChargeTick(void) {
    0;
}

void StartChargeOk(void) {
    ChangeState(START_CHARGE_CHOOSE_TYPE);
}

void StartChargeCancel(void) {
    0;
}

void StartChargeMinus(void) {
    ChangeState(DIAGNOSIS);
}

void StartChargePlus(void) {
    ChangeState(START_DISCHARGE);
}// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="StartDischarge callbacks">

void OnStartDischarge(void) {
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Discharge       Start discharge ");
}

void OnStartDischargeTick(void) {
    0;
}

void StartDischargeOk(void) {
    ChangeState(START_DISCHARGE_INPUT);
}

void StartDischargeCancel(void) {
    0;
}

void StartDischargeMinus(void) {
    ChangeState(START_CHARGE);
}

void StartDischargePlus(void) {
    ChangeState(SENSORS);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Sensors callbacks">

void OnSensors(void) {
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Sensors         Get sensor state");
}

void OnSensorsTick(void) {
    0;
}

void SensorsOk(void) {
    ChangeState(SENSORS_IOUT);
}

void SensorsCancel(void) {
    0;
}

void SensorsMinus(void) {
    ChangeState(START_DISCHARGE);
}

void SensorsPlus(void) {
    ChangeState(CUSTOM_OUTPUT);
}// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="CustomOutput callbaks">

void OnCustomOutput(void) {
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Custom output   Force output    ");
}

void OnCustomOutputTick(void) {
    0;
}

void CustomOutputOk(void) {
    ChangeState(CUSTOM_OUTPUT_CHOOSER);
}

void CustomOutputCancel(void) {
    0;
}

void CustomOutputMinus(void) {
    ChangeState(SENSORS);
}

void CustomOutputPlus(void) {
    ChangeState(DIAGNOSIS);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SensorsIOUT callbacks">

void OnSensorsIOUTTick(void) {
    static char str[33];
    sprintf(str, "IOUT            IOUT: %1.3fA", slc_CurrentValue());
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, str);
}

void OnSensorsIOUT(void) {
    OnSensorsIOUTTick();
}

void SensorsIOUTOk(void) {
    0;
}

void SensorsIOUTCancel(void) {
    ChangeState(SENSORS);
}

void SensorsIOUTMinus(void) {
    ChangeState(SENSORS_TEMP_EXT);
}

void SensorsIOUTPlus(void) {
    ChangeState(SENSORS_VOUT);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SensorsVOUT callbacks">

void OnSensorsVOUTTick(void) {
    static char str[33];
    sprintf(str, "VOUT            VOUT: %2.3fV", slc_VredValue());
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, str);
}

void OnSensorsVOUT(void) {
    OnSensorsVOUTTick();
}

void SensorsVOUTOk(void) {
    0;
}

void SensorsVOUTCancel(void) {
    ChangeState(SENSORS);
}

void SensorsVOUTMinus(void) {
    ChangeState(SENSORS_IOUT);
}

void SensorsVOUTPlus(void) {
    ChangeState(SENSORS_BATT);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SensorsBATT callbacks">

void OnSensorsBATTTick(void) {
    static char str[33];
    sprintf(str, "BATT            BATT: %2.3fV", slc_BattValue());
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, str);
}

void OnSensorsBATT(void) {
    OnSensorsBATTTick();
}

void SensorsBATTOk(void) {
    0;
}

void SensorsBATTCancel(void) {
    ChangeState(SENSORS);
}

void SensorsBATTMinus(void) {
    ChangeState(SENSORS_VOUT);
}

void SensorsBATTPlus(void) {
    ChangeState(SENSORS_TEMP_INT);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SensorsTEMPINT callbacks">

void OnSensorsTEMPINTTick(void) {
    static char str[33];
    sprintf(str, "TEMPINT         TEMP: %3.1f\xdf""C", slc_TempIntValue());
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, str);
}

void OnSensorsTEMPINT(void) {
    OnSensorsTEMPINTTick();
}

void SensorsTEMPINTOk(void) {
    0;
}

void SensorsTEMPINTCancel(void) {
    ChangeState(SENSORS);
}

void SensorsTEMPINTMinus(void) {
    ChangeState(SENSORS_BATT);
}

void SensorsTEMPINTPlus(void) {
    ChangeState(SENSORS_TEMP_EXT);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SensorsTEMPEXT callbacks">

void OnSensorsTEMPEXTTick(void) {
    static char str[33];
    sprintf(str, "TEMPEXT         TEMP: %3.1f\xdf""C", slc_TempBattValue());
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, str);
}

void OnSensorsTEMPEXT(void) {
    OnSensorsTEMPEXTTick();
}

void SensorsTEMPEXTOk(void) {
    0;
}

void SensorsTEMPEXTCancel(void) {
    ChangeState(SENSORS);
}

void SensorsTEMPEXTMinus(void) {
    ChangeState(SENSORS_TEMP_INT);
}

void SensorsTEMPEXTPlus(void) {
    ChangeState(SENSORS_IOUT);
}// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DiagnosisDetail callbaks">

void OnDiagnosisDetail(void) {
    static char str[33];
    sprintf(str, "Battery         %02.1fV  -  %3.1f%%", slc_BattValue(), getActualVoltagePercentage(slc_BattValue()));
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, str);
}

void OnDiagnosisDetailTick(void) {
    0;
}

void DiagnosisDetailOk(void) {
    ChangeState(DIAGNOSIS);
}

void DiagnosisDetailCancel(void) {
    ChangeState(DIAGNOSIS);
}

void DiagnosisDetailMinus(void) {
    0;
    ;
}

void DiagnosisDetailPlus(void) {
    0;
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="StartChargeChooseType callbaks">

void OnStartChargeChooseType(void) {
    lcd_input_pivot = CHARGE_TYPE_FIRST;
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "New charge plan:");
    TM_HD44780_PutCustom(0, 1, 0x7F);
    TM_HD44780_PutCustom(15, 1, 0x7E);
    TM_HD44780_Puts(1, 1, getChargeTypeDesc(lcd_input_pivot));

}

void OnStartChargeChooseTypeTick(void) {
    0;
}

void StartChargeChooseTypeOk(void) {
    setType(lcd_input_pivot);
    ChangeState(START_CHARGE_CELLS);
}

void StartChargeChooseTypeCancel(void) {
    ChangeState(START_CHARGE);
}

void StartChargeChooseTypeMinus(void) {
    if (--lcd_input_pivot < 0)
        lcd_input_pivot = CHARGE_TYPE_LAST;
    TM_HD44780_Puts(1, 1, getChargeTypeDesc(lcd_input_pivot));
}

void StartChargeChooseTypePlus(void) {
    if (++lcd_input_pivot > CHARGE_TYPE_LAST)
        lcd_input_pivot = 0;
    TM_HD44780_Puts(1, 1, getChargeTypeDesc(lcd_input_pivot));
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="StartChargeConfirm callbaks">

void OnStartChargeConfirm(void) {
    static char str[33];
    sprintf(str, "%-16s%02.1fV - %06uC", getChargeTypeDesc(getType()), getBatteryVoltage()/1000.0, getCapacity());
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, str);
}

void OnStartChargeConfirmTick(void) {
    0;
}

void StartChargeConfirmOk(void) {
    startCharge();
    ChangeState(CHARGING_STATE_0);
}

void StartChargeConfirmCancel(void) {
    ChangeState(START_CHARGE);
}

void StartChargeConfirmMinus(void) {
    0;
}

void StartChargeConfirmPlus(void) {
    0;
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="StartDischargeInput callbaks">

void OnStartDischargeInput(void) {
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Max current:    I: 00000");
    lcd_input_pivot = 0;
    lcd_edit = false;
    //lcd_input = {'0', '0', '2', '0', '0'};
    TM_HD44780_PutCustom(LCD_INPUT_FIRST + LCD_INPUT_LENGHT, 1, 0x7E);
    TM_HD44780_CursorOn();
    TM_HD44780_BlinkOff();
    TM_HD44780_CursorSet(LCD_INPUT_FIRST, 1);
}

void OnStartDischargeInputTick(void) {
    0;
}

void StartDischargeInputOk(void) {
    if (lcd_input_pivot == LCD_INPUT_LENGHT) {
        // done
        TM_HD44780_CursorOff();
        0;
    } else if (lcd_edit) {
        TM_HD44780_CursorSet(LCD_INPUT_FIRST + ++lcd_input_pivot, 1);
        if (lcd_input_pivot == LCD_INPUT_LENGHT)
            lcd_edit = false;
    } else if (!lcd_edit) {
        TM_HD44780_CursorSet(LCD_INPUT_FIRST + lcd_input_pivot, 1);
        TM_HD44780_BlinkOn();
        lcd_edit = true;
    }
}

void StartDischargeInputCancel(void) {
    if (lcd_edit) {
        TM_HD44780_BlinkOff();
        lcd_edit = false;
    } else if (!lcd_edit) {
        TM_HD44780_CursorOff();
        ChangeState(START_DISCHARGE);
    }
}

void StartDischargeInputMinus(void) {
    if (lcd_edit) {
        if (--lcd_input[lcd_input_pivot] < 0x30)
            lcd_input[lcd_input_pivot] = 0x39;
        TM_HD44780_PutCustom(LCD_INPUT_FIRST + lcd_input_pivot, 1, lcd_input[lcd_input_pivot]);
    } else {
        if (--lcd_input_pivot < 0) {
            lcd_input_pivot = LCD_INPUT_LENGHT;
        }
    }
    TM_HD44780_CursorSet(LCD_INPUT_FIRST + lcd_input_pivot, 1);
}

void StartDischargeInputPlus(void) {
    if (lcd_edit) {
        if (++lcd_input[lcd_input_pivot] > 0x39)
            lcd_input[lcd_input_pivot] = 0x30;
        TM_HD44780_PutCustom(LCD_INPUT_FIRST + lcd_input_pivot, 1, lcd_input[lcd_input_pivot]);
    } else {
        if (++lcd_input_pivot > LCD_INPUT_LENGHT) {
            lcd_input_pivot = 0;
        }
    }
    TM_HD44780_CursorSet(LCD_INPUT_FIRST + lcd_input_pivot, 1);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="CustomOutputChooser callbaks">
#define options_count 4
static char custom_output_options[options_count][16] = {
    "Current",
    "Voltage",
    "Open Base PWM",
    "Open Fan PWM"
};
void OnCustomOutputChooser(void) {
    lcd_input_pivot = 0;
    TM_HD44780_BlinkOn();
    TM_HD44780_CursorSet(0, 0);
    ScreenScrollUpate( lcd_input_pivot, options_count, &custom_output_options);
}

void OnCustomOutputChooserTick(void) {
    0;
}

void CustomOutputChooserOk(void) {
    if(lcd_input_pivot > 1)
        ChangeState(CUSTOM_OUTPUT_INPUT_PWM);
    0;
}

void CustomOutputChooserCancel(void) {
    ChangeState(CUSTOM_OUTPUT);
}

void CustomOutputChooserMinus(void) {
    if(--lcd_input_pivot < 0)
        lcd_input_pivot = options_count-1;
    ScreenScrollUpate( lcd_input_pivot, options_count, &custom_output_options);
}

void CustomOutputChooserPlus(void) {
    if(++lcd_input_pivot >= options_count)
        lcd_input_pivot = 0;
    ScreenScrollUpate( lcd_input_pivot, options_count, &custom_output_options);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="CustomOutputChooserInputPWM callbaks">

void OnCustomOutputChooserInputPWM(void) {
    char str[33];
    sprintf(lcd_input, "%05u", getBasePWM());
    sprintf(str, "PWM        :     : %05u", getBasePWM());
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0,str );
    lcd_input_pivot = 0;
    lcd_edit = false;
    //lcd_input = {'0', '0', '2', '0', '0'};
    TM_HD44780_PutCustom(LCD_INPUT_FIRST + LCD_INPUT_LENGHT, 1, 0x7E);
    TM_HD44780_CursorOn();
    TM_HD44780_BlinkOff();
    TM_HD44780_CursorSet(LCD_INPUT_FIRST, 1);
}

void OnCustomOutputChooserInputPWMTick(void) {
    0;
}

void CustomOutputChooserInputPWMOk(void) {
    if (lcd_input_pivot == LCD_INPUT_LENGHT) {
        int v = atoi(lcd_input);
        setBasePWM(v);
        TM_HD44780_CursorOff();
        ChangeState(FIRST_LCD);
    } else if (lcd_edit) {
        TM_HD44780_CursorSet(LCD_INPUT_FIRST + ++lcd_input_pivot, 1);
        if (lcd_input_pivot == LCD_INPUT_LENGHT)
            lcd_edit = false;
    } else if (!lcd_edit) {
        TM_HD44780_CursorSet(LCD_INPUT_FIRST + lcd_input_pivot, 1);
        TM_HD44780_BlinkOn();
        lcd_edit = true;
    }
}

void CustomOutputChooserInputPWMCancel(void) {
    if (lcd_edit) {
        TM_HD44780_BlinkOff();
        lcd_edit = false;
    } else if (!lcd_edit) {
        TM_HD44780_CursorOff();
        ChangeState(CUSTOM_OUTPUT_CHOOSER);
    }
}

void CustomOutputChooserInputPWMMinus(void) {
    if (lcd_edit) {
        if (--lcd_input[lcd_input_pivot] < 0x30)
            lcd_input[lcd_input_pivot] = 0x39;
        TM_HD44780_PutCustom(LCD_INPUT_FIRST + lcd_input_pivot, 1, lcd_input[lcd_input_pivot]);
    } else {
        if (--lcd_input_pivot < 0) {
            lcd_input_pivot = LCD_INPUT_LENGHT;
        }
    }
    TM_HD44780_CursorSet(LCD_INPUT_FIRST + lcd_input_pivot, 1);
}

void CustomOutputChooserInputPWMPlus(void) {
    if (lcd_edit) {
        if (++lcd_input[lcd_input_pivot] > 0x39)
            lcd_input[lcd_input_pivot] = 0x30;
        TM_HD44780_PutCustom(LCD_INPUT_FIRST + lcd_input_pivot, 1, lcd_input[lcd_input_pivot]);
    } else {
        if (++lcd_input_pivot > LCD_INPUT_LENGHT) {
            lcd_input_pivot = 0;
        }
    }
    TM_HD44780_CursorSet(LCD_INPUT_FIRST + lcd_input_pivot, 1);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="ChargingState0 callbacks">

void OnChargingState0(void) {
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Charging 0");
}

void OnChargingState0Tick(void) {
    if(hasErrors())
        ChangeState(ERROR_OCCURRED);
    if(isFinished())
        ChangeState(POST_CHARGE_STATS);
}

void ChargingState0Ok(void) {
    0;
}

void ChargingState0Cancel(void) {
    ChangeState(CONFIRM_CANCEL_CHARGE);
}

void ChargingState0Minus(void) {
    0;
}

void ChargingState0Plus(void) {
    0;
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="ChargingState1 callbacks">

void OnChargingState1(void) {
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Charging 1");
}

void OnChargingState1Tick(void) {
    if(hasErrors())
        ChangeState(ERROR_OCCURRED);
    if(isFinished())
        ChangeState(POST_CHARGE_STATS);
}

void ChargingState1Ok(void) {
    ChangeState(CHARGING_STATE_0);;
}

void ChargingState1Cancel(void) {
    ChangeState(CONFIRM_CANCEL_CHARGE);
}

void ChargingState1Minus(void) {
    ChangeState(CHARGING_STATE_0);
}

void ChargingState1Plus(void) {
    ChangeState(CHARGING_STATE_0);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="ConfirmCancelCharge callbacks">

void OnConfirmCancelCharge(void) {
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Are you sure you want to cancel?");
}

void OnConfirmCancelChargeTick(void) {
}

void ConfirmCancelChargeOk(void) {
    stopCharge();
    ChangeState(POST_CHARGE_STATS);
}

void ConfirmCancelChargeCancel(void) {
    ChangeState(CHARGING_STATE_0);
}

void ConfirmCancelChargeMinus(void) {
    0;
}

void ConfirmCancelChargePlus(void) {
    0;
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="PostChargeStatus callbacks">

void OnPostChargeStatus(void) {
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "FIM Status here");
}

void OnPostChargeStatusTick(void) {
}

void PostChargeStatusOk(void) {
    ChangeState(FIRST_LCD);
}

void PostChargeStatusCancel(void) {
    ChangeState(FIRST_LCD);
}

void PostChargeStatusMinus(void) {
    0;
}

void PostChargeStatusPlus(void) {
    0;
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="ChargingState1 callbacks">

void OnErrorOccurred(void) {
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "ERROR OCCURRED!!");
}

void OnErrorOccurredTick(void) {
}

void ErrorOccurredOk(void) {
    ChangeState(FIRST_LCD);
}

void ErrorOccurredCancel(void) {
    ChangeState(FIRST_LCD);
}

void ErrorOccurredMinus(void) {
    0;
}

void ErrorOccurredPlus(void) {
    0;
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="StartChargeCells callbaks">

void OnStartChargeCells(void) {
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Cell Number:    C: 00000");
    lcd_input_pivot = 0;
    lcd_edit = false;
    //lcd_input = {'0', '0', '2', '0', '0'};
    TM_HD44780_PutCustom(LCD_INPUT_FIRST + LCD_INPUT_LENGHT, 1, 0x7E);
    TM_HD44780_CursorOn();
    TM_HD44780_BlinkOff();
    TM_HD44780_CursorSet(LCD_INPUT_FIRST, 1);
}

void OnStartChargeCellsTick(void) {
    0;
}

void StartChargeCellsOk(void) {
    if (lcd_input_pivot == LCD_INPUT_LENGHT) {
        // done
        TM_HD44780_CursorOff();
        setBatteryCells(atoi(lcd_input));
        ChangeState(START_CHARGE_CAPACITY);
    } else if (lcd_edit) {
        TM_HD44780_CursorSet(LCD_INPUT_FIRST + ++lcd_input_pivot, 1);
        if (lcd_input_pivot == LCD_INPUT_LENGHT)
            lcd_edit = false;
    } else if (!lcd_edit) {
        TM_HD44780_CursorSet(LCD_INPUT_FIRST + lcd_input_pivot, 1);
        TM_HD44780_BlinkOn();
        lcd_edit = true;
    }
}

void StartChargeCellsCancel(void) {
    if (lcd_edit) {
        TM_HD44780_BlinkOff();
        lcd_edit = false;
    } else if (!lcd_edit) {
        TM_HD44780_CursorOff();
        ChangeState(START_CHARGE_CHOOSE_TYPE);
    }
}

void StartChargeCellsMinus(void) {
    if (lcd_edit) {
        if (--lcd_input[lcd_input_pivot] < 0x30)
            lcd_input[lcd_input_pivot] = 0x39;
        TM_HD44780_PutCustom(LCD_INPUT_FIRST + lcd_input_pivot, 1, lcd_input[lcd_input_pivot]);
    } else {
        if (--lcd_input_pivot < 0) {
            lcd_input_pivot = LCD_INPUT_LENGHT;
        }
    }
    TM_HD44780_CursorSet(LCD_INPUT_FIRST + lcd_input_pivot, 1);
}

void StartChargeCellsPlus(void) {
    if (lcd_edit) {
        if (++lcd_input[lcd_input_pivot] > 0x39)
            lcd_input[lcd_input_pivot] = 0x30;
        TM_HD44780_PutCustom(LCD_INPUT_FIRST + lcd_input_pivot, 1, lcd_input[lcd_input_pivot]);
    } else {
        if (++lcd_input_pivot > LCD_INPUT_LENGHT) {
            lcd_input_pivot = 0;
        }
    }
    TM_HD44780_CursorSet(LCD_INPUT_FIRST + lcd_input_pivot, 1);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="StartChargeCapacity callbaks">

void OnStartChargeCapacity(void) {
    TM_HD44780_Clear();
    TM_HD44780_Puts(0, 0, "Batt capacity:  C: 00000");
    lcd_input_pivot = 0;
    lcd_edit = false;
    //lcd_input = {'0', '0', '2', '0', '0'};
    TM_HD44780_PutCustom(LCD_INPUT_FIRST + LCD_INPUT_LENGHT, 1, 0x7E);
    TM_HD44780_CursorOn();    
    TM_HD44780_BlinkOff();
    TM_HD44780_CursorSet(LCD_INPUT_FIRST, 1);
}

void OnStartChargeCapacityTick(void) {
    0;
}

void StartChargeCapacityOk(void) {
    if (lcd_input_pivot == LCD_INPUT_LENGHT) {
        // done
        TM_HD44780_CursorOff();
        setCapacity(atoi(lcd_input));
        ChangeState(START_CHARGE_CONFIRM);
    } else if (lcd_edit) {
        TM_HD44780_CursorSet(LCD_INPUT_FIRST + ++lcd_input_pivot, 1);
        if (lcd_input_pivot == LCD_INPUT_LENGHT)
            lcd_edit = false;
    } else if (!lcd_edit) {
        TM_HD44780_CursorSet(LCD_INPUT_FIRST + lcd_input_pivot, 1);
        TM_HD44780_BlinkOn();
        lcd_edit = true;
    }
}

void StartChargeCapacityCancel(void) {
    if (lcd_edit) {
        TM_HD44780_BlinkOff();
        lcd_edit = false;
    } else if (!lcd_edit) {
        TM_HD44780_CursorOff();
        ChangeState(START_CHARGE_CELLS);
    }
}

void StartChargeCapacityMinus(void) {
    if (lcd_edit) {
        if (--lcd_input[lcd_input_pivot] < 0x30)
            lcd_input[lcd_input_pivot] = 0x39;
        TM_HD44780_PutCustom(LCD_INPUT_FIRST + lcd_input_pivot, 1, lcd_input[lcd_input_pivot]);
    } else {
        if (--lcd_input_pivot < 0) {
            lcd_input_pivot = LCD_INPUT_LENGHT;
        }
    }
    TM_HD44780_CursorSet(LCD_INPUT_FIRST + lcd_input_pivot, 1);
}

void StartChargeCapacityPlus(void) {
    if (lcd_edit) {
        if (++lcd_input[lcd_input_pivot] > 0x39)
            lcd_input[lcd_input_pivot] = 0x30;
        TM_HD44780_PutCustom(LCD_INPUT_FIRST + lcd_input_pivot, 1, lcd_input[lcd_input_pivot]);
    } else {
        if (++lcd_input_pivot > LCD_INPUT_LENGHT) {
            lcd_input_pivot = 0;
        }
    }
    TM_HD44780_CursorSet(LCD_INPUT_FIRST + lcd_input_pivot, 1);
}
// </editor-fold>
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
      &OnCustomOutputChooserInputPWM,
      &OnCustomOutputChooserInputPWMTick,
      &CustomOutputChooserInputPWMMinus,
      &CustomOutputChooserInputPWMPlus,
      &CustomOutputChooserInputPWMCancel,
      &CustomOutputChooserInputPWMOk,
    },
    {
      &OnChargingState0,
      &OnChargingState0Tick,
      &ChargingState0Minus,
      &ChargingState0Plus,
      &ChargingState0Cancel,
      &ChargingState0Ok,
    },
    {
      &OnChargingState1,
      &OnChargingState1Tick,
      &ChargingState1Minus,
      &ChargingState1Plus,
      &ChargingState1Cancel,
      &ChargingState1Ok,
    },
    {
      &OnConfirmCancelCharge,
      &OnConfirmCancelChargeTick,
      &ConfirmCancelChargeMinus,
      &ConfirmCancelChargePlus,
      &ConfirmCancelChargeCancel,
      &ConfirmCancelChargeOk,
    },
    {
      &OnPostChargeStatus,
      &OnPostChargeStatusTick,
      &PostChargeStatusMinus,
      &PostChargeStatusPlus,
      &PostChargeStatusCancel,
      &PostChargeStatusOk,
    },
    {
      &OnErrorOccurred,
      &OnErrorOccurredTick,
      &ErrorOccurredMinus,
      &ErrorOccurredPlus,
      &ErrorOccurredCancel,
      &ErrorOccurredOk,
    },
    {
      &OnStartChargeCells,
      &OnStartChargeCellsTick,
      &StartChargeCellsMinus,
      &StartChargeCellsPlus,
      &StartChargeCellsCancel,
      &StartChargeCellsOk,
    },
    {
      &OnStartChargeCapacity,
      &OnStartChargeCapacityTick,
      &StartChargeCapacityMinus,
      &StartChargeCapacityPlus,
      &StartChargeCapacityCancel,
      &StartChargeCapacityOk,
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
static uint32_t tick_counting = 0;
void __attribute__( (interrupt(IPL2AUTO), vector(_TIMER_5_VECTOR))) isr_buttons(void)
{
    if(CANCEL_BTN)
        (*lcd_screens[current_lcd].cancel)();
    else if(OK_BTN)
        (*lcd_screens[current_lcd].ok)();
    else if(MINUS_BTN)
        (*lcd_screens[current_lcd].minus)();
    else if(PLUS_BTN)
        (*lcd_screens[current_lcd].plus)();
    
    (*lcd_screens[current_lcd].on_tick)(); 
    tick_counting++;
	IFS0bits.T5IF = 0;
}

uint32_t getCurrentTick(void)
{
    return tick_counting;
}