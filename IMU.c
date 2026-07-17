#include <stdio.h>
#include <stdint.h>
#include "IMU.h"
#include "main.h"



// ---------- Addresses ------------ //

#define WHO_AM_I_ADD 0x0F
#define CTRL1_XL_ADD 0x10
#define CTRL2_G_ADD 0x11
#define CTRL3_C_ADD 0x12
#define CTRL4_C_ADD 0x13
#define CTRL5_C_ADD 0x14
#define CTRL6_C_ADD 0x15
#define CTRL7_G_ADD 0x16
#define CTRL8_XL_ADD 0x17
#define CTRL9_XL_ADD 0x18
#define CTRL10_C_ADD 0x19


// GLOBAL VARIABLES 
volatile bool data_ready_flag = false;

volatile int16_t IMU_DATA_GYRO_X = 0;
volatile int16_t IMU_DATA_GYRO_Y = 0;
volatile int16_t IMU_DATA_GYRO_Z = 0;
volatile int16_t IMU_DATA_ACCEL_X = 0;
volatile int16_t IMU_DATA_ACCEL_Y = 0;
volatile int16_t IMU_DATA_ACCEL_Z = 0;
volatile uint8_t who_am_i_buffer = 0;

// ================ MUST CONFIGURE THIS================= //
void IMU_CONF(IMU_CONFIG_DATA *config)
{
    // CTRL1_XL (0x10) -> | ODR_XL3 | ODR_XL2 | ODR_XL1 | ODR_XL0 | FS_XL1 | FS_XL0 | LPF1_BW_SEL | BW0_XL |
    config->CTRL1_XL.ODR = 0b0000;
    config->CTRL1_XL.FS = 0b00;
    config->CTRL1_XL.LPF = 0b0;
    config->CTRL1_XL.BW = 0b0;

    // use only this line if you want to write the whole register at once, otherwise use the individual bits
    // config->CTRL1_XL.reg = 0b00000000;
    // ----------------------------------------------------------------------------------------- //

    // CTRL2_G  (0x11) -> | ODR_G3 | ODR_G2 | ODR_G1 | ODR_G0 | FS_G1 | FS_G0 | FS_125 | 0 |
    config->CTRL2_G.ODR = 0b0000;
    config->CTRL2_G.FS = 0b00;
    config->CTRL2_G.FS_125 = 0b0;
    config->CTRL2_G.unused = 0b0;

    // use only this line if you want to write the whole register at once, otherwise use the individual bits
    // config->CTRL2_G.reg = 0b00000000;
    // ----------------------------------------------------------------------------------------- //

    // CTRL3_C  (0x12) -> | BOOT | BDU | H_LACTIVE | PP_OD | SIM | IF_INC | BLE | SW_RESET |
    config->CTRL3_C.BOOT = 0b0;
    config->CTRL3_C.BDU = 0b1;
    config->CTRL3_C.H_LACTIVE = 0b0;
    config->CTRL3_C.PP_OD = 0b0;
    config->CTRL3_C.SIM = 0b0;
    config->CTRL3_C.IF_INC = 0b1;
    config->CTRL3_C.BLE = 0b0;
    config->CTRL3_C.SW_RESET = 0b0;

    // use only this line if you want to write the whole register at once, otherwise use the individual bits
    // config->CTRL3_C.reg = 0b00000000;
    // ----------------------------------------------------------------------------------------- //

    // CTRL4_C  (0x13) -> | DEN_XL_EN | SLEEP | INT2_on_INT1 | DEN_DRDY_INT1 | DRDY_MASK | I2C_disable | LPF1_SEL_G | 0 |
    config->CTRL4_C.DEN_XL_EN = 0b0;
    config->CTRL4_C.SLEEP = 0b0;
    config->CTRL4_C.INT2_ON_INT1 = 0b0;
    config->CTRL4_C.DEN_DRDY_INT1 = 0b0;
    config->CTRL4_C.DRDY_MASK = 0b0;
    config->CTRL4_C.I2C_DISABLE = 0b0;
    config->CTRL4_C.LPF1_SEL_G = 0b0;
    config->CTRL4_C.RESERVED = 0b0;

    // use only this line if you want to write the whole register at once, otherwise use the individual bits
    // config->CTRL4_C.reg = 0b00000000;
    // ----------------------------------------------------------------------------------------- //

    // CTRL5_C  (0x14) -> | ROUNDING2 | ROUNDING1 | ROUNDING0 | DEN_LH | ST1_G | ST0_G | ST1_XL | ST0_XL |
    config->CTRL5_C.ROUNDING = 0b000;
    config->CTRL5_C.DEN_LH = 0b0;
    config->CTRL5_C.ST_G = 0b00;
    config->CTRL5_C.ST_XL = 0b00;

    // use only this line if you want to write the whole register at once, otherwise use the individual bits
    // config->CTRL5_C.reg = 0b00000000;
    // ----------------------------------------------------------------------------------------- //

    // CTRL6_C  (0x15) -> | TRIG_EN | LVL1_EN | LVL2_EN | XL_HM_MODE | USR_OFF_W | 0 | FTYPE_1 | FTYPE_0 |
    config->CTRL6_C.TRIG_EN = 0b0;
    config->CTRL6_C.LVL1_EN = 0b0;
    config->CTRL6_C.LVL2_EN = 0b0;
    config->CTRL6_C.XL_HM_MODE = 0b0;
    config->CTRL6_C.USR_OFF_W = 0b0;
    config->CTRL6_C.RESERVED = 0b0;
    config->CTRL6_C.FTYPE = 0b00;

    // use only this line if you want to write the whole register at once, otherwise use the individual bits
    // config->CTRL6_C.reg = 0b00000000;
    // ----------------------------------------------------------------------------------------- //

    // CTRL7_G  (0x16) -> | G_HM_MODE | HP_G_EN | HPM1_G | HPM0_G | 0 | ROUNDING_STATUS | 0 | 0 |
    config->CTRL7_G.G_HM_MODE = 0b0;
    config->CTRL7_G.HP_G_EN = 0b0;
    config->CTRL7_G.HPM_G = 0b00;
    config->CTRL7_G.RESERVED_3 = 0b0;
    config->CTRL7_G.ROUNDING_STATUS = 0b0;
    config->CTRL7_G.RESERVED_1_0 = 0b00;

    // use only this line if you want to write the whole register at once, otherwise use the individual bits
    // config->CTRL7_G.reg = 0b00000000;
    // ----------------------------------------------------------------------------------------- //

    // CTRL8_XL (0x17) -> | LPF2_XL_EN | HPCF_XL1 | HPCF_XL0 | HP_REF_MODE | INPUT_COMPOSITE | HP_SLOPE_XL_EN | 0 | LOW_PASS_ON_6D |
    config->CTRL8_XL.LPF2_XL_EN = 0b0;
    config->CTRL8_XL.HPCF_XL = 0b00;
    config->CTRL8_XL.HP_REF_MODE = 0b0;
    config->CTRL8_XL.INPUT_COMPOSITE = 0b0;
    config->CTRL8_XL.HP_SLOPE_XL_EN = 0b0;
    config->CTRL8_XL.RESERVED = 0b0;
    config->CTRL8_XL.LOW_PASS_ON_6D = 0b0;

    // use only this line if you want to write the whole register at once, otherwise use the individual bits
    // config->CTRL8_XL.reg = 0b00000000;
    // ----------------------------------------------------------------------------------------- //

    // CTRL9_XL (0x18) -> | DEN_X | DEN_Y | DEN_Z | DEN_XL_G | 0 | SOFT_EN | 0 | 0 |
    config->CTRL9_XL.DEN_X = 0b0;
    config->CTRL9_XL.DEN_Y = 0b0;
    config->CTRL9_XL.DEN_Z = 0b0;
    config->CTRL9_XL.DEN_XL_G = 0b0;
    config->CTRL9_XL.RESERVED_3 = 0b0;
    config->CTRL9_XL.SOFT_EN = 0b0;
    config->CTRL9_XL.RESERVED_1_0 = 0b00;

    // use only this line if you want to write the whole register at once, otherwise use the individual bits
    // config->CTRL9_XL.reg = 0b00000000;
    // ----------------------------------------------------------------------------------------- //

    // CTRL10_C (0x19) -> | WRIST_TILT_EN | 0 | TIMER_EN | PEDO_EN | TILT_EN | FUNC_EN | PEDO_RST_STEP | SIGN_MOTION_EN |
    config->CTRL10_C.WRIST_TILT_EN = 0b0;
    config->CTRL10_C.RESERVED = 0b0;
    config->CTRL10_C.TIMER_EN = 0b0;
    config->CTRL10_C.PEDO_EN = 0b0;
    config->CTRL10_C.TILT_EN = 0b0;
    config->CTRL10_C.FUNC_EN = 0b0;
    config->CTRL10_C.PEDO_RST_STEP = 0b0;
    config->CTRL10_C.SIGN_MOTION_EN = 0b0;

    // use only this line if you want to write the whole register at once, otherwise use the individual bits
    // config->CTRL10_C.reg = 0b00000000;
    // ----------------------------------------------------------------------------------------- //
}

