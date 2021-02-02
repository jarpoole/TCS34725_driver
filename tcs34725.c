
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "tcs34725.h"

//Internal helper functions for reading and writing registers
static tcs34725_err_t write8(uint8_t, uint32_t, tcs34725_config_t*);
static tcs34725_err_t read8(uint8_t, uint8_t*, tcs34725_config_t*);
static tcs34725_err_t read16(uint8_t, uint16_t*, tcs34725_config_t*);


static tcs34725_err_t write8(uint8_t reg, uint32_t value, tcs34725_config_t* config) {
    tcs34725_err_t err;
    uint8_t data = (uint8_t) value;
    
    if( config->write_reg(config->device_addr, reg, &data, 1) == 0){
        err = TCS34725_OK;
    }else{
        err = TCS34725_ERR_WRITE;
    }
    return err;
}


static tcs34725_err_t read8(uint8_t reg, uint8_t* data, tcs34725_config_t* config) {
    tcs34725_err_t err;
    if( config->read_reg(config->device_addr, reg, data, sizeof(uint8_t)) == 0 ){
        err = TCS34725_OK;
    }else{
        err = TCS34725_ERR_READ;
    }
    return err;
}


static tcs34725_err_t read16(uint8_t reg, uint16_t* data, tcs34725_config_t* config) {
    tcs34725_err_t err;
    if( config->read_reg(config->device_addr, reg, (uint8_t*) data, sizeof(uint16_t)) == 0 ){
        err = TCS34725_OK;
    }else{
        err = TCS34725_ERR_READ;
    }
    return err;

}


tcs34725_err_t tcs34725_enable(tcs34725_config_t* config) {
    tcs34725_err_t err = TCS34725_OK;

    err |= write8(TCS34725_ENABLE_REG, TCS34725_ENABLE_PON, config);
    config->delay_ms(3);
    err |= write8(TCS34725_ENABLE_REG, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN, config);
    /* Set a delay for the integration time.
       This is only necessary in the case where enabling and then
       immediately trying to read values back. This is because setting
       AEN triggers an automatic integration, so if a read RGBC is
       performed too quickly, the data is not yet valid and all 0's are
       returned */
    switch (config->settings.integration_time) {
    case TCS34725_INTEGRATIONTIME_2_4MS:
        config->delay_ms(3);
        break;
    case TCS34725_INTEGRATIONTIME_24MS:
        config->delay_ms(24);
        break;
    case TCS34725_INTEGRATIONTIME_50MS:
        config->delay_ms(50);
        break;
    case TCS34725_INTEGRATIONTIME_101MS:
        config->delay_ms(101);
        break;
    case TCS34725_INTEGRATIONTIME_154MS:
        config->delay_ms(154);
        break;
    case TCS34725_INTEGRATIONTIME_700MS:
        config->delay_ms(700);
        break;
    }
    return err;
}


tcs34725_err_t tcs34725_disable(tcs34725_config_t* config) {
    tcs34725_err_t err = TCS34725_OK;

    uint8_t reg_val = 0;
    err |= read8(TCS34725_ENABLE_REG, &reg_val, config);
    err |= write8(TCS34725_ENABLE_REG, reg_val & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN), config);
    return err;
}


tcs34725_err_t tcs34725_init(tcs34725_config_t* config) {
    tcs34725_err_t err = TCS34725_OK;

    // Make sure sensor is accessible
    uint8_t id = 0;
    err |= read8(TCS34725_ID_REG, &id, config);
    if(err != TCS34725_OK){
        //Do nothing, read error and err will already have the correct error code
    }else if( (id != TCS34725_ID) && (id != 0x10)) {
        err = TCS34725_ERR_DEVICE_NOT_FOUND;
    }else{
        // Set default integration time and gain
        err |= tcs34725_set_integration_time(config->settings.integration_time, config);
        err |= tcs34725_set_gain(config->settings.gain, config);

        // Note: by default, the device is in power down mode on bootup
        err |= tcs34725_enable(config);
    }
    return err;
}


tcs34725_err_t tcs34725_set_integration_time(tcs34725_integration_time_t it, tcs34725_config_t* config) {
    // Update the timing register
    return write8(TCS34725_ATIME_REG, it, config);
}


