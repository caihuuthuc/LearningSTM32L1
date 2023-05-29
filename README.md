# Learning STM32L1 

A learning project using STM32L1 NUCLEO board (STM32L152RET6), GNU ARM toolchain and the STM32L1xx Standard Peripheral Library.

Datasheet [Link] (https://www.st.com/resource/en/datasheet/stm32l152rb.pdf)
## Requirement

Clone the `STM32CubeL1` repo: 

`git clone https://github.com/STMicroelectronics/STM32CubeL1.git`


## Run

Step 1. Install the GNU ARM Toolchain and STM Library.

Step 2. Change the `TOOLROOT` point to folder cotaining GNU ARM Toolchains and `STM_LIBROOT` point to folder containg `STM32CubeL1` repo in `Makefile.common`

Step 3. `cd` to any target folder

Step 4. `make`: creating .elf file

Step 5. 

## Target folder

1. `Dummy`
2. `RCC`
