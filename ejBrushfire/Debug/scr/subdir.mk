################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../scr/brushfireMap.cc \
../scr/createMap.cc \
../scr/distanceMap.cc \
../scr/potentialMap.cc \
../scr/runBrushfire.cc 

OBJS += \
./scr/brushfireMap.o \
./scr/createMap.o \
./scr/distanceMap.o \
./scr/potentialMap.o \
./scr/runBrushfire.o 

CC_DEPS += \
./scr/brushfireMap.d \
./scr/createMap.d \
./scr/distanceMap.d \
./scr/potentialMap.d \
./scr/runBrushfire.d 


# Each subdirectory must supply rules for building sources it contributes
scr/%.o: ../scr/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/player-3.0/ -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags playerc++` -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


