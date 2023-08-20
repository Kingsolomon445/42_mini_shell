# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/22 13:18:59 by sbhatta           #+#    #+#              #
#    Updated: 2023/08/18 18:07:51 by sbhatta          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wextra -Werror -Wall -g 
LDFLAG = -lreadline -L $(READLINE)/lib
INCFLAGS = -I $(READLINE)/include
AR = ar
CRS = crs
NAME = minishell
LIBFT_PATH = ./lib/libft
LIBFT = $(LIBFT_PATH)/libft.a
READLINE = $(HOME)/.brew/opt/readline
OBJ_DIR = ./obj

GREEN		= \033[1;32m
WHITE		= \033[0m
RED 		= \033[0;31m

SRCS =  ./src/builtins/builtin_utils.c \
		./src/builtins/cd_builtin.c \
		./src/builtins/echo_builtin.c \
		./src/builtins/env_builtin.c \
		./src/builtins/exit_builtin.c \
		./src/builtins/export_builtin.c \
		./src/builtins/print_builtin_error.c \
		./src/builtins/pwd_builtin.c \
		./src/builtins/unset_builtin.c \
		./src/parser/parse_commands_list.c \
		./src/parser/parse_commands.c \
		./src/parser/parse_heredoc.c \
		./src/parser/parse_redirection_list.c \
		./src/parser/parse_redirection_utils.c \
		./src/parser/parse_redirection.c \
		./src/parser/parse_shell_list.c \
		./src/parser/parse_shell_utils.c \
		./src/parser/parse_shell.c \
		./src/parser/parse_tokens_list.c \
		./src/parser/parse_tokens.c \
		./src/processes/init_process.c \
		./src/processes/run_process.c \
		./src/utils/builtin_utils.c \
		./src/utils/env_utils.c \
		./src/utils/free_utils.c \
		./src/utils/heredoc_utils.c \
		./src/utils/init_shell.c \
		./src/utils/parser_utils_four.c \
		./src/utils/parser_utils_one.c \
		./src/utils/parser_utils_three.c \
		./src/utils/parser_utils_two.c \
		./src/utils/shell_utils.c \
		./src/utils/signal_utils.c \
		./src/utils/utils_one.c \
		./src/utils/utils_two.c \
		./src/main.c

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) -o $@ $^ $(LDFLAG)
	@echo "$(GREEN)Minishell is ready 👍$(GREEN)"

$(LIBFT):
		@make all -C $(LIBFT_PATH) 

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $< $(INCFLAGS)

clean:
	make -C $(LIBFT_PATH) clean
	rm -rf $(OBJ_DIR)
	
fclean: clean
	make -C $(LIBFT_PATH) fclean
	rm -f $(NAME)
	@echo "$(GREEN)The project is cleaned! 🗑️$(GREEN)"
	
re:	fclean all

.PHONY:	all clean fclean re bonus