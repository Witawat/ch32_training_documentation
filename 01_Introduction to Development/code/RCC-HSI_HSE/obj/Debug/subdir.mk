################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Debug/debug.c 

OBJS += \
./Debug/debug.o 

C_DEPS += \
./Debug/debug.d 


# Each subdirectory must supply rules for building sources it contributes
Debug/%.o: ../Debug/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"I:\CH32Vϵ��RISC-VMCU����ʵս����������)\���Ĵο�\RCC-HSI_HSE\Debug" -I"I:\CH32Vϵ��RISC-VMCU����ʵս����������)\���Ĵο�\RCC-HSI_HSE\Hardware\CLK" -I"I:\CH32Vϵ��RISC-VMCU����ʵս����������)\���Ĵο�\RCC-HSI_HSE\Core" -I"I:\CH32Vϵ��RISC-VMCU����ʵս����������)\���Ĵο�\RCC-HSI_HSE\User" -I"I:\CH32Vϵ��RISC-VMCU����ʵս����������)\���Ĵο�\RCC-HSI_HSE\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
