# Makefile for Bomb Processor
# Written by:
#   Renato Fermi   <repiazza@gmail.com>
#   Enzo Zamberlan <sincecontatodev@gmail.com>
# in July 2025

CC            = gcc
GCC_TARGET    := $(shell $(CC) -dumpmachine)

ifeq ($(findstring mingw32, $(GCC_TARGET)), mingw32)
  _WIN32 = 1
endif

SRC_PATH      = src
API_PATH      = $(SRC_PATH)/api
INCLUDE_PATH  = include
OBJ_DIR       = obj
BIN_DIR       = bin
INC_DIR       = -I. -I$(INCLUDE_PATH)
LIBS          = 

CCOPT         = -Wall -Wextra
ifdef _WIN32
  CCOPT += -D_WIN32
  MKDIR_CMD = if not exist $(OBJ_DIR) mkdir $(OBJ_DIR) & if not exist $(BIN_DIR) mkdir $(BIN_DIR)
  RM_CMD = rmdir /S /Q $(OBJ_DIR) & rmdir /S /Q $(BIN_DIR) & del /Q *.log 2>nul || exit 0
else
  CCOPT += -DLINUX
  MKDIR_CMD = mkdir -p $(OBJ_DIR) $(BIN_DIR)
  RM_CMD = rm -rf $(OBJ_DIR) $(BIN_DIR) *.log
endif

DEBUG_ADD_FLAGS = -O2
ifdef DEBUG
  DEBUG_ADD_FLAGS = -g -ggdb
endif

BOMB_PROC_EXEC = bomb_processor

OBJS = \
	$(OBJ_DIR)/bomb_processor.o \
	$(OBJ_DIR)/parse_messages.o \
	$(OBJ_DIR)/command.o \
	$(OBJ_DIR)/trace.o 

all: clean directories $(BIN_DIR)/$(BOMB_PROC_EXEC)

clean:
	$(RM_CMD)

directories:
	$(MKDIR_CMD)

$(BIN_DIR)/$(BOMB_PROC_EXEC): $(OBJS)
	$(CC) $(LDOPT) $(INC_DIR) -o $@ $(OBJS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_PATH)/%.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INC_DIR) $< -o $@
	
$(OBJ_DIR)/%.o: $(API_PATH)/%.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INC_DIR) $< -o $@

# Extra: gerar documentação do Doxygen
doc:
	doxygen Doxyfile

# Extra: rodar o servidor
run: $(BIN_DIR)/$(BOMB_PROC_EXEC)
	./$(BIN_DIR)/$(BOMB_PROC_EXEC)

distclean: clean

.PHONY: all clean distclean directories run doc
