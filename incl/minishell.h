/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 17:43:29 by imeulema          #+#    #+#             */
/*   Updated: 2025/08/28 11:00:44 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* LIBRARIES */
# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include "../libft/libft.h"

/* ENUMS */
typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND_IF,
	NODE_OR_IF,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_REDIR_APPEND,
	NODE_HEREDOC,
	NODE_SUBSHELL
}	t_node_type;

typedef enum e_shell_state
{
	INTERACTIVE,
	EXECUTING,
	HEREDOC_MODE
}	t_shell_state;

typedef enum e_token_type
{
	DEFAULT,
	WORD,
	ASSIGNMENT,
	QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	REDIR,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC,
	OPERATOR,
	PIPE,
	AND,
	OR,
	SEMICOLON,
	PARENTHESIS,
	PAREN_OPEN,
	PAREN_CLOSE,
	SPECIAL_CHARACTER,
	ENV_VAR,
	EXIT_STATUS,
	ESCAPE,
	COMMENT,
	T_NEWLINE,
	T_EOF,
	UNKNOWN
}	t_token_type;

/* STRUCTS */
typedef struct s_token
{
	struct s_token	*next;
	struct s_token	*previous;
	t_token_type	type;
	char			*content;
	int				needs_expansion;
}	t_token;

typedef struct s_cmd
{
	char	**args;
	char	*path;
	int		fd_in;
	int		fd_out;
}	t_cmd;

typedef struct s_shell
{
	char	**envp;
	char	**paths;
	int		exit_status;
	int		state;
}	t_shell;

typedef struct s_ast
{
	struct s_ast	**children;
	struct s_ast	*root;
	t_node_type		type;
	t_shell			data;
	t_cmd			cmd;
	char			*file;
	int				exit_status;
}	t_ast;

/* GLOBAL VARIABLE */
extern volatile sig_atomic_t g_signal_received;

/* FUNCTIONS */

/* MacOS compilation solution */
extern void	rl_replace_line(const char *str, int i);

/* Cleaning functions */
void	clean_ast(t_ast *ast);
void	clean_exit(t_ast *node, int status);
void	cleanup(t_ast *node);
int		clean_data(t_shell data);

/* Error handling functions */
void	getcwd_error(char **envp);
void	malloc_error(t_ast *node);

/* Parsing functions */
t_shell	init_shell_data(char **envp);
t_ast	*parse_command(char *command, t_shell *data);
char	**copy_env(char **envp);
char	**create_env_cpy(void);
char	**ft_split_paths(const char *s, char c);
void	get_paths(t_shell *data);
void	get_trunc_cwd(char cwd[128], t_shell data);

/* Signal handling functions */
void	init_execution_signals(char *command, t_shell data);
void	init_interactive_signals(t_shell data);

#endif
