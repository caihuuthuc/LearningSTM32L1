# Blinky

This target project introduce RCC and GPIO concepts. 

## RCC
**Reset and Clock Control (RCC)** is the a complex clock distribution system of STM32 which ensures that only those peripherals that are actually needed are powered.

The STM32 peripherals are organized into *three distinct groups* called APB1, APB2, and AHB. 

**APB1** peripherals include the I2C devices, USARTs 2-5, and SPI devices

**APB2** devices include the GPIO ports, ADC controllers and USART 1. 

**AHB** devices are primarily memory oriented including the DMA controllers and external memory interfaces (for some devices)

## GPIOs

Most of the pins of the STM32 can be configured as input or output and may be connected to either the GPIO ports or "alternative functions" (other peripherals). As a standard naming convention, the pins are called by their GPIO function – for example PA0 (bit 0 of port A) or PB9 (bit 9 of port B). Indeed, the labeling of the discovery board follows this convention. Each pins can be configured in the mode included 

- Input floating -- Default
- Input pull-down 
- Input pull-up 
- Analog 

- Output open-drain with pull-up or pull-down capabilities 
- Output push-pull with pull-up or pull-down capabilities

- Alternate function open-drain with pull-up or pull-down capabilities
- Alternate function push-pull with pull-up or pull-down capabilities


Each **General Purpose I/O (GPIO)** port has 
1. four 32-bit configuration registers (GPIOx_MODER, GPIOx_OTYPER, GPIOx_OSPEEDR and GPIOx_PUPDR), 
2. two 32-bit data registers (GPIOx_IDR and GPIOx_ODR)
3. a 32-bit set/reset register (GPIOx_BSRR), 
4. a 32-bit locking register (GPIOx_LCKR) and 
5. two 32-bit alternate function selection register (GPIOx_AFRH and GPIOx_AFRL).

# Sub-target 
