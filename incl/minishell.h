/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 17:43:29 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 14:07:36 by imeulema         ###   ########.fr       */
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
# include "tokenization.h"
# include "shell.h"

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
	NODE_HEREDOC_EXP,
	NODE_SUBSHELL
}	t_node_type;

typedef struct s_cmd
{
	char	**args;
	char	*path;
	int		*cat;
	int		*exp;
	int		arg_count;
	int		fd_in;
	int		fd_out;
}	t_cmd;

typedef struct s_ast
{
	struct s_ast	**children;
	struct s_ast	*root;
	t_node_type		type;
	t_shell			*data;
	t_cmd			cmd;
	char			*file;
}	t_ast;

typedef struct s_redir
{
	t_node_type	type;
	char		*file;
}	t_redir;

/* GLOBAL VARIABLE */
extern volatile sig_atomic_t	g_signal_received;

/* FUNCTIONS */

/* MacOS compilation solution */
extern void	rl_replace_line(const char *str, int i);

/* General use functions */
void	malloc_error(t_ast *node, t_shell *shell, t_token **tokens);

#endif
