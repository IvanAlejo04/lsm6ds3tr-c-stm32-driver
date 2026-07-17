/*
 * IMU.h
 *
 *  Created on: Jul 16, 2026
 *      Author: Ivan
 */

#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

// DECLARE YOUR I2C HANDLE HERE
extern I2C_HandleTypeDef hi2c1;
// must define this so that it will change the I2C handle to the one you are using in your project
#define hi2c &hi2c1
#define IMU_Adress_A (0x6B << 1) // (0x6A << 1)
// (0x6B << 1); // adress if your SDA is on GND
// (0x6A << 1); // adress if your SDA is on VCC


// data flag for interrupt

extern volatile bool data_ready_flag;

// IMU data adresses
// --- GYROSCOPE (Angular Rate) ---
#define OUTX_L_G 0x22 // Gyroscope X-axis Low Byte
#define OUTX_H_G 0x23 // Gyroscope X-axis High Byte
#define OUTY_L_G 0x24 // Gyroscope Y-axis Low Byte
#define OUTY_H_G 0x25 // Gyroscope Y-axis High Byte
#define OUTZ_L_G 0x26 // Gyroscope Z-axis Low Byte
#define OUTZ_H_G 0x27 // Gyroscope Z-axis High Byte

// --- ACCELEROMETER (Linear Acceleration) ---
#define OUTX_L_XL 0x28 // Accelerometer X-axis Low Byte
#define OUTX_H_XL 0x29 // Accelerometer X-axis High Byte
#define OUTY_L_XL 0x2A // Accelerometer Y-axis Low Byte
#define OUTY_H_XL 0x2B // Accelerometer Y-axis High Byte
#define OUTZ_L_XL 0x2C // Accelerometer Z-axis Low Byte
#define OUTZ_H_XL 0x2D // Accelerometer Z-axis High Byte

// ================= IMU DATA ================= //
extern volatile int16_t IMU_DATA_GYRO_X;  // USE THIS VARIABLE TO READ GYRO X DATA
extern volatile int16_t IMU_DATA_GYRO_Y;  // USE THIS VARIABLE TO READ GYRO Y DATA
extern volatile int16_t IMU_DATA_GYRO_Z;  // USE THIS VARIABLE TO READ GYRO Z DATA
extern volatile int16_t IMU_DATA_ACCEL_X; // USE THIS VARIABLE TO READ ACCEL X DATA
extern volatile int16_t IMU_DATA_ACCEL_Y; // USE THIS VARIABLE TO READ ACCEL Y DATA
extern volatile int16_t IMU_DATA_ACCEL_Z; // USE THIS VARIABLE TO READ ACCEL Z DATA
// =========================================== //

extern  volatile uint8_t who_am_i_buffer;
// MSB ON FIRST!!
typedef struct
{
    // Register 1: CTRL1_XL (0x10)
    union
    {
        uint8_t reg; // Write raw byte here: config.CTRL1_XL.reg = 0b11110000;
        struct
        {
            uint8_t BW : 1;  // Bit 7 (Leftmost bit of binary literal)
            uint8_t LPF : 1; // Bit 6
            uint8_t FS : 2;  // Bits 5-4
            uint8_t ODR : 4; // Bits 3-0 (Rightmost bits of binary literal)
        };
    } CTRL1_XL;

    // Register 2: CTRL2_G (0x11)
    union
    {
        uint8_t reg;
        struct
        {
            uint8_t unused : 1; // Bit 7
            uint8_t FS_125 : 1; // Bit 6
            uint8_t FS : 2;     // Bits 5-4
            uint8_t ODR : 4;    // Bits 3-0
        };
    } CTRL2_G;

    // Register 3: CTRL3_C (0x12)
    union
    {
        uint8_t reg;
        struct
        {
            uint8_t SW_RESET : 1;  // Bit 7
            uint8_t BLE : 1;       // Bit 6
            uint8_t IF_INC : 1;    // Bit 5
            uint8_t SIM : 1;       // Bit 4
            uint8_t PP_OD : 1;     // Bit 3
            uint8_t H_LACTIVE : 1; // Bit 2
            uint8_t BDU : 1;       // Bit 1
            uint8_t BOOT : 1;      // Bit 0
        };
    } CTRL3_C;

    // Register 4: CTRL4_C (0x13)
    union
    {
        uint8_t reg;
        struct
        {
            uint8_t RESERVED : 1;      // Bit 7
            uint8_t LPF1_SEL_G : 1;    // Bit 6
            uint8_t I2C_DISABLE : 1;   // Bit 5
            uint8_t DRDY_MASK : 1;     // Bit 4
            uint8_t DEN_DRDY_INT1 : 1; // Bit 3
            uint8_t INT2_ON_INT1 : 1;  // Bit 2
            uint8_t SLEEP : 1;         // Bit 1
            uint8_t DEN_XL_EN : 1;     // Bit 0
        };
    } CTRL4_C;

    // Register 5: CTRL5_C (0x14)
    union
    {
        uint8_t reg;
        struct
        {
            uint8_t ST_XL : 2;    // Bits 7-6
            uint8_t ST_G : 2;     // Bits 5-4
            uint8_t DEN_LH : 1;   // Bit 3
            uint8_t ROUNDING : 3; // Bits 2-0
        };
    } CTRL5_C;

    // Register 6: CTRL6_C (0x15)
    union
    {
        uint8_t reg;
        struct
        {
            uint8_t FTYPE : 2;      // Bits 7-6
            uint8_t RESERVED : 1;   // Bit 5
            uint8_t USR_OFF_W : 1;  // Bit 4
            uint8_t XL_HM_MODE : 1; // Bit 3
            uint8_t LVL2_EN : 1;    // Bit 2
            uint8_t LVL1_EN : 1;    // Bit 1
            uint8_t TRIG_EN : 1;    // Bit 0
        };
    } CTRL6_C;

    // Register 7: CTRL7_G (0x16)
    union
    {
        uint8_t reg;
        struct
        {
            uint8_t RESERVED_1_0 : 2;    // Bits 7-6
            uint8_t ROUNDING_STATUS : 1; // Bit 5
            uint8_t RESERVED_3 : 1;      // Bit 4
            uint8_t HPM_G : 2;           // Bits 3-2
            uint8_t HP_G_EN : 1;         // Bit 1
            uint8_t G_HM_MODE : 1;       // Bit 0
        };
    } CTRL7_G;

    // Register 8: CTRL8_XL (0x17)
    union
    {
        uint8_t reg;
        struct
        {
            uint8_t LOW_PASS_ON_6D : 1;  // Bit 7
            uint8_t RESERVED : 1;        // Bit 6
            uint8_t HP_SLOPE_XL_EN : 1;  // Bit 5
            uint8_t INPUT_COMPOSITE : 1; // Bit 4
            uint8_t HP_REF_MODE : 1;     // Bit 3
            uint8_t HPCF_XL : 2;         // Bits 2-1
            uint8_t LPF2_XL_EN : 1;      // Bit 0
        };
    } CTRL8_XL;

    // Register 9: CTRL9_XL (0x18)
    union
    {
        uint8_t reg;
        struct
        {
            uint8_t RESERVED_1_0 : 2; // Bits 7-6
            uint8_t SOFT_EN : 1;      // Bit 5
            uint8_t RESERVED_3 : 1;   // Bit 4
            uint8_t DEN_XL_G : 1;     // Bit 3
            uint8_t DEN_Z : 1;        // Bit 2
            uint8_t DEN_Y : 1;        // Bit 1
            uint8_t DEN_X : 1;        // Bit 0
        };
    } CTRL9_XL;

    // Register 10: CTRL10_C (0x19)
    union
    {
        uint8_t reg;
        struct
        {
            uint8_t SIGN_MOTION_EN : 1; // Bit 7
            uint8_t PEDO_RST_STEP : 1;  // Bit 6
            uint8_t FUNC_EN : 1;        // Bit 5
            uint8_t TILT_EN : 1;        // Bit 4
            uint8_t PEDO_EN : 1;        // Bit 3
            uint8_t TIMER_EN : 1;       // Bit 2
            uint8_t RESERVED : 1;       // Bit 1
            uint8_t WRIST_TILT_EN : 1;  // Bit 0
        };
    } CTRL10_C;

} __attribute__((packed)) IMU_CONFIG_DATA;

