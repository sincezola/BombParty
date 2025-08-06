# Makefile for Bomb Party
# Written by:
#   Renato Fermi   <repiazza@gmail.com>
#   Enzo Zamberlan <sincecontatodev@gmail.com>
# in July 2025

CC            = gcc
WINDRES       = windres
IMAGEMAGICK   = magick
GCC_TARGET    := $(shell $(CC) -dumpmachine)

ifeq ($(findstring mingw32, $(GCC_TARGET)), mingw32)
  _WIN32 = 1
endif

SRC_PATH      = src
API_PATH      = $(SRC_PATH)/api
INCLUDE_PATH  = include
ICON_PATH     = icon
OBJ_DIR       = obj
BIN_DIR       = bin
HTML_DIR      = html
LATEX_DIR     = latex
LOG_DIR       = log
INC_DIR       = -I. -I$(INCLUDE_PATH)
LIBS          = 

CCOPT         = -Wall -Wextra
ifdef _WIN32
  CCOPT += -D_WIN32
  ICON_OBJ = $(OBJ_DIR)/icon.o
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
	$(OBJ_DIR)/bomb_timer.o \
	$(OBJ_DIR)/sys_interface.o \
	$(OBJ_DIR)/terminal_utils.o \
	$(OBJ_DIR)/singleplayer.o \
	$(OBJ_DIR)/player.o \
	$(OBJ_DIR)/multiplayer.o \
	$(OBJ_DIR)/parse_api.o \
	$(OBJ_DIR)/room.o

ifneq ("$(wildcard $(OBJ_DIR)/icon.o)","")
  OBJS += $(OBJ_DIR)/icon.o
endif

all: clean directories $(BIN_DIR)/$(BOMB_EXEC)

with_icon: icon directories $(BIN_DIR)/$(BOMB_EXEC)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LOG_DIR) $(ICON_PATH)/icon.png $(ICON_PATH)/icon.ico $(HTML_DIR) $(LATEX_DIR)

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Target para gerar ícone a partir do WEBP
icon: $(ICON_PATH)/icon.webp $(ICON_PATH)/icon.rc
	$(IMAGEMAGICK) $< -resize 512x512 $(ICON_PATH)/icon.png
	$(IMAGEMAGICK) $(ICON_PATH)/icon.png -define icon:auto-resize=256,128,64,48,32,16 $(ICON_PATH)/icon.ico
	$(WINDRES) $(ICON_PATH)/icon.rc -O coff -o $(OBJ_DIR)/icon.o
	@echo "Ícone atualizado com sucesso!"

# Compilar recurso no Windows
$(OBJ_DIR)/icon.o: $(ICON_PATH)/icon.rc $(ICON_PATH)/icon.ico
	$(WINDRES) $(ICON_PATH)/icon.rc -O coff -o $@

$(BIN_DIR)/$(BOMB_EXEC): $(OBJS)
	$(CC) $(LDOPT) $(INC_DIR) -o $@ $(OBJS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_PATH)/%.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INC_DIR) $< -o $@
	
$(OBJ_DIR)/%.o: $(API_PATH)/%.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INC_DIR) $< -o $@

distclean: clean

.PHONY: all clean distclean directories icon
