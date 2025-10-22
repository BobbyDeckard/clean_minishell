# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/27 17:31:06 by imeulema          #+#    #+#              #
#    Updated: 2025/10/22 15:02:32 by imeulema         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wextra -Wall -Werror -g
LFLAGS = -lreadline
RM = rm
RMFLAGS = -f
MAKE = make
UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)
	LFLAGS += -L /opt/homebrew/opt/readline/lib -I /opt/homebrew/opt/readline/include/readline
endif

# PATHS
LIBFT_PATH = libft/
OBJS_PATH = objs/
SRCS_PATH = srcs/

# SOURCES
SRCS = $(addprefix $(SRCS_PATH), $(SRCS_FILES))
SRCS_FILES = main.c \
			 $(DEBUG) \
			 $(EXEC) \
			 $(PARS) \
			 $(PTHS) \
			 $(SIGS) \
			 $(UTLS)
OBJS = $(patsubst $(SRCS_PATH)%.c, $(OBJS_PATH)%.o, $(SRCS))

# SOURCES SUBDIRECTORIES

# EXEC
EXEC = $(addprefix exec/, $(EXEC_FILES))
EXEC_FILES = exec.c \
			 exec2.c \
			 $(BLTN) \
			 $(XPDR) \
			 $(HRDC) \
			 $(PIPE) \
			 $(RDRS) \
			 $(XUTL)

BLTN = $(addprefix builtins/, $(BLTN_FILES))
BLTN_FILES = builtins.c \
			 cd.c \
			 cd_utils.c \
			 echo.c \
			 exit.c \
			 export.c \
			 export_expander.c \
			 export_utils.c \
			 export_utils2.c \
			 export_var.c \
			 unset.c \
			 unset_utils.c

XPDR = $(addprefix expander/, $(XPDR_FILES))
XPDR_FILES = cat_words.c \
			 double_quotes.c \
			 expander.c \
			 new_expander.c \
			 quotes.c \
			 spaces.c \
			 utils.c

HRDC = $(addprefix heredoc/, $(HRDC_FILES))
HRDC_FILES = heredoc.c \
			 heredoc_expander.c \
			 heredoc_expander_utils.c \
			 heredoc_parser.c \
			 heredoc_parser_utils.c \
			 heredoc_utils.c

PIPE = $(addprefix pipe/, $(PIPE_FILES))
PIPE_FILES = link_pipes.c \
			 pipe.c \
			 pipe_utils.c \
			 pipe_utils2.c

RDRS = $(addprefix redirs/, $(RDRS_FILES))
RDRS_FILES = redir_double_expander.c \
			 redir_expander.c \
			 redir_expander_utils.c \
			 redirs.c \
			 redirs_utils.c

XUTL = $(addprefix utils/, $(XUTL_FILES))
XUTL_FILES = cmd_path.c \
			 envp_transmission.c \
			 utils.c

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
			command_scope.c \
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
			 tokenization.c \
			 tokenize.c \
			 tokenize_operators.c \
			 tokenize_redirs.c \
			 wordlen.c

# PATHS
PTHS = $(addprefix paths/, $(PTHS_FILES))
PTHS_FILES = ft_split_paths.c \
			 paths.c

# SIGNALS
SIGS = $(addprefix signals/, $(SIGS_FILES))
SIGS_FILES = setup_execution.c \
			 setup_heredoc.c \
			 setup_interactive.c \

# UTILS
UTLS = $(addprefix utils/, $(UTLS_FILES))
UTLS_FILES = char_arr.c \
			 cleanup.c \
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
	$(MAKE) -C $(LIBFT_PATH) --no-print-directory

clean:
	$(RM) $(RMFLAGS) $(OBJS)
	$(MAKE) clean -C $(LIBFT_PATH) --no-print-directory

fclean: clean
	$(RM) $(RMFLAGS) $(NAME)
	$(MAKE) fclean -C $(LIBFT_PATH) --no-print-directory
	echo "$(GREEN)Full clean done.$(RESET)"

re: fclean all

.PHONY: all clean fclean re

.SILENT:
