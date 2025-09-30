/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:36:29 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/23 23:32:21 by imeulema         ###   ########.fr       */
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

static void	clean_args(char **args, int count)
{
	int	i;

	i = -1;
	while (++i < count)
	{
		if (args[i])
			free(args[i]);
	}
}

void	clean_ast(t_ast *ast)
{
	int	i;

//	printf("clean_ast() called on node of type: ");
//	print_node_type(ast->type);
//	printf("\n");
	if (ast->children)
	{
//		printf("Node has children, cleaning them now\n");
		i = -1;
		while (ast->children[++i])
			clean_ast(ast->children[i]);
//		printf("About to free children of node of type: ");
//		print_node_type(ast->type);
//		printf("\n");
		free(ast->children);
//		printf("Freed node's children\n");
	}
	if (ast->cmd.args)
	{
		clean_args(ast->cmd.args, ast->cmd.arg_count);
		free(ast->cmd.args);
		free(ast->cmd.exp);
	}
	if (ast->cmd.path)
		free(ast->cmd.path);
	if (ast->file)
	{
//		printf("About to free file for node of type ");
//		print_node_type(ast->type);
//		printf(", address: %p\n", ast->file);
		free(ast->file);
//		printf("File freed\n");
	}
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
