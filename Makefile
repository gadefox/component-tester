#
#  Makefile
#
#  (c) 2012-2025 by Markus Reschke
#  based on code from Markus Frejek and Karl-Heinz Kübbeler
#

PROJECT := ComponentTester

DEBUG = yes

# Language of user interface.
# Available languages:
# - English (default)
# - Brazilian Portuguese
# - Czech (based on ISO 8859-1)
# - Czech 2 (with Czech characters based on ISO 8859-2)
# - Danish
# - French (based on ISO 8859-1)
# - German
# - Polish (based on ISO 8859-1)
# - Polish 2 (with Polish characters based on ISO 8859-2)
# - Spanish
# - Romanian
# - Russian (with cyrillic characters based on Windows-1251)
# - Russian 2 (with cyrillic characters based on Windows-1251)

# Choose one language:
LANG = ENGLISH
# LANG = BRAZILIAN
# LANG = CZECH
# LANG = CZECH_2
# LANG = DANISH
# LANG = FRENCH
# LANG = GERMAN
# LANG = ITALIAN
# LANG = POLISH
# LANG = POLISH_2
# LANG = ROMANIAN
# LANG = RUSSIAN
# LANG = RUSSIAN_2
# LANG = SPANISH


# Display module / controller
# Note:
# Please find source file with the same name in drv directory and adjust
#  settings.
# Comment out the default package (ST7565R, bit-bang SPI) if not used.
# To uncomment, remove the enclosing "#if 0" and "#endif" or put "//" in
#  front of both.  To comment out, remove the "//" in front of the "#if 0"
#  and "#endif".  Individual settings can be enabled by removing the leading
#  "//", or disabled by placing a "//" in front of the setting.
# Choose one display:
# DISPLAY = LCD_HD44780
# DISPLAY = LCD_HD44780
# DISPLAY = LCD_ILI9163
# DISPLAY = LCD_ILI9341
# DISPLAY = LCD_ILI9481
# DISPLAY = LCD_ILI9486
# DISPLAY = LCD_ILI9488
# DISPLAY = LCD_PCD8544
# DISPLAY = LCD_PCF8814
# DISPLAY = LCD_RD_DISPLAY
# DISPLAY = LCD_SH1106
# DISPLAY = LCD_SSD1306
# DISPLAY = LCD_SSD1306
# DISPLAY = LCD_ST7036
# DISPLAY = LCD_ST7565R
DISPLAY = LCD_ST7735
# DISPLAY = LCD_ST7735_SEMI
# DISPLAY = LCD_ST7920
# DISPLAY = LCD_VT100
# DISPLAY = TOUCH_ADS7843


# Display fonts and symbols
FONT = 10X16_HF
# FONT = 16X26_HF
# FONT = 6X8_VF
# FONT = 6X8_V_F
# FONT = 6X8_ISO8859_2_VF
# FONT = 6X8_ISO8859_2_V_F
# FONT = 8X8_H
# FONT = 8X8_HF
# FONT = 8X8_ISO8859_2_VF
# FONT = 8X8_ISO8859_2_HF
# FONT = 8X12T_ISO8859_2_VFP
# FONT = 8X12T_WIN1251_VFP
# FONT = 8X16_ISO8859_2_VFP
# FONT = 8X16_ISO8859_2_HF
# FONT = 8X16_WIN1251_VFP
# FONT = 8X16_WIN1251_HF
# FONT = 8X16ALT_WIN1251_HF
# FONT = 8X8_WIN1251_VF
# FONT = 8X8ALT_WIN1251_VF
# FONT = 8X8T_WIN1251_VF
# FONT = 10X16_ISO8859_2_HF
# FONT = 16X26_ISO8859_2_HF
# FONT = 16X26_WIN1251_HF
# FONT = ST7036

