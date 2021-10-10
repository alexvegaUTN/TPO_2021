################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/aeabi_romdiv_patch.s 

C_SRCS += \
../src/AP_FuncionesMDE.c \
../src/AP_Inicializacion.c \
../src/AP_MDE.c \
../src/DR_GPIO.c \
../src/DR_SPI.c \
../src/Trama_LPC845.c \
../src/cr_startup_lpc84x.c \
../src/crp.c \
../src/mtb.c 

OBJS += \
./src/AP_FuncionesMDE.o \
./src/AP_Inicializacion.o \
./src/AP_MDE.o \
./src/DR_GPIO.o \
./src/DR_SPI.o \
./src/Trama_LPC845.o \
./src/aeabi_romdiv_patch.o \
./src/cr_startup_lpc84x.o \
./src/crp.o \
./src/mtb.o 

C_DEPS += \
./src/AP_FuncionesMDE.d \
./src/AP_Inicializacion.d \
./src/AP_MDE.d \
./src/DR_GPIO.d \
./src/DR_SPI.d \
./src/Trama_LPC845.d \
./src/cr_startup_lpc84x.d \
./src/crp.d \
./src/mtb.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -D__LPC84X__ -D__REDLIB__ -I"C:\Users\alexx\Documents\MCUXpressoIDE_11.3.1_5262\workspace\Trama_LPC845\inc" -I"C:\Users\alexx\Documents\INFO2_2021\Fuentes_LPC845\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0 -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__USE_ROMDIVIDE -D__LPC84X__ -D__REDLIB__ -I"C:\Users\alexx\Documents\MCUXpressoIDE_11.3.1_5262\workspace\Trama_LPC845\inc" -g3 -mcpu=cortex-m0 -mthumb -specs=redlib.specs -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


