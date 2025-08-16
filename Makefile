NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
SRC = main.c helper_functions.c
OBJ = $(SRC:.c=.o)

all : $(NAME)

%o: %c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean :
	rm -rf $(OBJ)

fclean :
	rm -rf $(NAME) $(OBJ)

re: fclean all

.PHONY = all clean fclean re