# SYMBOLS = 24X24_VFP
# SYMBOLS = 24X24_H
# SYMBOLS = 24X24_HF
SYMBOLS = 30X32_HF
# SYMBOLS = 32X32_HF
# SYMBOLS = 32X39_HF
# SYMBOLS = 24X24_ALT1_VFP
# SYMBOLS = 24X24_ALT2_VFP
# SYMBOLS = 24X24_ALT1_H
# SYMBOLS = 24X24_ALT2_H
# SYMBOLS = 30X32_ALT1_HF
# SYMBOLS = 30X32_ALT2_HF
# SYMBOLS = 32X32_ALT1_HF
# SYMBOLS = 32X32_ALT2_HF


#  MCU settings
#  - Edit to match your setup!
#

# avr-gcc: MCU model
# - ATmega 328/328P        : atmega328
# - ATmega 328PB           : atmega328pb
# - ATmega 324P/324PA      : atmega324p
# - ATmega 640             : atmega640
# - ATmega 644/644P/644PA  : atmega644
# - ATmega 1280            : atmega1280
# - ATmega 1284/1284P      : atmega1284
# - ATmega 2560            : atmega2560
# - Logic Green 328P       : atmega328p
MCU = atmega328
LGT8F = no
#LGT8F = yes

# MCU freqency:
# - 8MHz  : 8
# - 16MHz : 16
# - 20MHz : 20
# - 24MHz : 24
# - 32MHz : 32
FREQ = 16

# oscillator type
# - internal RC oscillator      : RC
# - external full swing crystal : Crystal
# - external low power crystal  : LowPower
OSCILLATOR = Crystal

# oscillator start-up cycles
# - Crystal and LowPower could also be 1024 or 256 based on fuse settings
ifeq (${OSCILLATOR},RC)
  OSC_STARTUP = 6
endif
ifeq (${OSCILLATOR},Crystal)
  OSC_STARTUP = 16384
endif
ifeq (${OSCILLATOR},LowPower)
  OSC_STARTUP = 16384
endif


#
#  avrdude settings
#  - Edit to match your setup!
#

# avrdude: part number of MCU
# - ATmega 328       : m328
# - ATmega 328P      : m328p
# - ATmega 328PB     : m328pb
# - ATmega 324P      : m324p
# - ATmega 324PA     : m324pa
# - ATmega 640       : m640
# - ATmega 644       : m644
# - ATmega 644P      : m644p
# - ATmega 644PA     : m644p
# - ATmega 1280      : m1280
# - ATmega 1284      : m1284
# - ATmega 1284P     : m1284p
# - ATmega 2560      : m2560
# - Logic Green 328P : m328p  
PARTNO = m328p

# avrdude: ISP programmer, port and options

# Arduino as ISP
#PROGRAMMER = stk500v1
#PORT = /dev/ttyACM0
#OPTIONS = -b 19200

# Bus Pirate
#PROGRAMMER = buspirate
#PORT = /dev/bus_pirate
#OPTIONS = -B 10.0

# Diamex ALL-AVR/AVR-Prog
PROGRAMMER = avrispmkII
PORT = usb
OPTIONS = -B 1.0

# Pololu USB AVR Programmer
#PROGRAMMER = stk500v2
#PORT = /dev/ttyACM0
#OPTIONS = -B 1.0

# USBasp
#PROGRAMMER = usbasp
#PORT = usb
#OPTIONS = -B 20

# USBtinyISP
#PROGRAMMER = usbtiny
#PORT = usb
#OPTIONS = -B 5.0

# Arduino Uno bootloader via serial/USB
#PROGRAMMER = arduino
#PORT = /dev/ttyACM0
#OPTIONS = -D -b 115200

# Arduino Mega2560 bootloader via serial/USB
#PROGRAMMER = wiring
#PORT = /dev/ttyACM0
#OPTIONS = -D -b 115200


#
#  settings for special options
#

# optimize interrupt vector table (requires linux/unix environment)
#OPTIMIZE_VECTORS = yes


#
#  global settings
#

# project name
NAME = ComponentTester

# name and version based on directory name
DIST = $(notdir ${CURDIR})

