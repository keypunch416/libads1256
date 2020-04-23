#ifndef DAC8532_H
#define DAC8532_H

#include "DEV_Config.h"

#define DAC8532_CHANNEL_A (0x30)
#define DAC8532_CHANNEL_B (0x34)
#define DAC8532_CHANNEL_A_STR "A"
#define DAC8532_CHANNEL_B_STR "B"


#define DAC8532_REG_VALUE_MAX (0xFFFF)
#define DAC8532_VREF (5.0)

void DAC8532_Set_Voltage(uint8_t channel, double voltage);

#endif /* DAC8532_H */