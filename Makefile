# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/27 17:31:06 by imeulema          #+#    #+#              #
#    Updated: 2025/09/08 10:27:46 by imeulema         ###   ########.fr        #
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
			 $(PARS) \
			 $(UTLS) \
			 $(SIGS)
SRCS_PATH = srcs/
OBJS = $(patsubst $(SRCS_PATH)%.c, $(OBJS_PATH)%.o, $(SRCS))
OBJS_PATH = objs/

# SOURCES SUBDIRECTORIES
# PARSING
PARS = $(addprefix parsing/, $(PARS_FILES))
PARS_FILES = parsing.c \
			 $(AST) \
			 $(DATA) \
			 $(SNTX) \
			 $(TKNS)
AST = $(addprefix ast/, $(AST_FILES))
AST_FILES = ast.c \
			ast_utils.c \
			cmd.c \
			nodes.c \
			operators.c \
			parentheses.c \
			redirs.c
DATA = $(addprefix data/, $(DATA_FILES))
DATA_FILES = create_env_cpy.c \
			 env_cpy.c \
			 env_errors.c \
			 ft_split_paths.c \
			 paths.c \
			 shell_data.c
SNTX = $(addprefix syntax_checks/, $(SNTX_FILES))
SNTX_FILES = parentheses_check.c \
			 special_characters.c \
			 syntax_check.c
TKNS = $(addprefix tokens/, $(TKNS_FILES))
TKNS_FILES = expander.c \
			 token_types.c \
			 token_types2.c \
			 tokenize.c \
			 tokenize2.c \
			 tokens.c \
			 tokens_utils.c
# GENERAL UTILS
UTLS = $(addprefix general_utils/, $(UTLS_FILES))
UTLS_FILES = cleanup.c \
			 errors.c \
			 print.c \
			 safer_libft.c \
			 trunc_cwd.c
# SIGNALS
SIGS = $(addprefix signals/, $(SIGS_FILES))
SIGS_FILES = init.c

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

.SILENT:
