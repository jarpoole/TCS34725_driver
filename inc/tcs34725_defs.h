
#ifndef TCS34725_DEFS_H
#define TCS34725_DEFS_H

#include <stdint.h>
#include <stddef.h>

//Device Bus Address
#define TCS34725_ADDRESS        (0x29)        // I2C address

//Control bits
#define TCS34725_COMMAND_FORMAT(command_bit, transaction_type, address) \
    ( ((command_bit)<<7 & 0b10000000) | ((transaction_type)<<5 & 0b01100000) | ((address) & 0b00011111) )
#define TCS34725_COMMAND_BIT    (0b1)
#define TCS34725_REPEATED_BYTE  (0b00)
#define TCS34725_INCREMENT_ADDR (0b01)
#define TCS34725_SF_MODE        (0b11)
#define TCS34725_SF_INT_CLEAR   (0b00110)

//Register Addresses
#define TCS34725_ENABLE_REG     (0x00)        // Interrupt Enable register
#define TCS34725_ATIME_REG      (0x01)        // Integration time register
#define TCS34725_WTIME_REG      (0x03)        // Wait time register (if TCS34725_ENABLE_WEN is asserted)
#define TCS34725_AILTL_REG      (0x04)        // Clear channel lower interrupt threshold register (lower byte)
#define TCS34725_AILTH_REG      (0x05)        // Clear channel lower interrupt threshold register (higher byte)
#define TCS34725_AIHTL_REG      (0x06)        // Clear channel upper interrupt threshold register (lower byte)
#define TCS34725_AIHTH_REG      (0x07)        // Clear channel upper interrupt threshold register (higher byte)
#define TCS34725_PERS_REG       (0x0C)        // Persistence register (basic SW filtering mechanism for interrupts)
#define TCS34725_CONFIG_REG     (0x0D)        // Configuration register
#define TCS34725_CONTROL_REG    (0x0F)        // Gain level control register
#define TCS34725_ID_REG         (0x12)        // ID register (0x44 = TCS34721/TCS34725, 0x4D = TCS34723/TCS34727)
#define TCS34725_STATUS_REG     (0x13)        // Device status register
#define TCS34725_CDATAL_REG     (0x14)        // Clear channel data low byte register
#define TCS34725_CDATAH_REG     (0x15)        // Clear channel data high byte register
#define TCS34725_RDATAL_REG     (0x16)        // Red channel data low byte register
#define TCS34725_RDATAH_REG     (0x17)        // Red channel data high byte register
#define TCS34725_GDATAL_REG     (0x18)        // Green channel data low byte register
#define TCS34725_GDATAH_REG     (0x19)        // Green channel data high byte register
#define TCS34725_BDATAL_REG     (0x1A)        // Blue channel data low byte register
#define TCS34725_BDATAH_REG     (0x1B)        // Blue channel data high byte register

//Enable/Disable options (write to TCS34725_ENABLE_REG register)
#define TCS34725_ENABLE_AIEN    (0b00010000)  // RGBC Interrupt Enable
#define TCS34725_ENABLE_WEN     (0b00001000)  // Wait Enable - Writing 1 activates the wait timer
#define TCS34725_ENABLE_AEN     (0b00000010)  // RGBC Enable - Writing 1 actives the ADC, 0 disables it
#define TCS34725_ENABLE_PON     (0b00000001)  // Power on - Writing 1 activates the internal oscillator, 0 disables it

//Wait time WTIME bits7:0 options presented in the datasheet (write to TCS34725_WTIME_REG register), more options are possible
#define TCS34725_WTIME_2_4MS    (0xFF)        // WLONG0 = 2.4ms   WLONG1 = 0.029s
#define TCS34725_WTIME_204MS    (0xAB)        // WLONG0 = 204ms   WLONG1 = 2.45s
#define TCS34725_WTIME_614MS    (0x00)        // WLONG0 = 614ms   WLONG1 = 7.4s

//RGBC time ATIME bits7:0 options presented in the datasheet (write to TCS34725_ATIME_REG register), more options are possible
#define TCS34725_ATIME_2_4MS    (0xFF)        // INTEG_CYCLES = 1,   TIME = 2,4ms, MAX COUNT = 1024
#define TCS34725_ATIME_24MS     (0xF6)        // INTEG_CYCLES = 10,  TIME = 24ms,  MAX COUNT = 10240
#define TCS34725_ATIME_101MS    (0xD5)        // INTEG_CYCLES = 42,  TIME = 101ms, MAX COUNT = 43008
#define TCS34725_ATIME_154MS    (0xC0)        // INTEG_CYCLES = 64,  TIME = 154ms, MAX COUNT = 65535
#define TCS34725_ATIME_614MS    (0x00)        // INTEG_CYCLES = 256, TIME = 614ms, MAX COUNT = 65535

