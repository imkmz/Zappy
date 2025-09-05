##
## EPITECH PROJECT, 2024
## Thibaud WIMMER
## File description:
## Makefile
##

######################################################
#						  FILES						 #
######################################################

SERVER_NAME = zappy_server
AI_NAME = zappy_ai
GUI_NAME = zappy_gui

SRC_DIR = src

SERVER_SRC_DIR = $(SRC_DIR)/server
AI_SRC_DIR = $(SRC_DIR)/ai
GUI_SRC_DIR = $(SRC_DIR)/gui

BUILD_DIR = obj
SERVER_OBJ_DIR = $(BUILD_DIR)/server
GUI_OBJ_DIR = $(BUILD_DIR)/gui

SERVER_INCLUDE = $(SERVER_SRC_DIR)/includes
GUI_INCLUDE = $(GUI_SRC_DIR)

SERVER_SRC_FILES = $(shell find $(SERVER_SRC_DIR) -type f -name '*.c')
GUI_SRC_FILES = $(shell find $(GUI_SRC_DIR) -type f -name '*.cpp')

SERVER_OBJ_FILES = $(patsubst $(SERVER_SRC_DIR)/%.c, \
	$(SERVER_OBJ_DIR)/%.o,$(SERVER_SRC_FILES))
GUI_OBJ_FILES = $(patsubst $(GUI_SRC_DIR)/%.cpp, \
	$(GUI_OBJ_DIR)/%.o,$(GUI_SRC_FILES))

######################################################
#					   VARIABLES			 		 #
######################################################

CFLAGS_SERVER = -g -Wall -Werror -I$(SERVER_INCLUDE)
CFLAGS_GUI = -g -Wall -Werror -std=c++17 -I$(GUI_INCLUDE)

LDFLAGS_SERVER =
LDFLAGS_GUI = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

CC_SERVER = gcc
CC_GUI = g++

GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
CYAN = \033[0;36m
NC = \033[0m

EMOJI_BUILD = 🛠️
EMOJI_LINK = 🔗
EMOJI_CLEAN = 🧹
EMOJI_SUCCESS = 🎉

######################################################
#						RULES						 #
######################################################

all: $(SERVER_NAME) $(GUI_NAME) $(AI_NAME)
	@echo -e "${GREEN}${EMOJI_SUCCESS} All components \
	compiled successfully!${NC}"

$(SERVER_NAME): $(SERVER_OBJ_FILES)
	@echo -e "${CYAN}${EMOJI_LINK} Linking server...${NC}"
	@$(CC_SERVER) -o $@ $^ $(LDFLAGS_SERVER)
	@echo -e "${GREEN}${EMOJI_SUCCESS} Server compiled successfully!${NC}"

$(GUI_NAME): $(GUI_OBJ_FILES)
	@echo -e "${CYAN}${EMOJI_LINK} Linking GUI...${NC}"
	@$(CC_GUI) -o $@ $^ $(LDFLAGS_GUI)
	@echo -e "${GREEN}${EMOJI_SUCCESS} GUI compiled successfully!${NC}"

$(AI_NAME):
	@echo -e "${CYAN}${EMOJI_LINK} Preparing AI Python executable...${NC}"
	@chmod +x $(AI_SRC_DIR)/main.py
	@ln -sf $(AI_SRC_DIR)/main.py $(AI_NAME)
	@echo -e "${GREEN}${EMOJI_SUCCESS} AI executable ready!${NC}"

$(SERVER_OBJ_DIR)/%.o: $(SERVER_SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo -e "${CYAN}${EMOJI_BUILD} Compiling $<...${NC}"
	@$(CC_SERVER) -o $@ -c $< $(CFLAGS_SERVER)

$(GUI_OBJ_DIR)/%.o: $(GUI_SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo -e "${CYAN}${EMOJI_BUILD} Compiling $<...${NC}"
	@$(CC_GUI) -o $@ -c $< $(CFLAGS_GUI)

clean:
	@echo -e "${YELLOW}${EMOJI_CLEAN} Cleaning object files...${NC}"
	@rm -rf $(BUILD_DIR)

fclean: clean
	@echo -e "${YELLOW}${EMOJI_CLEAN} Cleaning executables...${NC}"
	@rm -f $(SERVER_NAME) $(GUI_NAME) $(AI_NAME)
	@rm -f coding-style-reports.log
	@rm -f vgcore.*
	@rm -f main.spec

cs: fclean
	@echo -e "${YELLOW}Running coding style check...${NC}"
	@coding-style . .
	@clear
	@if [ -s coding-style-reports.log ]; then \
		echo -e "${RED}Here are your errors:${NC}"; \
		cat coding-style-reports.log; \
	else \
		echo -e "${GREEN}No coding style errors!${NC}"; \
	fi
	@rm -f coding-style-reports.log

re: fclean all

.PHONY: all clean fclean re cs zappy_server zappy_gui zappy_ai
