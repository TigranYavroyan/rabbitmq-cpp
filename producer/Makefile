NAME = prod
CXX = c++
RM = rm -f

CXXERRFLAGS = -Wall
CXXSTD = -std=c++17

SRC_DIR = ./
BUILD_DIR = build
INCLUDE_DIRS = ../dotenv

INCLUDES = $(addprefix -I,$(INCLUDE_DIRS))
CXXFLAGS = $(CXXERRFLAGS) $(CXXSTD) $(INCLUDES)

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

LIBS = -lamqpcpp -lpthread -ldl

RED    = \033[1;31m
GREEN  = \033[1;32m
YELLOW = \033[1;33m
RESET  = \033[0;37m

all: $(NAME)

$(NAME): $(OBJS) $(LIBS)
	@$(CXX) $(DEBUG) $(OBJS) $(LIBS) -o $@
	@echo "$(GREEN)[producer] - The executable $@ is ready$(RESET)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "$(YELLOW)[producer] - Compiled $< → $@$(RESET)"

clean:
	@$(RM) $(OBJS)
	@echo "$(RED)[producer] - The object files are removed$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)[producer] - The executable is removed$(RESET)"

re: fclean all

.PHONY: all clean fclean re

