/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:51:41 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 14:21:09 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

typedef struct s_token	t_token;
typedef struct s_ast	t_ast;

typedef enum e_shell_state
{
	HEREDOC_MODE,
	INTERACTIVE,
	EXECUTING
}	t_shell_state;

typedef struct s_shell
{
	t_shell_state	state;
	t_token			**tokens;
	t_ast			*root;
	char			**envp;
	char			**paths;
	char			*command;
	int				exit_status;
}	t_shell;

int	clean_shell(t_shell *shell);

#endif