BIN_DIR := bin
DEP_DIR := dep
OUT_DIR := out
SRC_DIR := src

# compiler flags
CC = avr-gcc
CPP = avr-g++
CFLAGS = -Wall -Wno-comment -Os -std=gnu99
CFLAGS += -mmcu=${MCU} -DF_CPU=${FREQ}000000UL -DOSC_STARTUP=${OSC_STARTUP}
CFLAGS += -MD -MP -MT $(*F).o -MF ${DEP_DIR}/$(@F).d

# Logic Green 328P
ifeq (${LGT8F}, yes)
  CFLAGS += -D__LGT__
endif

CFLAGS += -mcall-prologues -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CFLAGS += -flto

# language
ifdef LANG  
  CFLAGS += -DLANG_${LANG}
else
  $(error No language specified!)
endif

# display
ifdef DISPLAY
  CFLAGS += -D${DISPLAY}
else
  $(error No display module enabled!)
endif

# font and symbol
ifdef FONT
  CFLAGS += -DFONT_${FONT}
  ifdef SYMBOLS
    CFLAGS += -DSYMBOLS_${SYMBOLS}
  endif
else
  ifdef SYMBOLS
    CFLAGS += -DSYMBOLS_${SYMBOLS}
  else
    $(error No font or symbol specified!)
  endif
endif

# linker flags
LDFLAGS = -mmcu=${MCU} -Wl,-Map=${BIN_DIR}/${NAME}.map
ifeq (${DEBUG},no)
  LDFLAGS += -Wl,--strip-all
endif
ifeq (${OPTIMIZE_VECTORS},yes)
  LDFLAGS += -nostartfiles
endif
# > 128kB flash
#LDFLAGS += -Wl,-relax

# hex file flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature
HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

