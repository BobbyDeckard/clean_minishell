/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:36:29 by imeulema          #+#    #+#             */
/*   Updated: 2025/08/27 20:44:32 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

//	This function supposes non-malloc'ed variables are set to NULL
//	and that cmd.path is malloc'ed (which I think to be the case, but need to verify).
void	clean_ast(t_ast *ast)
{
	int	i;

	if (ast->children)
	{
		i = -1;
		while (ast->children[++i])
			clean_ast(ast->children[i]);
	}
	if (ast->cmd.args)
	{
		i = -1;
		while (ast->cmd.args[++i])
			free(ast->cmd.args[i]);
		free(ast->cmd.args);
	}
	if (ast->cmd.path)
		free(ast->cmd.path);
	free(ast);
}

void	cleanup(t_ast *node)
{
	clean_data(node->data);
	clean_ast(node->root);
}

void	clean_exit(t_ast *node, int status)
{
	cleanup(node->root);
	exit(status);
}
