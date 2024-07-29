# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: stobin <stobin@student.42berlin.de>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/27 22:41:59 by stobin            #+#    #+#              #
#    Updated: 2024/07/27 22:41:59 by stobin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -g -Wall -Wextra -Werror -Ilibft
LDFLAGS = -lreadline -lncurses
NAME = minishell

LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a

SRCS = minishell.c lexer.c table.c hash.c load_envv.c parens_stack.c parser.c \
		pathstrings.c pipes.c error_handling.c string_handling.c utils.c \
    	unb_vars.c unb_echo.c unb_export.c unb_cd.c 
OBJS = $(SRCS:.c=.o)

all: $(NAME)
	@if [ -f $(LOGO) ]; then \
		bash $(LOGO); \
	fi

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LDFLAGS)

$(LIBFT):
	$(MAKE) -C $(dir $@)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re