################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/OpenGL-Viewer.cpp \
../src/Shader.cpp \
../src/TextRender.cpp 

OBJS += \
./src/OpenGL-Viewer.o \
./src/Shader.o \
./src/TextRender.o 

CPP_DEPS += \
./src/OpenGL-Viewer.d \
./src/Shader.d \
./src/TextRender.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -I/usr/include/freetype2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


