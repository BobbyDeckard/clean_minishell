/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 14:08:17 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 14:21:23 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	cleanup(t_ast *node);
void	clean_ast(t_ast *root);
void	clean_tokens(t_token **tokens);

void	malloc_error(t_ast *ast, t_shell *shell, t_token **tokens)
{
	perror("malloc");
	if (shell)
	{
		clean_shell(shell);
		printf("malloc_error() successfully cleaned shell\n");
	}
	if (tokens)
	{
		clean_tokens(tokens);
		printf("malloc_error() successfully cleaned token list\n");
	}
	if (ast)
	{
		clean_ast(ast->root);
		printf("malloc_error() successfully cleaned ast\n");
	}
	exit(1);
}

int	fork_error(void)
{
	perror("fork");
	return (1);
}

// Function can only be called from within child processes, hence the exit()
void	dup2_error(t_ast *node)
{
	perror("dup2");
	cleanup(node);
	exit(1);
}

void	getcwd_error(t_ast *node)
{
	perror("getcwd");
	cleanup(node);
	exit(1);
}
