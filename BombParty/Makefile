# Makefile for Bomb Party
# Written by:
#   Renato Fermi   <repiazza@gmail.com>
#   Enzo Zamberlan <sincecontatodev@gmail.com>
# in July 2025

UNAME_S := $(shell uname -s)

ifneq ($(findstring _NT-, $(UNAME_S)),)
  _WIN32 = 1
else
  LINUX = 1
endif
 
CC            = gcc
SRC_PATH      = src
API_PATH      = $(SRC_PATH)/api
INCLUDE_PATH  = include
OBJ_DIR       = obj
BIN_DIR       = bin

CCOPT   = -Wall -Wextra
INCDIR  = -I. -I$(SRC_PATH) -I$(INCLUDE_PATH)

LIBS    = -lcurl

ifdef _WIN32
  CCOPT += -LD:/msys64/usr/lib -LD:/msys64/mingw64/lib -lmingw32 -mwindows -D_WIN32
  LIBS  += -LD:/msys64/usr/lib -LD:/msys64/mingw64/lib -lmingw32 -mwindows -D_WIN32
endif

ifdef LINUX
  CCOPT += -Wl,-rpath,/usr/lib64 -Wl,--enable-new-dtags -DLINUX
  LIBS  += -Wl,-rpath,/usr/lib64 -Wl,--enable-new-dtags -DLINUX
endif

DEBUG_ADD_FLAGS = -O2
ifdef DEBUG
  DEBUG_ADD_FLAGS = -g -ggdb
endif

BOMB_EXEC = bombparty

OBJS = \
	$(OBJ_DIR)/bombparty.o \
	$(OBJ_DIR)/infix_generator.o \
	$(OBJ_DIR)/input.o \
	$(OBJ_DIR)/word_db.o \
	$(OBJ_DIR)/bombtimer.o \
	$(OBJ_DIR)/parse_api.o

all: clean directories $(BIN_DIR)/$(BOMB_EXEC)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) *.log

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(BIN_DIR)/$(BOMB_EXEC): $(OBJS)
	$(CC) $(LDOPT) $(INCDIR) -o $@ $(OBJS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_PATH)/%.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INCDIR) $< -o $@
	
$(OBJ_DIR)/%.o: $(API_PATH)/%.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INCDIR) $< -o $@

distclean: clean

.PHONY: all clean distclean directories
