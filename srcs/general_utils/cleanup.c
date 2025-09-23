/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:36:29 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/12 15:10:57 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	clean_data(t_shell *data)
{
	int	i;

	if (data->cmd)
	{
		free(data->cmd);
		data->cmd = NULL;
	}
	if (data->envp)
	{
		i = -1;
		while (data->envp[++i])
			free(data->envp[i]);
		free(data->envp);
		data->envp = NULL;
	}
	if (data->paths)
	{
		i = -1;
		while (data->paths[++i])
			free(data->paths[i]);
		free(data->paths);
		data->paths = NULL;
	}
	return (data->exit_status);
}

//	This function supposes non-malloc'ed variables are set to NULL
//	and that cmd.path is malloc'ed (which I think to be the case,
//	but need to verify).
void	clean_ast(t_ast *ast)
{
	int	i;

//	printf("Begining clean_ast()\n");
	if (ast->children)
	{
		i = -1;
		while (ast->children[++i])
			clean_ast(ast->children[i]);
		free(ast->children);
	}
//	else
//		printf("Found no children\n");
	if (ast->cmd.args)
	{
//		printf("Found cmd.args, cmd.arg_count = %d\n", ast->cmd.arg_count);
		i = -1;
		while (++i < ast->cmd.arg_count)
		{
			if (ast->cmd.args[i])
			{
//				printf("Freeing ast->cmd.args[%d] (%d of %d)\n", i, i + 1, ast->cmd.arg_count);
				free(ast->cmd.args[i]);
			}
//			else
//				printf("ast->cmd.args[%d] is empty\n", i);
		}
		free(ast->cmd.args);
		free(ast->cmd.exp);
	}
//	else
//		printf("Found no cmd.args\n");
	if (ast->cmd.path)
		free(ast->cmd.path);
//	else
//		printf("Found no cmd.path\n");
	if (ast->file)
		free(ast->file);
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
