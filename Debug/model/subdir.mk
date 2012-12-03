################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../model/block.c \
../model/inode.c \
../model/superblock.c 

OBJS += \
./model/block.o \
./model/inode.o \
./model/superblock.o 

C_DEPS += \
./model/block.d \
./model/inode.d \
./model/superblock.d 


# Each subdirectory must supply rules for building sources it contributes
model/%.o: ../model/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


