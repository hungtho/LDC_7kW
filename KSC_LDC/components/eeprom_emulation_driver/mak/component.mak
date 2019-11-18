##############################################################################
# References to the component library (./component/lib) locations.

LIB_C_SRC       += ./components/eeprom_emulation_driver/lib/src/eeprom.c \
                   ./components/eeprom_emulation_driver/lib/src/eeprom_lld.c
LIB_CPP_SRC     +=

LIB_ASM_SRC     +=

LIB_INCLUDES    += ./components/eeprom_emulation_driver/lib/include

##############################################################################
# References to the application locations.

APP_C_SRC       += ./components/eeprom_emulation_driver/cfg/eeprom_cfg.c \
                   ./components/eeprom_emulation_driver/cfg/eeprom_lld_cfg.c

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    += ./components/eeprom_emulation_driver/cfg

##############################################################################
# Default directories, definitions and libraries.

# C/C++ definitions (-D....).
DDEFS   +=

# Assembled definitions (-D....).
DADEFS  +=

# Include files search directories separated by spaces.
DINCDIR +=

# Library files search directories separated by spaces.
DLIBDIR +=

# Libraries (-l...).
DLIBS   += ./components/flash_driver/lib/src/c55_freegcc.a
