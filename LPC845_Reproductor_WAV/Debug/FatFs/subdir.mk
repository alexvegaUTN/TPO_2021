################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FatFs/diskio.c \
../FatFs/ff.c \
../FatFs/ffsystem.c 

OBJS += \
./FatFs/diskio.o \
./FatFs/ff.o \
./FatFs/ffsystem.o 

C_DEPS += \
./FatFs/diskio.d \
./FatFs/ff.d \
./FatFs/ffsystem.d 


# Each subdirectory must supply rules for building sources it contributes
FatFs/%.o: ../FatFs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -D__LPC84X__ -D__REDLIB__ -DSDK_DEBUGCONSOLE=1 -I"/home/federico/Documents/MCUXpresso_11.4.1_6260/workspace/LPC845_Reproductor_WAV/inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0 -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


