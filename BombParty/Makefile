# Makefile for Bomb Party
# Written by:
#   Renato Fermi   <repiazza@gmail.com>
#   Enzo Zamberlan <sincecontatodev@gmail.com>
# in July 2025

UNAME_S := $(shell uname -s)

UNAME_S := $(shell uname -s)

ifneq ($(findstring _NT-, $(UNAME_S)),)
  _WIN32 = 1
else
  LINUX = 1
endif

CC            = gcc
SRC_PATH      = src
INCLUDE_PATH  = include
OBJ_DIR       = obj
BIN_DIR       = bin
BOMB_EXEC     = bombparty

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

SRCS := $(shell find $(SRC_PATH) -name '*.c')

OBJS := $(patsubst $(SRC_PATH)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: directories $(BIN_DIR)/$(BOMB_EXEC)

$(BIN_DIR)/$(BOMB_EXEC): $(OBJS)
	$(CC) $(LDOPT) $(INCDIR) -o $@ $(OBJS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INCDIR) $< -o $@

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) *.log

distclean: clean

.PHONY: all clean distclean directories
