NAME		= irc
CC			= c++
FLAGS		= -Wall -Werror -Wextra -std=c++98
SRC			= main.cpp \
server.cpp \


OBJ			= $(SRC:.cpp=.o)
RM			= rm -f

all		: $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) -o $@ $^

.cpp.o	:
	$(CC) $(FLAGS) -o $@ -c $<

.PHONY	: all clean fclean re

clean	:
	$(RM) $(OBJ)

fclean	: clean
	$(RM) $(NAME)

re		: fclean
	make all