//Interrupt period APERS bits3:0 (write to TCS34725_PERS_REG register)
#define TCS34725_PERS_NONE      (0b00000000)  // Every RGBC cycle generates an interrupt
#define TCS34725_PERS_1_CYCLE   (0b00000001)  // 1 clean channel value outside threshold range generates an interrupt
#define TCS34725_PERS_2_CYCLE   (0b00000010)  // 2 clean channel values outside threshold range generates an interrupt
#define TCS34725_PERS_3_CYCLE   (0b00000011)  // 3 clean channel values outside threshold range generates an interrupt
#define TCS34725_PERS_5_CYCLE   (0b00000100)  // 5 clean channel values outside threshold range generates an interrupt
#define TCS34725_PERS_10_CYCLE  (0b00000101)  // 10 clean channel values outside threshold range generates an interrupt
#define TCS34725_PERS_15_CYCLE  (0b00000110)  // 15 clean channel values outside threshold range generates an interrupt
#define TCS34725_PERS_20_CYCLE  (0b00000111)  // 20 clean channel values outside threshold range generates an interrupt
#define TCS34725_PERS_25_CYCLE  (0b00001000)  // 25 clean channel values outside threshold range generates an interrupt
#define TCS34725_PERS_30_CYCLE  (0b00001001)  // 30 clean channel values outside threshold range generates an interrupt
#define TCS34725_PERS_35_CYCLE  (0b00001010)  // 35 clean channel values outside threshold range generates an interrupt
#define TCS34725_PERS_40_CYCLE  (0b00001011)  // 40 clean channel values outside threshold range generates an interrupt
#define TCS34725_PERS_45_CYCLE  (0b00001100)  // 45 clean channel values outside threshold range generates an interrupt
#define TCS34725_PERS_50_CYCLE  (0b00001101)  // 50 clean channel values outside threshold range generates an interrupt
#define TCS34725_PERS_55_CYCLE  (0b00001110)  // 55 clean channel values outside threshold range generates an interrupt
#define TCS34725_PERS_60_CYCLE  (0b00001111)  // 60 clean channel values outside threshold range generates an interrupt

//Configuration register options (write to TCS34725_CONFIG_REG register)
#define TCS34725_CONFIG_WLONG   (0b00000010)  // When WLONG bit is 1, TCS34725_WTIME wait time value is 12x longer

//Control register options (write to TCS34725_CONTROL_REG register)
#define TCS34725_AGAIN_1X       (0b00000000)  // RGBC 1x gain
#define TCS34725_AGAIN_4X       (0b00000001)  // RGBC 4x gain
#define TCS34725_AGAIN_16X      (0b00000010)  // RGBC 16x gain
#define TCS34725_AGAIN_60X      (0b00000011)  // RGBC 60x gain

//ID register device ID's (read from TCS34725_ID_REG register)
#define TCS34725_ID             (0x44)        // Tested with this driver
#define TCS34727_ID             (0x4D)        // Untested

//Status register flags (read from TCS34725_STATUS_REG register)
#define TCS34727_FLAG_AINT      (0b00010000)  // RGBC clear channel interrupt flag
#define TCS34727_FLAG_AVALID    (0b00000001)  // RGBC Valid flag (Indicates that the RGBC channels have completed an integration cycle)




// Integration time setting enumeration for TCS34725 (Preferred over hard defines)
typedef enum {
    TCS34725_INTEGRATIONTIME_2_4MS = 0xFF,  //  2.4ms - 1 cycle    - Max Count: 1024
    TCS34725_INTEGRATIONTIME_24MS =  0xF6,  //  24ms  - 10 cycles  - Max Count: 10240
    TCS34725_INTEGRATIONTIME_50MS =  0xEB,  //  50ms  - 20 cycles  - Max Count: 20480
    TCS34725_INTEGRATIONTIME_101MS = 0xD5,  //  101ms - 42 cycles  - Max Count: 43008
    TCS34725_INTEGRATIONTIME_154MS = 0xC0,  //  154ms - 64 cycles  - Max Count: 65535
    TCS34725_INTEGRATIONTIME_700MS = 0x00   //  700ms - 256 cycles - Max Count: 65535
} tcs34725_integration_time_t;

// Gain setting enumeration for TCS34725 (Preferred over hard defines)
typedef enum {
    TCS34725_GAIN_1X = 0x00,  // No gain
    TCS34725_GAIN_4X = 0x01,  // 4x gain
    TCS34725_GAIN_16X = 0x02, // 16x gain
    TCS34725_GAIN_60X = 0x03  // 60x gain
} tcs34725_gain_t;

// Error codes for TCS34725
typedef enum {
    TCS34725_OK,
    TCS34725_ERR_UNKNOWN,
    TCS34725_ERR_DEVICE_NOT_FOUND,
    TCS34725_ERR_WRITE,
    TCS34725_ERR_READ,
} tcs34725_err_t;

// Define platform specific function pointers (return 0 for success, return non-zero for error)
typedef int8_t (*tcs34725_read_reg_fptr_t)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint32_t len);
typedef int8_t (*tcs34725_write_reg_fptr_t)(uint8_t dev_addr, uint8_t reg_addr, const uint8_t *reg_data, uint32_t len);
typedef int8_t (*tcs34725_write_byte_fptr_t)(uint8_t dev_addr, uint8_t single_byte);
typedef void   (*tcs34725_delay_ms_fptr_t)(uint32_t period);

// Color type
typedef struct {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t clear;
} tcs34725_color_t;

// Normalized color type (single precision)
typedef struct {
    float red;
    float green;
    float blue;
} tcs34725_normalized_color_t;

// Device settings
typedef struct {
    tcs34725_gain_t gain;
    tcs34725_integration_time_t integration_time;
} tcs34725_settings_t;

// Device configuration
typedef struct {
    uint8_t device_addr;                    // I2C device bus address
    tcs34725_read_reg_fptr_t read_reg;      // Read register function pointer
    tcs34725_write_reg_fptr_t write_reg;    // Write register function pointer
    tcs34725_write_byte_fptr_t write_byte;  // Write a single byte function pointer
    tcs34725_delay_ms_fptr_t delay_ms;      // Delay function pointer
    tcs34725_settings_t settings;           // Sensor settings
} tcs34725_config_t;





#endif