################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/aeabi_romdiv_patch.s 

C_SRCS += \
../src/AP_Serial.c \
../src/DR_DAC.c \
../src/DR_GPIO.c \
../src/DR_IOCON.c \
../src/DR_MRT.c \
../src/DR_NVIC.c \
../src/DR_SPI.c \
../src/DR_SWM.c \
../src/DR_SYSCON.c \
../src/DR_SYSTICK.c \
../src/DR_Timer.c \
../src/DR_USART.c \
../src/LPC845_Reproductor_WAV.c \
../src/PR_DAC.c \
../src/PR_GPIO.c \
../src/PR_IOCON.c \
../src/PR_Leds.c \
../src/PR_MMC_SD.c \
../src/PR_SPI.c \
../src/PR_SYSCON.c \
../src/PR_SYSTICK.c \
../src/PR_Timer.c \
../src/PR_USART.c \
../src/cr_startup_lpc84x.c \
../src/crp.c \
../src/mtb.c 

OBJS += \
./src/AP_Serial.o \
./src/DR_DAC.o \
./src/DR_GPIO.o \
./src/DR_IOCON.o \
./src/DR_MRT.o \
./src/DR_NVIC.o \
./src/DR_SPI.o \
./src/DR_SWM.o \
./src/DR_SYSCON.o \
./src/DR_SYSTICK.o \
./src/DR_Timer.o \
./src/DR_USART.o \
./src/LPC845_Reproductor_WAV.o \
./src/PR_DAC.o \
./src/PR_GPIO.o \
./src/PR_IOCON.o \
./src/PR_Leds.o \
./src/PR_MMC_SD.o \
./src/PR_SPI.o \
./src/PR_SYSCON.o \
./src/PR_SYSTICK.o \
./src/PR_Timer.o \
./src/PR_USART.o \
./src/aeabi_romdiv_patch.o \
./src/cr_startup_lpc84x.o \
./src/crp.o \
./src/mtb.o 

C_DEPS += \
./src/AP_Serial.d \
./src/DR_DAC.d \
./src/DR_GPIO.d \
./src/DR_IOCON.d \
./src/DR_MRT.d \
./src/DR_NVIC.d \
./src/DR_SPI.d \
./src/DR_SWM.d \
./src/DR_SYSCON.d \
./src/DR_SYSTICK.d \
./src/DR_Timer.d \
./src/DR_USART.d \
./src/LPC845_Reproductor_WAV.d \
./src/PR_DAC.d \
./src/PR_GPIO.d \
./src/PR_IOCON.d \
./src/PR_Leds.d \
./src/PR_MMC_SD.d \
./src/PR_SPI.d \
./src/PR_SYSCON.d \
./src/PR_SYSTICK.d \
./src/PR_Timer.d \
./src/PR_USART.d \
./src/cr_startup_lpc84x.d \
./src/crp.d \
./src/mtb.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -D__LPC84X__ -D__REDLIB__ -DSDK_DEBUGCONSOLE=1 -I"/home/federico/Documents/MCUXpresso_11.4.1_6260/workspace/LPC845_Reproductor_WAV/inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0 -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__USE_ROMDIVIDE -D__LPC84X__ -D__REDLIB__ -I"/home/federico/Documents/MCUXpresso_11.4.1_6260/workspace/LPC845_Reproductor_WAV/inc" -g3 -mcpu=cortex-m0 -mthumb -specs=redlib.specs -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


