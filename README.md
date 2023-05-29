# Learning STM32L1 

A learning project using STM32L1 NUCLEO board (STM32L152RET6), GNU ARM toolchain and the STM32L1xx Standard Peripheral Library.

Datasheet [Link] (https://www.st.com/resource/en/datasheet/stm32l152rb.pdf)
## Requirement

Download and unzip STM32L1xx pheripheral driver at [link](https://www.st.com/en/embedded-software/stsw-stm32077.html) (accessed 28-May-2023)

## To test

Step 1. Install the GNU ARM Toolchain and STM Library.

Step 2. Change the TOOLROOT and LIBROOT paths in Makefile.common

Step 3. `cd` to any target folder

Step 4. `make`

## Target folder

1. `Dummy`
2. `RCC`