void IMU_INIT(IMU_CONFIG_DATA *config)
{
    // single line will do the work
    // must enable the auto increment of I2C
    // but firs we must enable the auto increment of I2C in CTRL3_C register

    HAL_I2C_Mem_Write(hi2c, IMU_Adress_A, CTRL3_C_ADD, I2C_MEMADD_SIZE_8BIT, &config->CTRL3_C.reg, 1, HAL_MAX_DELAY);

    // We increment from CTRL1_XL (0x10) to CTRL2_G (0x11) and then to CTRL4_C (0x13)
    // So we can write all the registers in one go, starting from CTRL1_XL
    HAL_I2C_Mem_Write(hi2c, IMU_Adress_A, CTRL1_XL_ADD, I2C_MEMADD_SIZE_8BIT, &config->CTRL1_XL.reg, 2, HAL_MAX_DELAY);
    HAL_I2C_Mem_Write(hi2c, IMU_Adress_A, CTRL4_C_ADD, I2C_MEMADD_SIZE_8BIT, &config->CTRL4_C.reg, 7, HAL_MAX_DELAY);

    //jumpstart the IMU by reading the WHO_AM_I register
    HAL_I2C_Mem_Read_DMA(hi2c, IMU_Adress_A, WHO_AM_I_ADD, I2C_MEMADD_SIZE_8BIT, &who_am_i_buffer, 1);
    HAL_Delay(100);
}

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

