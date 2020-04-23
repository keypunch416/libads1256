#include "DAC8532.h"
#include "debug.h"

static void DAC8532_Write(uint8_t channel, uint16_t reg_val)
{
    DEV_Digital_Write(DEV_CS1_PIN, 1);
    DEV_Digital_Write(DEV_CS1_PIN, 0);
    DEV_SPI_WriteByte(channel);
    DEV_SPI_WriteByte((reg_val >> 8));
    DEV_SPI_WriteByte((reg_val & 0xff));
    DEV_Digital_Write(DEV_CS1_PIN, 1);
}

void DAC8532_Set_Voltage(uint8_t channel, double voltage)
{
    if (DAC8532_CHANNEL_A != channel &&
        DAC8532_CHANNEL_B != channel)
    {
        DAC8532_ERR("invalid channel");
        return;
    }

    if (DAC8532_VREF < voltage)
    {
        DAC8532_ERR("invalid voltage value");
        return;
    }

    uint16_t reg_val = (uint16_t) (voltage * DAC8532_REG_VALUE_MAX / DAC8532_VREF);
    DAC8532_Write(channel, reg_val);
    DAC8532_DBG("out voltage %f for channel %s was setted",
                 voltage, DAC8532_CHANNEL_A == channel ?
                 DAC8532_CHANNEL_A_STR:DAC8532_CHANNEL_B_STR);
}