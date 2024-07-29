CC=cc -g
FLAGS=-Wall -Werror -Wextra -Wuninitialized
NAME=minishell
EXT=-L libft -lreadline -lft
LOGO=~/42/ascii/logo.sh
SRCS=readline.c lexer.c table.c hash.c load_envv.c pipes.c utils.c unb_vars.c router.c \
	unb_echo.c unb_export.c unb_cd.c pathstrings.c minishell.c parser.c error_handling.c \
	string_handling.c parens_stack.c
OBJS= $(SRCS:.c=.o)
LIBFT=/libft/

all: deps $(NAME)
		@if [ -f $(LOGO) ]; then \
			bash $(LOGO); \
		fi 
deps:
		$(MAKE) -C libft

$(NAME): $(OBJS)
		$(CC) -o $(NAME) $(OBJS) $(EXT) $(FLAGS)

clean:
		rm -rf $(OBJS)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re deps
