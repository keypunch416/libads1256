#ifndef ADS1256_H
#define ADS1256_H

#include "DEV_Config.h"

#define ADS1256_MAX_VAL     (5.0)
#define ADS1256_MAX_BIT_VAL (0x7fffff)

typedef enum
{
    ADS1256_MODE_SINGLE_END_INPUT = 0,
    ADS1256_MODE_DIFF_INPUT
} ADS1256_input_mode_e;

typedef enum
{
    ADS1256_CHANNEL_0 = 0,
    ADS1256_CHANNEL_1,
    ADS1256_CHANNEL_2,
    ADS1256_CHANNEL_3,
    ADS1256_CHANNEL_4,
    ADS1256_CHANNEL_5,
    ADS1256_CHANNEL_6,
    ADS1256_CHANNEL_7,
    ADS1256_CHANNEL_MAX = ADS1256_CHANNEL_7
} ADS1256_channel_e;

typedef enum
{
    ADS1256_DIFF_CHANNEL_0 = 0, // AIN0-AIN1
    ADS1256_DIFF_CHANNEL_1,     // AIN2-AIN3
    ADS1256_DIFF_CHANNEL_2,     // AIN4-AIN5
    ADS1256_DIFF_CHANNEL_3,     // AIN6-AIN7
    ADS1256_DIFF_CHANNEL_MAX = ADS1256_DIFF_CHANNEL_3
} ADS1256_diff_channel_e;

typedef enum
{
    ADS1256_GAIN_1 = 0,
    ADS1256_GAIN_2,
    ADS1256_GAIN_4,
    ADS1256_GAIN_8,
    ADS1256_GAIN_16,
    ADS1256_GAIN_32,
    ADS1256_GAIN_64
} ADS1256_gain_e;

typedef enum
{
    ADS1256_30000SPS = 0,
    ADS1256_15000SPS,
    ADS1256_7500SPS,
    ADS1256_3750SPS,
    ADS1256_2000SPS,
    ADS1256_1000SPS,
    ADS1256_500SPS,
    ADS1256_100SPS,
    ADS1256_60SPS,
    ADS1256_50SPS,
    ADS1256_30SPS,
    ADS1256_25SPS,
    ADS1256_15SPS,
    ADS1256_10SPS,
    ADS1256_5SPS,
    ADS1256_2d5SPS,

    ADS1256_DRATE_MAX
} ADS1256_drate_e;

typedef enum
{
    /*Register address, followed by reset the default values */
    REG_STATUS = 0,  // x1H
    REG_MUX    = 1,  // 01H
    REG_ADCON  = 2,  // 20H
    REG_DRATE  = 3,  // F0H
    REG_IO     = 4,  // E0H
    REG_OFC0   = 5,  // xxH
    REG_OFC1   = 6,  // xxH
    REG_OFC2   = 7,  // xxH
    REG_FSC0   = 8,  // xxH
    REG_FSC1   = 9,  // xxH
    REG_FSC2   = 10, // xxH
} ADS1256_reg_e;


typedef enum
{
    CMD_WAKEUP  = 0x00, // Completes SYNC and Exits Standby Mode 0000  0000 (00h)
    CMD_RDATA   = 0x01, // Read Data 0000  0001 (01h)
    CMD_RDATAC  = 0x03, // Read Data Continuously 0000   0011 (03h)
    CMD_SDATAC  = 0x0F, // Stop Read Data Continuously 0000   1111 (0Fh)
    CMD_RREG    = 0x10, // Read from REG rrr 0001 rrrr (1xh)
    CMD_WREG    = 0x50, // Write to REG rrr 0101 rrrr (5xh)
    CMD_SELFCAL = 0xF0, // Offset and Gain Self-Calibration 1111    0000 (F0h)
    CMD_SELFOCAL= 0xF1, // Offset Self-Calibration 1111    0001 (F1h)
    CMD_SELFGCAL= 0xF2, // Gain Self-Calibration 1111    0010 (F2h)
    CMD_SYSOCAL = 0xF3, // System Offset Calibration 1111   0011 (F3h)
    CMD_SYSGCAL = 0xF4, // System Gain Calibration 1111    0100 (F4h)
    CMD_SYNC    = 0xFC, // Synchronize the A/D Conversion 1111   1100 (FCh)
    CMD_STANDBY = 0xFD, // Begin Standby Mode 1111   1101 (FDh)
    CMD_RESET   = 0xFE, // Reset to Power-Up Values 1111   1110 (FEh)
} ADS1256_cmd_e;

static const uint8_t ADS1256_drate_bits[ADS1256_DRATE_MAX] =
{
    0xF0,       /*reset the default values  */
    0xE0,
    0xD0,
    0xC0,
    0xB0,
    0xA1,
    0x92,
    0x82,
    0x72,
    0x63,
    0x53,
    0x43,
    0x33,
    0x20,
    0x13,
    0x03
};

uint8_t ADS1256_init(void);
void ADS1256_SetMode(ADS1256_input_mode_e mode);
void ADS1256_ConfigADC(ADS1256_gain_e gain, ADS1256_drate_e drate);
uint32_t ADS1256_GetChannalValue(ADS1256_channel_e channel);
uint32_t ADS1256_GetValueRDATAC(void);
void ADS1256_GetAll(uint32_t * values);

#endif /* ADS1256_H */