/* =======================================================================================================
 * LSM6DS3TR-C MAIN CONTROL REGISTERS OVERVIEW
 * ======================================================================================================= */

// CTRL1_XL (0x10) -> | ODR_XL3 | ODR_XL2 | ODR_XL1 | ODR_XL0 | FS_XL1 | FS_XL0 | LPF1_BW_SEL | BW0_XL |
// Purpose: Accelerometer main control. Configures Output Data Rate (ODR), full-scale sensitivity range,
//          and low-pass bandwidth selection.

// CTRL2_G  (0x11) -> | ODR_G3 | ODR_G2 | ODR_G1 | ODR_G0 | FS_G1 | FS_G0 | FS_125 | 0 |
// Purpose: Gyroscope main control. Configures Output Data Rate (ODR) and full-scale sensitivity range.

// CTRL3_C  (0x12) -> | BOOT | BDU | H_LACTIVE | PP_OD | SIM | IF_INC | BLE | SW_RESET |
// Purpose: Main system configuration. Controls Block Data Update (BDU) for stable reads, auto-increment,
//          interrupt pin polarity, software resets, and memory rebooting.

// CTRL4_C  (0x13) -> | DEN_XL_EN | SLEEP | INT2_on_INT1 | DEN_DRDY_INT1 | DRDY_MASK | I2C_disable | LPF1_SEL_G | 0 |
// Purpose: Data routing and interface config. Disables I2C, masks Data-Ready (DRDY) until filters settle,
//          routes INT2 signals over to INT1, and sets Gyroscope sleep mode.

// CTRL5_C  (0x14) -> | ROUNDING2 | ROUNDING1 | ROUNDING0 | DEN_LH | ST1_G | ST0_G | ST1_XL | ST0_XL |
// Purpose: Self-test and rounding. Triggers mechanical hardware self-tests for the Gyro/Accel and enables
//          circular burst-reading (rounding) modes.

// CTRL6_C  (0x15) -> | TRIG_EN | LVL1_EN | LVL2_EN | XL_HM_MODE | USR_OFF_W | 0 | FTYPE_1 | FTYPE_0 |
// Purpose: Advanced power and triggers. Configures external Edge/Level trigger modes (DEN pin), sets
//          Gyroscope LPF1 cutoff frequency, and toggles Accelerometer High-Performance mode.

// CTRL7_G  (0x16) -> | G_HM_MODE | HP_G_EN | HPM1_G | HPM0_G | 0 | ROUNDING_STATUS | 0 | 0 |
// Purpose: Gyroscope filters and modes. Configures the Gyroscope high-pass filter, toggles Gyroscope
//          High-Performance mode, and enables source register rounding.

// CTRL8_XL (0x17) -> | LPF2_XL_EN | HPCF_XL1 | HPCF_XL0 | HP_REF_MODE | INPUT_COMPOSITE | HP_SLOPE_XL_EN | 0 | LOW_PASS_ON_6D |
// Purpose: Accelerometer composite filters. Configures Accelerometer low-pass/high-pass filters and applies
//          low-pass filtering specifically to the 6D orientation algorithm.

// CTRL9_XL (0x18) -> | DEN_X | DEN_Y | DEN_Z | DEN_XL_G | 0 | SOFT_EN | 0 | 0 |
// Purpose: Sensor hub and external data. Selects specific axes for Data Enable (DEN) stamping and enables
//          the soft-iron correction algorithm for external magnetometers.

// CTRL10_C (0x19) -> | WRIST_TILT_EN | 0 | TIMER_EN | PEDO_EN | TILT_EN | FUNC_EN | PEDO_RST_STEP | SIGN_MOTION_EN |
// Purpose: Embedded functions setup. Acts as the master switch (FUNC_EN) to globally enable hardware features
//          like the pedometer, timestamp timer, tilt detection, and significant motion.

