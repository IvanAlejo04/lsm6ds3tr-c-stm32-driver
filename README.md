# LSM6DS3TR-C STM32 Driver

An STM32 HAL driver for the **LSM6DS3TR-C** IMU (accelerometer + gyroscope), using I2C with DMA.

> This README covers **Quick Setup only** — get it running fast. A deeper "how it works / why it works" write-up will come later.

---

## 1. Add the files to your project

Drop `IMU.h` and `IMU.c` into your STM32CubeIDE project like this:

```
📁Project Folder
  |__📂Inc
  |     |__ 📄main.h
  |     |__ 📄stm32h5xx_hal_conf.h
  |     |__ 📄stm32h5xx_t.h
  |     |__ 📄IMU.h            <-- put it here
  |
  |__📂Src
        |__ 📄main.c
        |__ 📄stm32h5xx_hal_msp.c
        |__ 📄stm32h5xx_it.c
        |__ 📄IMU.c            <-- put it here
```

Then `#include "IMU.h"` in your `main.c`.

---

## 2. Configure `IMU.h` for your project

Before doing anything else, edit these two things in `IMU.h`:

**a) I2C handle** — point it at whichever I2C peripheral you're using:

![I2C handle config](https://github.com/IvanAlejo04/lsm6ds3tr-c-stm32-driver/blob/6bff6a783e63ea871fa88e81ac95be8cf9295c43/carbon%20(1).png?raw=true)

**b) IMU I2C address** — depends on how your `SDO/SA0` pin is wired:

![IMU I2C address config](https://github.com/IvanAlejo04/lsm6ds3tr-c-stm32-driver/blob/6bff6a783e63ea871fa88e81ac95be8cf9295c43/carbon%20(2).png?raw=true)

---

## 3. Set up the DMA-complete callback (required!)

The driver relies on `data_ready_flag` to know when a new I2C DMA read has finished and it's safe to parse the buffer. **Nothing in `IMU.c` sets this flag to `true` for you** — you must set it yourself in the I2C DMA Rx-complete callback, or `IMU_READ()` will never fire.

In your `main.c` (or `stm32xxxx_it.c`), add:

![I2C DMA Rx-complete callback](https://github.com/IvanAlejo04/lsm6ds3tr-c-stm32-driver/blob/6bff6a783e63ea871fa88e81ac95be8cf9295c43/carbon%20(3).png?raw=true)

This callback fires automatically once `HAL_I2C_Mem_Read_DMA()` finishes transferring data — which is exactly what `IMU_READ()` calls internally to queue up the next read.

---

## 4. Instantiate the structs and initialize

In `main.c`, before your main loop:

![Struct instantiation and init](https://github.com/IvanAlejo04/lsm6ds3tr-c-stm32-driver/blob/6bff6a783e63ea871fa88e81ac95be8cf9295c43/carbon%20(4).png?raw=true)

---

## 5. Configure the sensor registers (`IMU_CONF` in `IMU.c`)

Before calling `IMU_INIT`, go into `IMU.c` and set up `IMU_CONF()` with your desired ODR (output data rate), full-scale range, filters, etc. Each register is broken into named bit fields so you don't have to hand-write raw binary, for example:

![CTRL1_XL register config example](https://github.com/IvanAlejo04/lsm6ds3tr-c-stm32-driver/blob/6bff6a783e63ea871fa88e81ac95be8cf9295c43/carbon%20(5).png?raw=true)

> ⚠️ **Bit-order:** the comments above each register (e.g. `| ODR_XL3 | ODR_XL2 | ... | BW0_XL |`) list bits in **MSB → LSB** order to match the datasheet table. But when writing the **raw `.reg` value** directly (the commented-out `config->CTRL1_XL.reg = 0b...` line), keep in mind that STM32CubeIDE's compiler packs struct bit fields **LSB-first** — the *first* field you declare in the struct is *not* guaranteed to land on the *leftmost* bit of the byte the way the datasheet comment visually suggests. If you always set fields individually by name (as shown above), this doesn't matter — the driver handles the mapping correctly either way. It only bites you if you try to hand-write the raw `0b...` byte yourself and assume it reads left-to-right like the datasheet table.

Do this for all 10 control registers (`CTRL1_XL` through `CTRL10_C`) before moving on.

---

## 6. Read data in your loop

In your main `while(1)` loop, just call:

![IMU_READ call in main loop](https://github.com/IvanAlejo04/lsm6ds3tr-c-stm32-driver/blob/6bff6a783e63ea871fa88e81ac95be8cf9295c43/carbon%20(6).png?raw=true)

`IMU_READ()` checks `data_ready_flag`, and when it's `true`:
1. Copies the freshly-DMA'd raw bytes into the signed 16-bit output variables.
2. Clears the flag.
3. Immediately queues up the next DMA read.

---

## 7. Use the data

Read these global variables anywhere in your code once the loop is running:

![Available data variables](https://github.com/IvanAlejo04/lsm6ds3tr-c-stm32-driver/blob/6bff6a783e63ea871fa88e81ac95be8cf9295c43/carbon%20(7).png?raw=true)

---

## Register Map Reference

| Register | Address | Purpose |
|---|---|---|
| `CTRL1_XL` | 0x10 | Accelerometer ODR, full-scale range, low-pass bandwidth |
| `CTRL2_G`  | 0x11 | Gyroscope ODR, full-scale range |
| `CTRL3_C`  | 0x12 | BDU, auto-increment, interrupt polarity, software reset/boot |
| `CTRL4_C`  | 0x13 | I2C disable, DRDY masking, INT2→INT1 routing, gyro sleep |
| `CTRL5_C`  | 0x14 | Self-test, register rounding (burst read) modes |
| `CTRL6_C`  | 0x15 | DEN trigger modes, gyro LPF1 cutoff, accel high-perf mode |
| `CTRL7_G`  | 0x16 | Gyro high-pass filter, gyro high-perf mode, rounding source |
| `CTRL8_XL` | 0x17 | Accel composite low/high-pass filters, 6D low-pass |
| `CTRL9_XL` | 0x18 | DEN axis selection, soft-iron correction (external mag) |
| `CTRL10_C` | 0x19 | Embedded functions master switch (pedometer, tilt, timer, etc.) |

### Output data registers

| Axis | Low byte | High byte |
|---|---|---|
| Gyro X | `0x22` | `0x23` |
| Gyro Y | `0x24` | `0x25` |
| Gyro Z | `0x26` | `0x27` |
| Accel X | `0x28` | `0x29` |
| Accel Y | `0x2A` | `0x2B` |
| Accel Z | `0x2C` | `0x2D` |

---

## Checklist

- [ ] `IMU.h` / `IMU.c` added to `Inc` / `Src`
- [ ] `hi2c` handle points to your I2C peripheral
- [ ] `IMU_Adress_A` matches your SDO/SA0 wiring
- [ ] `HAL_I2C_MemRxCpltCallback()` implemented and sets `data_ready_flag = true`
- [ ] `IMU_CONF()` edited with your desired register settings
- [ ] `IMU_CONF()` + `IMU_INIT()` called once before the loop
- [ ] `IMU_READ()` called every loop iteration
