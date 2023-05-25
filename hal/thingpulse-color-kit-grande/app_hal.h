#ifndef DRIVER_H
#define DRIVER_H

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <FT6236.h>


void hal_setup(void);
void hal_loop(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DRIVER_H*/