tcs34725_err_t tcs34725_set_gain(tcs34725_gain_t gain, tcs34725_config_t* config) {
    // Update the timing register
    return write8(TCS34725_CONTROL_REG, gain, config);
}


tcs34725_err_t tcs34725_get_raw_data(tcs34725_color_t* color, tcs34725_config_t* config) {

    tcs34725_err_t err = TCS34725_OK;

    err |= read16(TCS34725_CDATAL_REG, &(color->clear), config);
    err |= read16(TCS34725_RDATAL_REG, &(color->red),   config);
    err |= read16(TCS34725_GDATAL_REG, &(color->green), config);
    err |= read16(TCS34725_BDATAL_REG, &(color->blue),  config);

    // Set a delay for the integration time
    switch (config->settings.integration_time) {
    case TCS34725_INTEGRATIONTIME_2_4MS:
        config->delay_ms(3);
        break;
    case TCS34725_INTEGRATIONTIME_24MS:
        config->delay_ms(24);
        break;
    case TCS34725_INTEGRATIONTIME_50MS:
        config->delay_ms(50);
        break;
    case TCS34725_INTEGRATIONTIME_101MS:
        config->delay_ms(101);
        break;
    case TCS34725_INTEGRATIONTIME_154MS:
        config->delay_ms(154);
        break;
    case TCS34725_INTEGRATIONTIME_700MS:
        config->delay_ms(700);
        break;
    }
    return err;
}


tcs34725_err_t tcs34725_get_raw_data_one_shot(tcs34725_color_t* color, tcs34725_config_t* config) {
    tcs34725_err_t err = TCS34725_OK;

    err |= tcs34725_enable(config);
    err |= tcs34725_get_raw_data(color, config);
    err |= tcs34725_disable(config);

    return err;
}


tcs34725_err_t tcs34725_get_normalized_RGB(tcs34725_normalized_color_t* normalized_color, tcs34725_config_t* config) {
    tcs34725_err_t err = TCS34725_OK;

    tcs34725_color_t color;
    err = tcs34725_get_raw_data(&color, config);

    if(err == TCS34725_OK){
        uint32_t sum = color.clear;
        // Check for a divide by zero error
        if (color.clear == 0) {
            normalized_color->red = 0;
            normalized_color->green = 0;
            normalized_color->blue = 0;
        }else{
            normalized_color->red   = (float)color.red   / sum * 255.0;
            normalized_color->green = (float)color.green / sum * 255.0;
            normalized_color->blue  = (float)color.blue  / sum * 255.0;
        }
    }
    return err;
}


