################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MK60D10.c 

OBJS += \
./CMSIS/system_MK60D10.o 

C_DEPS += \
./CMSIS/system_MK60D10.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK60DN512VMD10_cm4 -DCPU_MK60DN512VMD10 -DDEBUG -DPRINTF_ADVANCED_ENABLE=1 -DTWR_K60D100M -DTOWER -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../lwip/src/apps/httpd -I../lwip/src/include/lwip/apps -I../lwip/port -I../lwip/port/arch -I../lwip/src/include/lwip -I../lwip/src/include/lwip/priv -I../lwip/src/include/lwip/prot -I../lwip/src/include/netif -I../lwip/src/include/netif/ppp -I../lwip/src/include/netif/ppp/polarssl -I../lwip/src/include/posix -I../lwip/src/include/posix/sys -I../startup -I../lwip/src/apps/httpd/include -I../lwip/src -I../lwip/src/include -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m4 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


