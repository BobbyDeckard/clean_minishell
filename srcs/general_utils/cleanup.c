/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:36:29 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/08 16:09:12 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	clean_data(t_shell data)
{
	int	i;

	if (data.cmd)
		free(data.cmd);
	if (data.envp)
	{
		i = -1;
		while (data.envp[++i])
			free(data.envp[i]);
		free(data.envp);
	}
	if (data.paths)
	{
		i = -1;
		while (data.paths[++i])
			free(data.paths[i]);
		free(data.paths);
	}
	return (data.exit_status);
}

//	This function supposes non-malloc'ed variables are set to NULL
//	and that cmd.path is malloc'ed (which I think to be the case,
//	but need to verify).
void	clean_ast(t_ast *ast)
{
	int	i;

	if (ast->children)
	{
		i = -1;
		while (ast->children[++i])
			clean_ast(ast->children[i]);
		free(ast->children);
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
	clean_data(*node->data);
	clean_ast(node->root);
}

void	clean_exit(t_ast *node, int status)
{
	cleanup(node->root);
	exit(status);
}

void	free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}