typedef struct
{
    uint8_t gyro_x_l;
    uint8_t gyro_x_h;

    uint8_t gyro_y_l;
    uint8_t gyro_y_h;

    uint8_t gyro_z_l;
    uint8_t gyro_z_h;

    uint8_t accel_x_l;
    uint8_t accel_x_h;

    uint8_t accel_y_l;
    uint8_t accel_y_h;

    uint8_t accel_z_l;
    uint8_t accel_z_h;

} __attribute__((packed)) IMU_DATA;

void IMU_CONF(IMU_CONFIG_DATA *config);
void IMU_INIT(IMU_CONFIG_DATA *config);

// make this inline to skip CPU overhead of function call
static inline void IMU_READ(IMU_DATA *data)
{
    if (data_ready_flag)
    {
        // Reset the flag before reading the data
        
        IMU_DATA_GYRO_X = (int16_t)(data->gyro_x_h << 8) | data->gyro_x_l;
        IMU_DATA_GYRO_Y = (int16_t)(data->gyro_y_h << 8) | data->gyro_y_l;
        IMU_DATA_GYRO_Z = (int16_t)(data->gyro_z_h << 8) | data->gyro_z_l;
        IMU_DATA_ACCEL_X = (int16_t)(data->accel_x_h << 8) | data->accel_x_l;
        IMU_DATA_ACCEL_Y = (int16_t)(data->accel_y_h << 8) | data->accel_y_l;
        IMU_DATA_ACCEL_Z = (int16_t)(data->accel_z_h << 8) | data->accel_z_l;

    	data_ready_flag = false;

        HAL_I2C_Mem_Read_DMA(hi2c, IMU_Adress_A, OUTX_L_G, I2C_MEMADD_SIZE_8BIT, (uint8_t *)data, 12);

    

         // Reset the flag before reading the data
    }
}

#endif

// --- GYROSCOPE (Angular Rate) ---
// OUTX_L_G   (0x22) : Gyroscope X-axis Low Byte
// OUTX_H_G   (0x23) : Gyroscope X-axis High Byte
// OUTY_L_G   (0x24) : Gyroscope Y-axis Low Byte
// OUTY_H_G   (0x25) : Gyroscope Y-axis High Byte
// OUTZ_L_G   (0x26) : Gyroscope Z-axis Low Byte
// OUTZ_H_G   (0x27) : Gyroscope Z-axis High Byte

// --- ACCELEROMETER (Linear Acceleration) ---
// OUTX_L_XL  (0x28) : Accelerometer X-axis Low Byte
// OUTX_H_XL  (0x29) : Accelerometer X-axis High Byte
// OUTY_L_XL  (0x2A) : Accelerometer Y-axis Low Byte
// OUTY_H_XL  (0x2B) : Accelerometer Y-axis High Byte
// OUTZ_L_XL  (0x2C) : Accelerometer Z-axis Low Byte
// OUTZ_H_XL  (0x2D) : Accelerometer Z-axis High Byte
