################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Profile/devinfoservice.c \
../Profile/gattprofile.c 

OBJS += \
./Profile/devinfoservice.o \
./Profile/gattprofile.o 

C_DEPS += \
./Profile/devinfoservice.d \
./Profile/gattprofile.d 


# Each subdirectory must supply rules for building sources it contributes
Profile/%.o: ../Profile/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -DCH32V20x_D8W -I"D:\GoingOn\����\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.2\Դ����\CH32V208WBU6_Peripheral\APP\include" -I"D:\GoingOn\����\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.2\Դ����\CH32V208WBU6_Peripheral\HAL\include" -I"D:\GoingOn\����\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.2\Դ����\CH32V208WBU6_Peripheral\LIB" -I"D:\GoingOn\����\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.2\Դ����\CH32V208WBU6_Peripheral\Profile\include" -I"D:\GoingOn\����\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.2\Դ����\CH32V208WBU6_Peripheral\SRC\Core" -I"D:\GoingOn\����\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.2\Դ����\CH32V208WBU6_Peripheral\SRC\Debug" -I"D:\GoingOn\����\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.2\Դ����\CH32V208WBU6_Peripheral\SRC\Peripheral\inc" -I"D:\GoingOn\����\�γ̽�ѧ\��ѧ�ļ�\�μ�\Lesson.2\Դ����\CH32V208WBU6_Peripheral\SRC\Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

