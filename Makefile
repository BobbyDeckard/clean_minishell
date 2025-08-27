# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/27 17:31:06 by imeulema          #+#    #+#              #
#    Updated: 2025/08/27 17:42:57 by imeulema         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CLFAGS = -Wextra -Wall -Werror
LFLAGS = -lreadline
RM = rm
RMFLAGS = -f
MAKE = make
UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)
	LFLAGS += -L /opt/homebrew/opt/readline/lib -I /opt/homebrew/opt/readline/include/readline
endif

# SOURCES
SRCS = $(addprefix $(SRCS_PATH), $(SRCS_FILES))
SRCS_FILES = main.c \
			 $(UTLS) \
			 $(PARS)
SRCS_PATH = srcs/
OBJS = $(patsubst $(SRCS_PATH)%.c, $(OBJS_PATH)%.o, $(SRCS))
OBJS_PATH = objs/

# SOURCES SUBDIRECTORIES
UTLS = $(addprefix general_utils/, $(UTLS_FILES))
UTLS_FILES = cleanup.c \
			 errors.c
PARS = $(addprefix parsing/, $(PARS_FILES))
PARS_FILES = create_env_cpy.c \
			 env_cpy.c \
			 env_errors.c \
			 ft_split_paths.c \
			 paths.c \
			 shell_data.c

# INCLUDES
HEADERS = incl/minishell.h
LIBFT = libft/libft.a

# TERMINAL COLOURS
RED = \033[0;31m
YELLOW = \033[0;33m
GREEN = \033[0;32m
RESET = \033[0m

# RULES
all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	echo "$(YELLOW)Compiling $(NAME).$(RESET)"
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) $(LIBFT) -o $(NAME)
	echo "$(GREEN)$(NAME) compiled!$(RESET)"

$(OBJS_PATH)%.o: $(SRCS_PATH)%.c $(HEADERS) $(LIBFT)
	echo "$(YELLOW)Compiling $<.$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	echo "$(YELLOW)Compiling libft.$(RESET)"
	$(MAKE) -C libft --no-print-directory

clean:
	$(RM) $(RMFLAGS) $(OBJS)
	$(MAKE) clean -C libft --no-print-directory

fclean: clean
	$(RM) $(RMFLAGS) $(NAME)
	$(MAKE) fclean -C libft --no-print-directory
	echo "$(GREEN)Full clean done.$(RESET)"

re: fclean all

.PHONY: all clean fclean re
