# Learning STM32L1 

A learning project using STM32L1 NUCLEO board (STM32L152RET6), GNU ARM toolchain and the ARM CMSIS Library.

Datasheet [Link] (https://www.st.com/resource/en/datasheet/stm32l152rb.pdf)
## Requirement

1. Clone the `CMSIS_5` and `cmsis_device_l1` repo: 

    ```shell
        git clone https://github.com/ARM-software/CMSIS_5.git CMSIS
        cd CMSIS/Device
        mkdir ST
        cd ST
        git clone https://github.com/STMicroelectronics/cmsis_device_l1.git STM32L1
    ```

2. `st-link`

    `apt install stlink-tools`
## Run

Step 1. Install the GNU ARM Toolchain and STM Library.

Step 2. Change the `TOOLROOT` point to folder cotaining GNU ARM Toolchains and `CMSIS_ROOT` point to folder containg `CMSIS` folder in `Makefile.common`

Step 3. `cd <target-folder>`

Step 4. `make`: creating execution file

Step 5. `make flash-to-st`: flash execution file to ST

## Target folder

1. `Dummy`
2. `Blinky`
