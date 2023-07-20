# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/22 13:18:59 by sbhatta           #+#    #+#              #
#    Updated: 2023/07/19 13:05:02 by ofadahun         ###   ########.fr        #
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
SRC_DIR = ./src
OBJ_DIR = ./obj

SRCS =  $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)

OBJS = $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) -o $@ $^ $(LDFLAG)

$(LIBFT):
		@make all -C $(LIBFT_PATH) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $< $(INCFLAGS)
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/*/%.c
	mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $< $(INCFLAGS)

clean:
	make -C $(LIBFT_PATH) clean
	rm -rf $(OBJ_DIR)
	
fclean: clean
	make -C $(LIBFT_PATH) fclean
	rm -f $(NAME)
	
re:	fclean all

.PHONY:	all clean fclean re bonus