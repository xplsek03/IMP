################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/startup_mk60d10.c 

OBJS += \
./startup/startup_mk60d10.o 

C_DEPS += \
./startup/startup_mk60d10.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MK60DN512VMD10_cm4 -DCPU_MK60DN512VMD10 -DDEBUG -DPRINTF_ADVANCED_ENABLE=1 -DTWR_K60D100M -DTOWER -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DNDEBUG -D__REDLIB__ -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../lwip/src/apps/httpd -I../lwip/src/include/lwip/apps -I../lwip/port -I../lwip/port/arch -I../lwip/src/include/lwip -I../lwip/src/include/lwip/priv -I../lwip/src/include/lwip/prot -I../lwip/src/include/netif -I../lwip/src/include/netif/ppp -I../lwip/src/include/netif/ppp/polarssl -I../lwip/src/include/posix -I../lwip/src/include/posix/sys -I../startup -I../lwip/src/apps/httpd/include -I../lwip/src -I../lwip/src/include -Os -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m4 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


