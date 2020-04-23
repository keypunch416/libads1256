#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef ADS1256_DEBUG
    #define ADS1256_DBG(__info__, ...) { fprintf(stderr, "[ADS1256-DBG]: <%s> %d: " __info__, __FUNCTION__, __LINE__, ##__VA_ARGS__); fprintf(stderr, "\n"); }
#else
    #define ADS1256_DBG(__info__, ...)
#endif /* ADS1256_DEBUG */

#define ADS1256_WARN(__info__, ...) { fprintf(stderr, "[ADS1256-WARN]: <%s> %d: " __info__, __FUNCTION__, __LINE__, ##__VA_ARGS__); fprintf(stderr, "\n"); }
#define ADS1256_ERR(__info__, ...) { fprintf(stderr, "[ADS1256-ERR]: <%s> %d: " __info__, __FUNCTION__, __LINE__, ##__VA_ARGS__); fprintf(stderr, "\n"); }

#ifdef DAC8532_DEBUG
    #define DAC8532_DBG(__info__, ...) { fprintf(stderr, "[DAC8532-DBG]: <%s> %d: " __info__, __FUNCTION__, __LINE__, ##__VA_ARGS__); fprintf(stderr, "\n"); }
#else
    #define DAC8532_DBG(__info__, ...)
#endif /* DAC8532_DEBUG */

#define DAC8532_ERR(__info__, ...) { fprintf(stderr, "[DAC8532-ERR]: <%s> %d: " __info__, __FUNCTION__, __LINE__, ##__VA_ARGS__); fprintf(stderr, "\n"); }

#ifdef DEV_DEBUG
    #define DEV_DBG(__info__, ...) { fprintf(stderr, "[DEV-DBG]: <%s> %d: " __info__, __FUNCTION__, __LINE__, ##__VA_ARGS__); fprintf(stderr, "\n"); }
#else
    #define DEV_DBG(__info__, ...)
#endif

#define DEV_ERR(__info__, ...) { fprintf(stderr, "[DEV-ERR]: <%s> %d: " __info__, __FUNCTION__, __LINE__, ##__VA_ARGS__); fprintf(stderr, "\n"); }

#endif /* DEV_DEBUG */