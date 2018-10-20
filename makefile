PROJECT_NAME		:=	ChiptunePlayer

# MCU param.
MK_FLAGS			:=	-mcpu=cortex-m4
MK_FLAGS			+=	-mthumb
MK_FLAGS			+=	-mfloat-abi=hard
MK_FLAGS			+=	-mfpu=fpv4-sp-d16
MK_FLAGS			+=	--specs=nano.specs

# Optimization.
CODE_OPTIMIZATION	:=	-O0 -g3

# C flags.
C_FLAGS				:=	$(MK_FLAGS) 
C_FLAGS				+=	-std=c99 
C_FLAGS				+=	-fshort-enums								

# CPP flags.
CPP_FLAGS			:=	$(MK_FLAGS)
CPP_FLAGS			+=	-Werror
CPP_FLAGS			+=	-Wall
CPP_FLAGS			+=	-Wextra
CPP_FLAGS			+=	-std=c++14
CPP_FLAGS			+=	-fno-exceptions
CPP_FLAGS			+=	-fshort-enums	

# Linker.
LD_FILES			:=	-T bsp/submodule/module_stm32f4_low_level_by_st/LD/STM32F415RGTx_FLASH.ld
STARTUPE_S_NAME		:=	bsp/submodule/module_stm32f4_low_level_by_st/startupe/startup_stm32f415xx.s

#Linker flags.
LDFLAGS				:=	$(MK_FLAGS) $(LD_FILES) -fno-exceptions
LDFLAGS				+=	-Wl,--undefined=uxTopUsedPriority			
LDFLAGS				+=	-ffunction-sections -Wl,--gc-sections
#LDFLAGS				+=	-Wl,-Map="build/$(PROJECT_NAME).map"

#**********************************************************************
# Toolchain param.
#**********************************************************************
TOOLCHAIN_PATH		=	arm-none-eabi

CC					=	$(TOOLCHAIN_PATH)-gcc
CPP					=	$(TOOLCHAIN_PATH)-g++
CCDEP				=	$(TOOLCHAIN_PATH)-gcc
LD					=	$(TOOLCHAIN_PATH)-g++
AR					=	$(TOOLCHAIN_PATH)-ar
AS					=	$(TOOLCHAIN_PATH)-gcc
OBJCOPY				=	$(TOOLCHAIN_PATH)-objcopy
OBJDUMP				=	$(TOOLCHAIN_PATH)-objdump
GDB					=	$(TOOLCHAIN_PATH)-gdb
SIZE				=	$(TOOLCHAIN_PATH)-size

#**********************************************************************
# Конфигурация проекта пользователя.
#**********************************************************************
# Все файлы из папки cfg в каталоге проекта.
PROJ_CFG_H_FILE		:=	$(wildcard cfg/*.h)
PROJ_CFG_DIR		:=	cfg
PROJ_CFG_PATH		:=	-I$(PROJ_CFG_DIR)

PROJECT_PATH		:=	$(PROJ_CFG_PATH)

PROJ_H_FILE			:=	$(shell find -maxdepth 10 -type f -name "*.h" )
PROJ_CPP_FILE		:=	$(shell find -maxdepth 10 -type f -name "*.cpp" )
PROJ_C_FILE			:=	$(shell find -maxdepth 10 -type f -name "*.c" )

PROJ_S_FILE			=	$(STARTUPE_S_NAME)

PROJ_DIR			:=	$(shell find -maxdepth 10 -type d -name "*" )

PROJ_PATH			:=	$(addprefix -I, $(PROJ_DIR))

PROJ_OBJ_FILE		:=	$(addprefix build/obj/, $(PROJ_CPP_FILE))
PROJ_OBJ_FILE		+=	$(addprefix build/obj/, $(PROJ_C_FILE))
PROJ_OBJ_FILE		+=	$(addprefix build/obj/, $(PROJ_S_FILE))

PROJ_OBJ_FILE		:=	$(patsubst %.cpp, %.o, $(PROJ_OBJ_FILE))
PROJ_OBJ_FILE		:=	$(patsubst %.c, %.o, $(PROJ_OBJ_FILE))
PROJ_OBJ_FILE		:=	$(patsubst %.s, %.o, $(PROJ_OBJ_FILE))

PROJECT_PATH		+=	$(PROJ_PATH)
PROJECT_OBJ_FILE	+=	$(PROJ_OBJ_FILE)

build/obj/%.o:	%.s
	@echo [AS] $<
	@mkdir -p $(dir $@)
	@$(AS)								\
	$(DEFINE_PROJ)						\
	$(CODE_OPTIMIZATION)				\
	$(PROJECT_PATH)						\
	-c $< -o $@


build/obj/%.o:	%.c	
	@echo [CC] $<
	@mkdir -p $(dir $@)
	@$(CC) $(C_FLAGS) 					\
	$(DEFINE_PROJ)						\
	$(CODE_OPTIMIZATION)				\
	$(PROJECT_PATH)						\
	-c $< -o $@
	
build/obj/%.o:	%.cpp	
	@echo [CPP] $<
	@mkdir -p $(dir $@)
	@$(CPP) $(CPP_FLAGS) 				\
	$(DEFINE_PROJ)						\
	$(CODE_OPTIMIZATION)				\
	$(PROJECT_PATH)						\
	-c $< -o $@

build/$(PROJECT_NAME).elf:	$(PROJECT_OBJ_FILE)
	@$(LD) $(LDFLAGS) $(PROJECT_OBJ_FILE)  -o build/$(PROJECT_NAME).elf
	@echo ' '
	@echo 'Finished building target: $@'
	@echo ' '

$(PROJECT_NAME).siz:	build/$(PROJECT_NAME).elf
	@echo 'Print Size:'
	@$(SIZE) --format=berkeley "build/$(PROJECT_NAME).elf"
	@echo ' '

all: $(PROJECT_NAME).siz
	
clear:
	@rm -R ./build
	@echo 'Project cleared!'
	@echo ' '

clear_all:	
	@rm -R ./build
	@echo 'Project cleared!'
	@echo ' '
	
clear_bsp:	
	@rm -R ./build/bsp
	@echo 'Bsp cleared!'
	@echo ' '
	
clear_user:	
	@rm -R ./build/user
	@echo 'User cash cleared!'
	@echo ' '
