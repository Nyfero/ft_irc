#    Colors
GREY = \e[90m
RED = \e[91m
GREEN = \e[92m
YELLOW = \e[93m
BLUE = \e[34m
PURPLE = \e[95m
CYAN = \e[96m
END = \e[39m

#    Showing command
DISP = TRUE

ifeq ($(DISP),TRUE)
    HIDE =
else
    HIDE = @
endif

#    Includes
CLASS_PATH = class

CFILES =	utils.hpp 		\
			server.hpp 		\
			user.hpp 		\
			channel.hpp		\
			error_msg.hpp 	\
			mode.hpp 		\
			\

CLASS = $(addprefix $(CLASS_PATH)/, $(CFILES))

#    Files
SRCS_PATH = src

FILES = main.cpp 	\
		user.cpp 	\
		server.cpp 	\
		utils.cpp 	\
		channel.cpp \
		command.cpp \
		mode.cpp 	\
		join.cpp 	\
		privmsg.cpp \
		notice.cpp 	\
		wallops.cpp	\
		invite.cpp	\
		kick.cpp	\
		list.cpp	\
		bot.cpp		\
		\


SRCS = $(addprefix $(SRCS_PATH)/, $(FILES))

#    Compilation
NAME = ircserv

CXX = clang++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

RM = rm -rf

OBJS_PATH = objs/

OBJS =     $(patsubst $(SRCS_PATH)%.cpp,    $(OBJS_PATH)%.o,    $(SRCS))

#    Rules
all: $(NAME)

$(NAME): $(OBJS_PATH) $(OBJS) $(CLASS)
	@ echo "$(BLUE)\n         ***Make $(NAME) ***\n$(END)"
	$(HIDE) $(CXX) $(CFLAGS) $(OBJS) -o $(NAME)
	@ echo "$(GREEN)\n        ---$(NAME) created ---\n$(END)"

$(OBJS_PATH):
	$(HIDE) mkdir -p $(OBJS_PATH)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp $(CLASS) Makefile
	$(HIDE) $(CXX) $(CFLAGS) -c $< -o $@
	@ echo "$(GREEN)[ OK ]$(END) $(CYAN)${<:.s=.o}$(END)"

clean:
	$(HIDE) $(RM) $(OBJS_PATH)
	@ echo "$(PURPLE)\n        *** Clean objects ***\n$(END)"

fclean: clean
	$(HIDE) $(RM) $(NAME)
	@ echo "$(RED)\n        *** Remove $(NAME) ***\n$(END)"

re: fclean all

.PHONY: all clean fclean re