uint16_t tcs34725_calculate_color_temperature(tcs34725_color_t color) {
    float X, Y, Z; // RGB to XYZ correlation
    float xc, yc;  // Chromaticity co-ordinates
    float n;       // McCamy's formula
    float cct;

    if (color.red == 0 && color.green == 0 && color.blue == 0) {
        return 0;
    }

    /* 1. Map RGB values to their XYZ counterparts.
          Based on 6500K fluorescent, 3000K fluorescent
          and 60W incandescent values for a wide range.
          Note: Y = Illuminance or lux */
    X = (-0.14282F * color.red) + (1.54924F * color.green) + (-0.95641F * color.blue);
    Y = (-0.32466F * color.red) + (1.57837F * color.green) + (-0.73191F * color.blue);
    Z = (-0.68202F * color.red) + (0.77073F * color.green) + (0.56332F * color.blue);

    // 2. Calculate the chromaticity co-ordinates
    xc = (X) / (X + Y + Z);
    yc = (Y) / (X + Y + Z);

    // 3. Use McCamy's formula to determine the CCT
    n = (xc - 0.3320F) / (0.1858F - yc);

    // Calculate the final CCT
    cct = (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

    // Return the results in degrees Kelvin
    return (uint16_t)cct;
}


uint16_t tcs34725_calculate_color_temperature_dn40(tcs34725_color_t color, tcs34725_config_t* config) {
    uint16_t r2, b2; // RGB values minus IR component
    uint16_t sat;    // Digital saturation level
    uint16_t ir;     // Inferred IR content

    if (color.clear == 0) {
        return 0;
    }

    /* Analog/Digital saturation:
     *
     * (a) As light becomes brighter, the clear channel will tend to
     *     saturate first since R+G+B is approximately equal to C.
     * (b) The TCS34725 accumulates 1024 counts per 2.4ms of integration
     *     time, up to a maximum values of 65535. This means analog
     *     saturation can occur up to an integration time of 153.6ms
     *     (64*2.4ms=153.6ms).
     * (c) If the integration time is > 153.6ms, digital saturation will
     *     occur before analog saturation. Digital saturation occurs when
     *     the count reaches 65535.
     */
    if ((256 - config->settings.integration_time) > 63) {
        // Track digital saturation
        sat = 65535;
    } else {
        // Track analog saturation
        sat = 1024 * (256 - config->settings.integration_time);
    }

    /* Ripple rejection:
     *
     * (a) An integration time of 50ms or multiples of 50ms are required to
     *     reject both 50Hz and 60Hz ripple.
     * (b) If an integration time faster than 50ms is required, you may need
     *     to average a number of samples over a 50ms period to reject ripple
     *     from fluorescent and incandescent light sources.
     *
     * Ripple saturation notes:
     *
     * (a) If there is ripple in the received signal, the value read from C
     *     will be less than the max, but still have some effects of being
     *     saturated. This means that you can be below the 'sat' value, but
     *     still be saturating. At integration times >150ms this can be
     *     ignored, but <= 150ms you should calculate the 75% saturation
     *     level to avoid this problem.
     */
    if ((256 - config->settings.integration_time) <= 63) {
        // Adjust sat to 75% to avoid analog saturation if atime < 153.6ms
        sat -= sat / 4;
    }

    // Check for saturation and mark the sample as invalid if true
    if (color.clear >= sat) {
        return 0;
    }

    // AMS RGB sensors have no IR channel, so the IR content must be calculated indirectly
    if(color.red + color.green + color.blue > color.clear){
        ir = (color.red + color.green + color.blue - color.clear) / 2;
    }else{
        ir = 0;
    }

    // Remove the IR component from the raw RGB values
    r2 = color.red - ir;
    b2 = color.blue - ir;

    if (r2 == 0) {
        return 0;
    }

    /* A simple method of measuring color temp is to use the ratio of blue
       to red light, taking IR cancellation into account. */
    uint16_t cct = (3810 * (uint32_t)b2) / (uint32_t)r2 + 1391;

    return cct;
}


uint16_t tcs34725_calculate_lux(tcs34725_color_t color) {
    float illuminance;

    /* This only uses RGB ... how can we integrate clear or calculate lux
       based exclusively on clear since this might be more reliable? */
    illuminance = (-0.32466F * color.red) + (1.57837F * color.green) + (-0.73191F * color.blue);

    return (uint16_t) illuminance;
}


tcs34725_err_t tcs34725_set_interrupt(bool interrupt, tcs34725_config_t* config) {
    tcs34725_err_t err = TCS34725_OK;

    uint8_t reg_val = 0;
    err |= read8(TCS34725_ENABLE_REG, &reg_val, config);
    if (interrupt) {
        reg_val |= TCS34725_ENABLE_AIEN;
    } else {
        reg_val &= ~TCS34725_ENABLE_AIEN;
    }
    err |= write8(TCS34725_ENABLE_REG, reg_val, config);
    return err;
}


tcs34725_err_t tcs34725_clear_interrupt(tcs34725_config_t* config) {
    tcs34725_err_t err;
    if( config->write_byte(config->device_addr, TCS34725_COMMAND_FORMAT(TCS34725_COMMAND_BIT, TCS34725_SF_MODE, TCS34725_SF_INT_CLEAR)) == 0){
        err = TCS34725_OK;
    }else{
        err = TCS34725_ERR_WRITE;
    }
    return err;
}


tcs34725_err_t tcs34725_set_int_limits(uint16_t low, uint16_t high, tcs34725_config_t* config) {
    tcs34725_err_t err = TCS34725_OK;
    err |= write8(TCS34725_AILTL_REG, low & 0xFF , config);
    err |= write8(TCS34725_AILTH_REG, low >> 8   , config);
    err |= write8(TCS34725_AIHTL_REG, high & 0xFF, config);
    err |= write8(TCS34725_AIHTH_REG, high >> 8  , config);
    return err;
}