
// Based on the driver provided by adafruit for the arduino IDE found at 
// https://github.com/adafruit/Adafruit_TCS34725

#ifndef TCS34725_H
#define TCS34725_H

// C++ guard
#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include "tcs34725_defs.h"

// Initialize driver and check that device is attached
tcs34725_err_t tcs34725_init(tcs34725_config_t*);
// Wake device from sleep state
tcs34725_err_t tcs34725_enable(tcs34725_config_t*);
// Place device into sleep state
tcs34725_err_t tcs34725_disable(tcs34725_config_t*);

// Useful for changing config and simultaneously writing changes to device
tcs34725_err_t tcs34725_set_integration_time(tcs34725_integration_time_t, tcs34725_config_t*);
tcs34725_err_t tcs34725_set_gain(tcs34725_gain_t, tcs34725_config_t*);

// Get normalized RGB reading (returns 3xfloat from 0-255)
tcs34725_err_t tcs34725_get_normalized_RGB(tcs34725_normalized_color_t*, tcs34725_config_t*);
// Samples raw data
tcs34725_err_t tcs34725_get_raw_data(tcs34725_color_t*, tcs34725_config_t*);
// Wakes the device, samples raw data and sleeps the device
tcs34725_err_t tcs34725_get_raw_data_one_shot(tcs34725_color_t*, tcs34725_config_t*);

//Conversion helpers
uint16_t tcs34725_calculate_color_temperature(tcs34725_color_t);
uint16_t tcs34725_calculate_color_temperature_dn40(tcs34725_color_t, tcs34725_config_t*);
uint16_t tcs34725_calculate_lux(tcs34725_color_t);

// Configure interrupts
tcs34725_err_t tcs34725_set_interrupt(bool, tcs34725_config_t*);
tcs34725_err_t tcs34725_clear_interrupt(tcs34725_config_t*);
tcs34725_err_t tcs34725_set_int_limits(uint16_t l, uint16_t h, tcs34725_config_t*);


#ifdef __cplusplus
}
#endif // End of C++ guard

#endif