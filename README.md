# lsm6ds3tr-c-stm32-driver
stm32 driver for LSM6DS3TR (IMU)

#1 Quick Start:
  *For STM32 CubeIDE setup, you must include the header file and the C file on your project
   it must look like this:
  
  ex.
  Project Folder
    |__Inc
    |     |__main.h
    |     |__ stm32h5xx_hal_conf.h
    |     |__ stm32h5xx_t.h
    |     |__ IMU.h (put it here)
    |
    |__Src
          |__main.c
          |__stm32h5xx_hal_msp.c
          |__stm32h5xx_it.c
          |__IMU.c (put it here)

  *After adding your file to your project folder you must include the header to the main.c file
  
  *Now on your main main file you must instantiate these structs:
    - IMU_CONFIG_DATA
    - IMU_DATA
    ex.
    ![Carbon Code Snippet]https://github.com/IvanAlejo04/lsm6ds3tr-c-stm32-driver/blob/0298d297bb7461ae5b3f9e3d0782a388ed8af187/carbon.png
