/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:35:22 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/29 14:04:44 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// Function can only be called from within child processes, hence the exit()
void	dup2_error(t_ast *node)
{
	perror("dup2");
	cleanup(node);
	exit(1);
}

int	fork_error(void)
{
	perror("fork");
	return (1);
}

void	getcwd_error(t_ast *node)
{
	perror("getcwd");
	cleanup(node);
	exit(1);
}

void	malloc_error(t_ast *node, t_shell *data, t_token **tl)
{
	perror("malloc");
	if (data)
		clean_data(data);
	if (tl)
		free_tokens(tl);
	if (node)
		clean_exit(node->root, 1);
	exit(1);
}
