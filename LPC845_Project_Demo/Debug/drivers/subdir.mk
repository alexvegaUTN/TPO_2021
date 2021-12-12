################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/DR_Timer.c \
../drivers/DR_UART0.c \
../drivers/PR_Timer.c \
../drivers/PR_UART0.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_gpio.c \
../drivers/fsl_power.c \
../drivers/fsl_reset.c \
../drivers/fsl_swm.c \
../drivers/fsl_usart.c 

OBJS += \
./drivers/DR_Timer.o \
./drivers/DR_UART0.o \
./drivers/PR_Timer.o \
./drivers/PR_UART0.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_gpio.o \
./drivers/fsl_power.o \
./drivers/fsl_reset.o \
./drivers/fsl_swm.o \
./drivers/fsl_usart.o 

C_DEPS += \
./drivers/DR_Timer.d \
./drivers/DR_UART0.d \
./drivers/PR_Timer.d \
./drivers/PR_UART0.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_gpio.d \
./drivers/fsl_power.d \
./drivers/fsl_reset.d \
./drivers/fsl_swm.d \
./drivers/fsl_usart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/home/federico/Documents/MCUXpresso_11.4.1_6260/workspace/LPC845_Project_Demo/board" -I"/home/federico/Documents/MCUXpresso_11.4.1_6260/workspace/LPC845_Project_Demo/source" -I"/home/federico/Documents/MCUXpresso_11.4.1_6260/workspace/LPC845_Project_Demo/drivers" -I"/home/federico/Documents/MCUXpresso_11.4.1_6260/workspace/LPC845_Project_Demo/component/uart" -I"/home/federico/Documents/MCUXpresso_11.4.1_6260/workspace/LPC845_Project_Demo/utilities" -I"/home/federico/Documents/MCUXpresso_11.4.1_6260/workspace/LPC845_Project_Demo/CMSIS" -I"/home/federico/Documents/MCUXpresso_11.4.1_6260/workspace/LPC845_Project_Demo/device" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


