/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:16:09 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/16 07:45:52 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

typedef struct s_shell	t_shell;
typedef struct s_ast	t_ast;

typedef enum e_n_type
{
	NODE_REDIR_APPEND,
	NODE_REDIR_OUT,
	NODE_REDIR_IN,
	NODE_HEREDOC,
	NODE_PIPE,
	NODE_CMD,
	NODE_AND,
	NODE_OR
}	t_n_type;

typedef enum e_r_type
{
	RDR_HEREDOC,
	RDR_APPEND,
	RDR_OUT,
	RDR_IN
}	t_r_type;

typedef struct s_cmd
{
	char	**args;
	char	*path;
	int		fd_in;
	int		fd_out;
}	t_cmd;

typedef struct	s_rdr
{
	t_r_type	type;
	char		**args;
	char		*file;
	int			fd;
}	t_rdr;

typedef struct s_ast
{
	t_n_type	type;
	t_shell		*shell;
	t_ast		**children;
	t_ast		*root;
	t_cmd		cmd;
	t_rdr		rdr;
}	t_ast;

void	set_root(t_shell *shell, t_ast *node);

#endif
