################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/Callback.cpp \
../source/TM1637Display.cpp \
../source/main.cpp \
../source/mkl_DevGPIO.cpp 

OBJS += \
./source/Callback.o \
./source/TM1637Display.o \
./source/main.o \
./source/mkl_DevGPIO.o 

CPP_DEPS += \
./source/Callback.d \
./source/TM1637Display.d \
./source/main.d \
./source/mkl_DevGPIO.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DCPU_MKL25Z128VFM4 -DCPU_MKL25Z128VFM4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"C:\Users\caio_\Desktop\MonitoriaMCU\Display_TM1637\TM1637_ARM-Architeture\board" -I"C:\Users\caio_\Desktop\MonitoriaMCU\Display_TM1637\TM1637_ARM-Architeture\source" -I"C:\Users\caio_\Desktop\MonitoriaMCU\Display_TM1637\TM1637_ARM-Architeture" -I"C:\Users\caio_\Desktop\MonitoriaMCU\Display_TM1637\TM1637_ARM-Architeture\drivers" -I"C:\Users\caio_\Desktop\MonitoriaMCU\Display_TM1637\TM1637_ARM-Architeture\CMSIS" -I"C:\Users\caio_\Desktop\MonitoriaMCU\Display_TM1637\TM1637_ARM-Architeture\utilities" -I"C:\Users\caio_\Desktop\MonitoriaMCU\Display_TM1637\TM1637_ARM-Architeture\startup" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fno-rtti -fno-exceptions -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