# source files
SRC_FILES_C := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/display/*.c $(SRC_DIR)/font/*.c $(SRC_DIR)/lang/*.c $(SRC_DIR)/tool/*.c $(SRC_DIR)/symbol/*.c)

SRC_FILES_S := $(SRC_DIR)/wait.S
ifeq (${OPTIMIZE_VECTORS},yes)
  SRC_FILES_S += $(SRC_DIR)/gcrt1.S
endif

OBJECTS_C := $(patsubst $(SRC_DIR)/%.c, $(OUT_DIR)/%.o, $(SRC_FILES_C))
OBJECTS_S := $(patsubst $(SRC_DIR)/%.S, $(OUT_DIR)/%.o, $(SRC_FILES_S))

OBJECTS := ${OBJECTS_C} ${OBJECTS_S}


#
#  build
#

.PHONY: all size
all: $(DEP_DIR) ${BIN_DIR} ${BIN_DIR}/${NAME} ${BIN_DIR}/${NAME}.hex ${BIN_DIR}/${NAME}.eep ${BIN_DIR}/${NAME}.lss size

${BIN_DIR}:
	@mkdir -p ${BIN_DIR}

${DEP_DIR}:
	@mkdir -p ${DEP_DIR}


#
#  compile source files
#

# rule for all c-files
${OUT_DIR}/%.o: ${SRC_DIR}/%.c
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $< -o $@

# rule for all S-files
${OUT_DIR}/%.o: ${SRC_DIR}/%.S
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $< -o $@

-include $(DEP_DIR)/*.d


#
#  link
#

# link firmware
${BIN_DIR}/${NAME}: ${OBJECTS}
	 ${CC} ${LDFLAGS} ${OBJECTS} ${LIBDIRS} ${LIBS} -o $@

# create hex file of firmware
${BIN_DIR}/${NAME}.hex: ${BIN_DIR}/${NAME}
	avr-objcopy -O ihex ${HEX_FLASH_FLAGS} $< $@

# create hex file of EEPROM data
${BIN_DIR}/${NAME}.eep: ${BIN_DIR}/${NAME}
	avr-objcopy ${HEX_EEPROM_FLAGS} -O ihex $< $@ || exit 0

# create dump of firmware
${BIN_DIR}/${NAME}.lss: ${BIN_DIR}/${NAME}
	avr-objdump -h -S $< > $@

# output firmware size and other info
size: ${BIN_DIR}/${NAME}
	@echo
	@avr-size --mcu=${MCU} $<
#	@avr-objdump -Pmem-usage $<


#
#  special options
#

# trigger creation of gcrt1.inc
ifeq (${OPTIMIZE_VECTORS},yes)
gcrt1.S: gcrt1.inc
endif

# rule for gcrt1.inc
ifeq (${OPTIMIZE_VECTORS},yes)
gcrt1.inc: $(filter-out gcrt1.o, $(OBJECTS))
	chmod +x ./gcrt1.inc-builder
	./gcrt1.inc-builder '$(findstring -flto, ${CFLAGS})' '$^'
endif


#
#  program MCU
#

# program firmware and EEPROM data
.PHONY: upload
upload: ${NAME} ${NAME}.hex ${NAME}.eep ${NAME}.lss size
	avrdude -c ${PROGRAMMER} -P ${PORT} -p ${PARTNO} ${OPTIONS} \
	  -U flash:w:./${NAME}.hex:a -U eeprom:w:./${NAME}.eep:a

# program firmware only
.PHONY: prog_fw
prog_fw: ${NAME} ${NAME}.hex ${NAME}.lss size
	avrdude -c ${PROGRAMMER} -P ${PORT} -p ${PARTNO} ${OPTIONS} \
	  -U flash:w:./${NAME}.hex:a

# program EEPROM data only
.PHONY: prog_ee
prog_ee: ${NAME} ${NAME}.eep ${NAME}.lss size
	avrdude -c ${PROGRAMMER} -P ${PORT} -p ${PARTNO} ${OPTIONS} \
	  -U eeprom:w:./${NAME}.eep:a


#
#  misc
#

# create distribution package
.PHONY: dist clean
dist: clean
	cd ..; tar -czf ${DIST}/${DIST}.tgz \
	  ${DIST}/src/*.h ${DIST}/src/*.c ${DIST}/src/*.S ${DIST}/src/font/*.h\
	  ${DIST}src/symbol/*.h ${DIST}/src/Makefile ${DIST}/README ${DIST}/CHANGES \
	  ${DIST}/README.de ${DIST}/CHANGES.de ${DIST}/Clones \
	  ${DIST}/EUPL-v1.2.txt ${DIST}/${DEP_DIR} ${DIST}/script/gcrt1.inc-builder

# clean up
clean:
	-rm -rf ${OBJECTS} ${BIN_DIR}/${NAME} *.tgz
	-rm -rf ${BIN_DIR}/${NAME}.hex ${BIN_DIR}/${NAME}.eep ${BIN_DIR}/${NAME}.lss
	-rm -rf gcrt1.inc ${BIN_DIR}/${NAME}.map


#
#  MCU fuses
#

# ATmega 328/328P
ifeq (${MCU},atmega328)
  FAMILY = atmega328_324_640
endif

# ATmega 328PB
ifeq (${MCU},atmega328pb)
  FAMILY = atmega328pb
endif

# ATmega 324P/324PA
ifeq (${MCU},atmega324p)
  FAMILY = atmega328_324_640
endif

# ATmega 640
ifeq (${MCU},atmega640)
  FAMILY = atmega328_324_640
endif

# ATmega 644/644P/644PA
ifeq (${MCU},atmega644)
  FAMILY = atmega328_324_640
endif

# ATmega 1280
ifeq (${MCU},atmega1280)
  FAMILY = atmega328_324_640
endif

# ATmega 1284/1284P
ifeq (${MCU},atmega1284)
  FAMILY = atmega328_324_640
endif

# ATmega 2560
ifeq (${MCU},atmega2560)
  FAMILY = atmega328_324_640
endif

# ATmega 328/324/640/644/1280/1284/2560
ifeq (${FAMILY},atmega328_324_640)
  # high byte: use default settings, disable JTAG
  HFUSE = -U hfuse:w:0xd9:m
  # extended byte: BOD level 4.3V
  EFUSE = -U efuse:w:0xfc:m
  # low byte: clock settings
  ifeq (${FREQ},1)
    # internal RC oscillator (8MHz) and /1 clock divider
    LFUSE_RC = -U lfuse:w:0x62:m
    # external 8MHz crystal, full swing osc, /8 clock divider
    LFUSE_CRYSTAL = -U lfuse:w:0x77:m
    # external 8MHz crystal, low power osc, /8 clock divider
    LFUSE_LOWPOWER = -U lfuse:w:0x7f:m
  endif
  ifeq (${FREQ},8)
    # internal RC oscillator (8MHz) and /1 clock divider
    LFUSE_RC = -U lfuse:w:0xe2:m
    # external 8MHz crystal, full swing osc, /1 clock divider
    LFUSE_CRYSTAL = -U lfuse:w:0xf7:m
    # external 8MHz crystal, low power osc, /1 clock divider
    LFUSE_LOWPOWER = -U lfuse:w:0xff:m
  endif
  ifeq (${FREQ},16)
    # internal RC oscillator (8MHz) not possible
    LFUSE_RC =
    # external 16MHz crystal, full swing osc, /1 clock divider
    LFUSE_CRYSTAL = -U lfuse:w:0xf7:m
    # external 16MHz crystal, low power osc, /1 clock divider
    LFUSE_LOWPOWER = -U lfuse:w:0xff:m
  endif
  ifeq (${FREQ},20)
    # internal RC oscillator (8MHz) not possible
    LFUSE_RC =
    # external 20MHz crystal, full swing osc, /1 clock divider
    LFUSE_CRYSTAL = -U lfuse:w:0xf7:m
    # external 20MHz crystal, low power osc, /1 clock divider
    LFUSE_LOWPOWER = -U lfuse:w:0xff:m
  endif
endif

# ATmega 328PB
ifeq (${FAMILY},atmega328pb)
  # high byte: use default settings
  HFUSE = -U hfuse:w:0xd9:m
  # extended byte: BOD level 4.3V, CFD disabled
  EFUSE = -U efuse:w:0xf4:m
  # low byte: clock settings
  ifeq (${FREQ},8)
    # internal RC oscillator (8MHz) and /1 clock divider
    LFUSE_RC = -U lfuse:w:0xe2:m
    # external crystal with full swing osc not supported, use low power osc
    LFUSE_CRYSTAL = -U lfuse:w:0xff:m
    # external 8MHz crystal, low power osc, /1 clock divider
    LFUSE_LOWPOWER = -U lfuse:w:0xff:m
  endif
  ifeq (${FREQ},16)
    # internal RC oscillator (8MHz) not possible
    LFUSE_RC =
    # external crystal with full swing osc not supported, use low power osc
    LFUSE_CRYSTAL = -U lfuse:w:0xff:m
    # external 16MHz crystal, low power osc, /1 clock divider
    LFUSE_LOWPOWER = -U lfuse:w:0xff:m
  endif
endif

# select low byte
ifeq (${OSCILLATOR},RC)
  LFUSE = ${LFUSE_RC}
endif
ifeq (${OSCILLATOR},Crystal)
  LFUSE = ${LFUSE_CRYSTAL}
endif
ifeq (${OSCILLATOR},LowPower)
  LFUSE = ${LFUSE_LOWPOWER}
endif

# check fuses
FUSES =
ifneq ($(strip ${LFUSE}),)
  ifneq ($(strip ${HFUSE}),)
    FUSES = ${LFUSE} ${HFUSE} ${EFUSE}
  endif
endif

# set fuses
.PHONY: fuses
fuses:
  ifeq ($(strip ${FUSES}),)
	@echo Invalid fuse settings!
  else
	avrdude -c ${PROGRAMMER} -P ${PORT} -p ${PARTNO} ${OPTIONS} ${FUSES}
  endif
