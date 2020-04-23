#ifndef DEV_CONFIG_H
#define DEV_CONFIG_H

#include <bcm2835.h>
#include <stdint.h>
#include <stdio.h>

/* ADS1256 */
#define DEV_RST_PIN  (18)
#define DEV_CS0_PIN  (22)
#define DEV_DRDY_PIN (17)
/* DAC8532 */
#define DEV_CS1_PIN  (23)

#define DEV_Digital_Write(_pin, _value) bcm2835_gpio_write(_pin, _value)
#define DEV_Digital_Read(_pin) bcm2835_gpio_lev(_pin)

#define DEV_SPI_WriteByte(__value) bcm2835_spi_transfer(__value)
#define DEV_SPI_ReadByte() bcm2835_spi_transfer(0xff)

#define DEV_Delay_ms(__ms) bcm2835_delay(__ms)
#define DEV_Delay_us(__us) bcm2835_delayMicroseconds(__us)

uint8_t DEV_ModuleInit(void);
void DEV_ModuleExit(void);

#endif /* DEV_CONFIG_H */