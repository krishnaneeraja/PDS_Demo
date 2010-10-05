################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/V91Modem.o \
../src/db_config.o \
../src/dn.o \
../src/download.o \
../src/e_sqlite.o \
../src/ethernet.o \
../src/exch_rate.o \
../src/globals.o \
../src/grn.o \
../src/gsm_gprs.o \
../src/login.o \
../src/main_menu.o \
../src/parse.o \
../src/pppcom.o \
../src/price_list.o \
../src/reader.o \
../src/reports.o \
../src/retail.o \
../src/returns.o \
../src/scroll_menu.o \
../src/synchronise.o \
../src/utils.o 

C_SRCS += \
../src/V91Modem.c \
../src/configuration.c \
../src/convert.c \
../src/db_config.c \
../src/dn.c \
../src/download.c \
../src/e_sqlite.c \
../src/ethernet.c \
../src/exch_rate.c \
../src/fileupload.c \
../src/globals.c \
../src/grn.c \
../src/gsm_gprs.c \
../src/httplint.c \
../src/insert.c \
../src/login.c \
../src/main_menu.c \
../src/parse.c \
../src/pppcom.c \
../src/price_list.c \
../src/reader.c \
../src/reports.c \
../src/retail.c \
../src/returns.c \
../src/scroll_menu.c \
../src/synchronise.c \
../src/utils.c 

OBJS += \
./src/V91Modem.o \
./src/configuration.o \
./src/convert.o \
./src/db_config.o \
./src/dn.o \
./src/download.o \
./src/e_sqlite.o \
./src/ethernet.o \
./src/exch_rate.o \
./src/fileupload.o \
./src/globals.o \
./src/grn.o \
./src/gsm_gprs.o \
./src/httplint.o \
./src/insert.o \
./src/login.o \
./src/main_menu.o \
./src/parse.o \
./src/pppcom.o \
./src/price_list.o \
./src/reader.o \
./src/reports.o \
./src/retail.o \
./src/returns.o \
./src/scroll_menu.o \
./src/synchronise.o \
./src/utils.o 

C_DEPS += \
./src/V91Modem.d \
./src/configuration.d \
./src/convert.d \
./src/db_config.d \
./src/dn.d \
./src/download.d \
./src/e_sqlite.d \
./src/ethernet.d \
./src/exch_rate.d \
./src/fileupload.d \
./src/globals.d \
./src/grn.d \
./src/gsm_gprs.d \
./src/httplint.d \
./src/insert.d \
./src/login.d \
./src/main_menu.d \
./src/parse.d \
./src/pppcom.d \
./src/price_list.d \
./src/reader.d \
./src/reports.d \
./src/retail.d \
./src/returns.d \
./src/scroll_menu.d \
./src/synchronise.d \
./src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I../include -I./include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


