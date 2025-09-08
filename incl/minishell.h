/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 17:43:29 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/08 16:08:54 by imeulema         ###   ########.fr       */
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
	SPACE,
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
	struct s_ast	*root;
	t_token			**tokens;
	char			**envp;
	char			**paths;
	char			*cmd;
	int				exit_status;
	int				state;
}	t_shell;

typedef struct s_ast
{
	struct s_ast	**children;
	struct s_ast	*root;
	t_node_type		type;
	t_shell			*data;
	t_cmd			cmd;
	char			*file;
	int				exit_status;
}	t_ast;

/* GLOBAL VARIABLE */
extern volatile sig_atomic_t	g_signal_received;

/* FUNCTIONS */

/* MacOS compilation solution */
extern void	rl_replace_line(const char *str, int i);

/* Cleaning functions */
void		clean_ast(t_ast *ast);
void		clean_exit(t_ast *node, int status);
void		cleanup(t_ast *node);
void		free_str_array(char **arr);
int			clean_data(t_shell data);

/* Error handling functions */
void		getcwd_error(char **envp);
void		malloc_error(t_ast *node, t_shell *data, t_token **tl);

/* Exec functions */
int			exec_ast(t_ast *node);

/* General utils functions */
char		*sf_strdup(const char *s, t_token **tokens, char **args,
				t_shell *data);
void		print_token_list(t_token **token_list);
void		print_tree(t_ast *ast);

/* Parsing functions */
t_node_type	convert_types(t_token_type type);
t_token		**tokenize_command(t_shell *data, char *command);
t_token		*cat_word(t_shell *data, t_token *current, t_token *prev,
				t_token *next);
t_token		*get_token_at_index(t_token **tokens, int index);
t_token		*handle_token_type(t_shell *data, char	**command,
				t_token_type type, t_token *new_token);
t_token		*tokenize_operator(t_shell *data, t_token **tl, char **command,
				t_token *token);
t_token		*tokenize_parenthesis(t_shell *data, t_token **tl, char **command,
				t_token *token);
t_token		*tokenize_quote(t_shell *data, t_token **tl, char **command,
				t_token *token);
t_token		*tokenize_redir(t_shell *data, t_token **tl, char **command,
				t_token *token);
t_token		*tokenize_space(char **command, t_token *token);
t_token		*tokenize_special_character(t_shell *data, t_token **tl,
				char **command, t_token *token);
t_token		*tokenize_word(t_shell *data, t_token **tl, char **command,
				t_token *token);
t_shell		init_shell_data(char **envp);
t_ast		**extract_redirs(t_shell *data, char **args, int start, int end);
t_ast		*create_ast(t_token **token_list, t_shell *data);
t_ast		*create_cmd_node(char **args, t_token **tokens, t_shell *data);
t_ast		*create_operator_node(t_node_type type, t_ast *left, t_ast *right,
				t_shell *data);
t_ast		*create_redir_node(t_node_type type, char *file, t_shell *data);
t_ast		*create_subshell_node(t_ast *child, t_shell *data);
t_ast		*parse(char *command, t_shell *data);
t_ast		*parse_command(t_token **tokens, int start, int end, t_shell *data);
t_ast		*parse_command_line(t_token **tokens, int start, int end,
				t_shell *data);
t_ast		*parse_operator(t_shell *data, int start, int end, int op_pos);
char		**copy_env(char **envp);
char		**create_env_cpy(void);
char		**ft_split_paths(const char *s, char c);
void		env_cpy_malloc_error(char **env_cpy, int i);
void		expander(t_token **token_list, t_shell *data);
void		free_tokens(t_token **token_list);
void		get_paths(t_shell *data);
void		get_trunc_cwd(char cwd[256], t_shell data);
void		set_root_node(t_ast *ast, t_ast *root);
void		set_shlvl_malloc_error(char **env_cpy, int i);
int			count_tokens(t_token **token_list);
int			count_redirs(t_token **tokens, int start, int end);
int			find_lowest_precedence_op(t_token **tokens, int i, int end);
int			find_matching_parentheses(t_token **tokens, int open_pos, int end);
int			ft_wordlen(char *content);
int			is_command_char(char c);
int			is_env_var(char c);
int			is_logical_operator(char c);
int			is_parenthesis(char c);
int			is_redir_token(t_token *token);
int			is_redirection(char c);
int			is_space(char c);
int			is_special_character(char c);
int			is_quote(char c);
int			matching_parentheses(t_token **tokens, int start, int end);
int			valid_parentheses(t_shell *data, t_token **token_list);
int			valid_syntax(t_shell *data, t_token **token_list);

/* Signal handling functions */
void		init_execution_signals(char *command, t_shell data);
void		init_interactive_signals(t_shell data);

#endif
