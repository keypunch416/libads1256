#include "ADS1256.h"
#include "debug.h"

uint8_t ScanMode = 0;

static void ADS1256_reset(void)
{
    DEV_Digital_Write(DEV_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(DEV_RST_PIN, 0);
    DEV_Delay_ms(200);
    DEV_Digital_Write(DEV_RST_PIN, 1);
}

static void ADS1256_WriteCmd(uint8_t Cmd)
{
    DEV_Digital_Write(DEV_CS_PIN, 0);
    DEV_SPI_WriteByte(Cmd);
    DEV_Digital_Write(DEV_CS_PIN, 1);
}

static void ADS1256_WriteReg(uint8_t Reg, uint8_t data)
{
    DEV_Digital_Write(DEV_CS_PIN, 0);
    DEV_SPI_WriteByte(CMD_WREG | Reg);
    DEV_SPI_WriteByte(0x00);
    DEV_SPI_WriteByte(data);
    DEV_Digital_Write(DEV_CS_PIN, 1);
}

static uint8_t ADS1256_Read_data(uint8_t Reg)
{
    uint8_t temp = 0;
    DEV_Digital_Write(DEV_CS_PIN, 0);
    DEV_SPI_WriteByte(CMD_RREG | Reg);
    DEV_SPI_WriteByte(0x00);
    DEV_Delay_ms(1);
    temp = DEV_SPI_ReadByte();
    DEV_Digital_Write(DEV_CS_PIN, 1);
    return temp;
}

static void ADS1256_WaitDRDY()
{
    for (uint32_t i = 0; i < 4000000; i++)
    {
        if (!DEV_Digital_Read(DEV_DRDY_PIN))
            return;
    }

    ADS1256_WARN("time out");
}

uint8_t ADS1256_ReadChipID()
{
    ADS1256_WaitDRDY();
    return ADS1256_Read_data(REG_STATUS) >> 4;
}

void ADS1256_ConfigADC(ADS1256_gain_e gain, ADS1256_drate_e drate)
{
    ADS1256_WaitDRDY();
    uint8_t buf[4] = {0};
    buf[0] = (0 << 3) | (1 << 2) | (0 << 1);
    buf[1] = 0x08;
    buf[2] = (0 << 5) | (0 << 3) | (gain << 0);
    buf[3] = ADS1256_drate_bits[drate];
    DEV_Digital_Write(DEV_CS_PIN, 0);
    DEV_SPI_WriteByte(CMD_WREG | 0);
    DEV_SPI_WriteByte(0x03);

    DEV_SPI_WriteByte(buf[0]);
    DEV_SPI_WriteByte(buf[1]);
    DEV_SPI_WriteByte(buf[2]);
    DEV_SPI_WriteByte(buf[3]);
    DEV_Digital_Write(DEV_CS_PIN, 1);
    DEV_Delay_ms(1);
}

static void ADS1256_SetChannel(ADS1256_channel_e channel)
{
    if (ADS1256_CHANNEL_MAX < channel)
    {
        ADS1256_WARN("invalid channel %u", channel);
        return ;
    }
    ADS1256_WriteReg(REG_MUX, (channel << 4) | (1 << 3));
    ADS1256_DBG("set channel %u", channel);
}

void ADS1256_SetDiffChannal(ADS1256_diff_channel_e diff_channel)
{
    if (ADS1256_DIFF_CHANNEL_MAX < diff_channel)
    {
        ADS1256_WARN("invalid diff channel %u", diff_channel);
        return ;
    }

    ADS1256_WriteReg(REG_MUX, ((diff_channel << 1) << 4) | 1);
    ADS1256_DBG("set channel %u", diff_channel);
}

void ADS1256_SetMode(ADS1256_input_mode_e mode)
{
    if (ADS1256_MODE_SINGLE_END_INPUT == mode)
    {
        ScanMode = ADS1256_MODE_SINGLE_END_INPUT;
        ADS1256_DBG("single-ended input was set");
    }
    else if (ADS1256_MODE_DIFF_INPUT == mode)
    {
        ScanMode = ADS1256_MODE_DIFF_INPUT;
        ADS1256_DBG("differential input was set");
    }
    else
    {
        ADS1256_ERR("undefined input mode, ignore");
    }
}

uint8_t ADS1256_init(void)
{
    ADS1256_reset();

    if(3 == ADS1256_ReadChipID())
    {
        ADS1256_DBG("ID read success");
    }
    else
    {
        ADS1256_ERR("ID read failed");
        return 1;
    }

    ADS1256_ConfigADC(ADS1256_GAIN_1, ADS1256_30000SPS);
    // DEV_Delay_us(30);
    ADS1256_WriteCmd(CMD_SELFCAL);
    DEV_Delay_us(5);
    ADS1256_WaitDRDY();
    ADS1256_WriteCmd(CMD_RDATAC);
    // ADS1256_WaitDRDY();

    // printf("ADD 0x02  = 0x%x\r\n",ADS1256_Read_data(0x02));
    // printf("ADD 0x03  = 0x%x\r\n",ADS1256_Read_data(0x03));
    return 0;
}

static uint32_t ADS1256_Read_ADC_Data(void)
{
    uint32_t read = 0;
    uint8_t buf[3] = {0,0,0};

    ADS1256_WaitDRDY();
    DEV_Delay_us(10);
    DEV_Digital_Write(DEV_CS_PIN, 0);
    DEV_SPI_WriteByte(CMD_RDATA);
    DEV_Delay_us(10);
    buf[0] = DEV_SPI_ReadByte();
    buf[1] = DEV_SPI_ReadByte();
    buf[2] = DEV_SPI_ReadByte();
    DEV_Digital_Write(DEV_CS_PIN, 1);
    read = ((uint32_t)buf[0] << 16) & 0x00FF0000;
    read |= ((uint32_t)buf[1] << 8);  /* Pay attention to It is wrong   read |= (buf[1] << 8) */
    read |= buf[2];
    //printf("%d  %d  %d \r\n",buf[0],buf[1],buf[2]);
    if (read & 0x800000)
        read &= 0xFF000000;
    return read;
}

uint32_t ADS1256_GetValueRDATAC()
{
    uint32_t read = 0;
    uint8_t buf[3] = {0,0,0};

    ADS1256_WaitDRDY();
    DEV_Delay_us(3); // от этих задержек прямо зависит будет ли ~30к попугаев
    DEV_Digital_Write(DEV_CS_PIN, 0);
    DEV_Delay_us(5); // от этих задержек прямо зависит будет ли ~30к попугаев
    buf[0] = DEV_SPI_ReadByte();
    buf[1] = DEV_SPI_ReadByte();
    buf[2] = DEV_SPI_ReadByte();
    DEV_Digital_Write(DEV_CS_PIN, 1);
    read = ((uint32_t)buf[0] << 16) & 0x00FF0000;
    read |= ((uint32_t)buf[1] << 8);  /* Pay attention to It is wrong   read |= (buf[1] << 8) */
    read |= buf[2];
    //printf("%d  %d  %d \r\n",buf[0],buf[1],buf[2]);
    if (read & 0x800000)
        read &= 0xFF000000;
    return read;
}

uint32_t ADS1256_GetChannalValue(ADS1256_channel_e channel)
{
    if (ADS1256_MODE_SINGLE_END_INPUT != ScanMode)
    {
        ADS1256_WARN("cannot read single-ended input channel in differencial input mode");
        return 0;
    }
    if (ADS1256_CHANNEL_MAX <= channel)
    {
        ADS1256_WARN("invalid channel %u", channel);
        return 0;
    }

    while (DEV_Digital_Read(DEV_DRDY_PIN));

    ADS1256_SetChannel(channel);
    ADS1256_WriteCmd(CMD_SYNC);
    ADS1256_WriteCmd(CMD_WAKEUP);

    return ADS1256_Read_ADC_Data();
}

uint32_t ADS1256_GetDiffChannalValue(ADS1256_diff_channel_e diff_channel)
{
    if (ADS1256_MODE_SINGLE_END_INPUT != ScanMode)
    {
        ADS1256_WARN("cannot read differencial input channel in single-ended input mode");
        return 0;
    }
    if (ADS1256_DIFF_CHANNEL_MAX <= diff_channel)
    {
        ADS1256_WARN("invalid diff channel %u", diff_channel);
        return 0;
    }

    while (DEV_Digital_Read(DEV_DRDY_PIN));

    ADS1256_SetDiffChannal(diff_channel);
    ADS1256_WriteCmd(CMD_SYNC);
    ADS1256_WriteCmd(CMD_WAKEUP);

    return ADS1256_Read_ADC_Data();
}

void ADS1256_GetAll(uint32_t * values)
{
    if (ADS1256_MODE_SINGLE_END_INPUT == ScanMode)
    {
        for (uint8_t i = 0; i < ADS1256_CHANNEL_MAX; i++)
        {
            values[i] = ADS1256_GetChannalValue(i);
        }
    }
    else if (ADS1256_MODE_DIFF_INPUT == ScanMode)
    {
        for (uint8_t i = 0; i < ADS1256_DIFF_CHANNEL_MAX; i++)
        {
            values[i] = ADS1256_GetDiffChannalValue(i);
        }
    }
}