#include <stdlib.h>
#include <signal.h>

#include "ADS1256.h"

#define SIZE (1024*8)

void sig_handler(int signo)
{
    printf("SIGINT received, exiting\n");
    DEV_ModuleExit();
    exit(0);
}

int main(void)
{
    printf("test\n");

    DEV_ModuleInit();

    signal(SIGINT, sig_handler);

    if (ADS1256_init())
    {
        printf("ads1256 init failed\n");
        
        DEV_ModuleExit();
        exit(0);
    }

    FILE * fileOrig = fopen("raw", "w");
    if (!fileOrig)
    {
        printf("fopen() failed\n");
        DEV_ModuleExit();
        return -1;
    }

    double val_orig[SIZE] = {};

    for (int i = 0; i < SIZE; ++i)
    {
        val_orig[i] = ADS1256_GetValueRDATAC()*5.0/0x7fffff;
    }

    DEV_ModuleExit();

    if (SIZE != fwrite(val_orig, sizeof(double), SIZE, fileOrig))
    {
        printf("invalid number blocks written\n");
    }
    else
    {
        printf("samples write success\n");
    }

    fclose(fileOrig);

    return 0;
}
