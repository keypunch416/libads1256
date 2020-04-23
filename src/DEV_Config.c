#include "DEV_Config.h"
#include "debug.h"

static void DEV_GPIOConfig(void)
{
    /* ADS1256 */
    bcm2835_gpio_fsel(DEV_RST_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(DEV_CS0_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(DEV_DRDY_PIN,BCM2835_GPIO_FSEL_INPT);
    /* DAC8532 */
    bcm2835_gpio_fsel(DEV_CS1_PIN, BCM2835_GPIO_FSEL_OUTP);
}

uint8_t DEV_ModuleInit(void)
{
    if(!bcm2835_init())
    {
        DEV_ERR("init bcm2835 library failed");
        return 1;
    }

    DEV_GPIOConfig();

    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);

    DEV_DBG("bcm2835 init success");

    return 0;
}

void DEV_ModuleExit(void)
{
    DEV_DBG("bcm2835 deinit");
    bcm2835_spi_end();
    bcm2835_close();
}