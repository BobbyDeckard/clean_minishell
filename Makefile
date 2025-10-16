# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/27 17:31:06 by imeulema          #+#    #+#              #
#    Updated: 2025/10/16 11:47:39 by imeulema         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wextra -Wall -Werror
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
			 $(PATH) \
			 $(SIGS) \
			 $(UTLS) \
			 $(DEBUG)
SRCS_PATH = srcs/
OBJS = $(patsubst $(SRCS_PATH)%.c, $(OBJS_PATH)%.o, $(SRCS))
OBJS_PATH = objs/

# SOURCES SUBDIRECTORIES

# PARSING
PARS = $(addprefix parsing/, $(PARS_FILES))
PARS_FILES = parsing.c \
			 $(AST) \
			 $(SHLL) \
			 $(SNTX) \
			 $(TKNS)
AST = $(addprefix ast/, $(AST_FILES))
AST_FILES = ast.c \
			ast_utils.c \
			commands.c \
			commands_utils.c \
			lone_redirs.c \
			operators.c \
			parentheses.c \
			pipes.c \
			redirs.c \
			redirs_utils.c
SHLL = $(addprefix shell/, $(SHLL_FILES))
SHLL_FILES = copy_env.c \
			 create_env.c \
			 env_errors.c \
			 shell.c
SNTX = $(addprefix syntax/, $(SNTX_FILES))
SNTX_FILES = operators.c \
			 parentheses.c \
			 parentheses_utils.c \
			 pipes.c \
			 quotes.c \
			 redirs.c
TKNS = $(addprefix tokenization/, $(TKNS_FILES))
TKNS_FILES = char_checks.c \
			 errors.c \
			 tokenization.c \
			 tokenize.c \
			 tokenize_operators.c \
			 tokenize_redirs.c \
			 wordlen.c

# PATHS
PATH = $(addprefix paths/, $(PATH_FILES))
PATH_FILES = ft_split_paths.c \
			 paths.c

# SIGNALS
SIGS = $(addprefix signals/, $(SIGS_FILES))
SIGS_FILES = setup_execution.c \
			 setup_interactive.c

# UTILS
UTLS = $(addprefix utils/, $(UTLS_FILES))
UTLS_FILES = cleanup.c \
			 errors.c \
			 read_command.c \
			 utils.c

# DEBUG
DEBUG = $(addprefix debug_files/, $(DEBUG_FILES))
DEBUG_FILES = print.c


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
	echo "$(GREEN)Libft compiled.$(RESET)"

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
