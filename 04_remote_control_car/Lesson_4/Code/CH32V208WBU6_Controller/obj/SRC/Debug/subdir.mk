################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SRC/Debug/debug.c 

OBJS += \
./SRC/Debug/debug.o 

C_DEPS += \
./SRC/Debug/debug.d 


# Each subdirectory must supply rules for building sources it contributes
SRC/Debug/%.o: ../SRC/Debug/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -DCH32V20x_D8W -DCLK_OSC32K=0 -I"C:\Users\OWNER\Desktop\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.4\Դ����\CH32V208WBU6_Controller\APP\include" -I"C:\Users\OWNER\Desktop\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.4\Դ����\CH32V208WBU6_Controller\HAL\include" -I"C:\Users\OWNER\Desktop\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.4\Դ����\CH32V208WBU6_Controller\LIB" -I"C:\Users\OWNER\Desktop\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.4\Դ����\CH32V208WBU6_Controller\Profile\include" -I"C:\Users\OWNER\Desktop\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.4\Դ����\CH32V208WBU6_Controller\SRC\Core" -I"C:\Users\OWNER\Desktop\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.4\Դ����\CH32V208WBU6_Controller\SRC\Debug" -I"C:\Users\OWNER\Desktop\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.4\Դ����\CH32V208WBU6_Controller\SRC\Peripheral\inc" -I"C:\Users\OWNER\Desktop\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.4\Դ����\CH32V208WBU6_Controller\SRC\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

