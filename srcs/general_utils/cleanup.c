/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:36:29 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/30 17:14:48 by imeulema         ###   ########.fr       */
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

	close_all_redirs(ast);
	if (ast->children)
	{
		i = -1;
		while (ast->children[++i])
			clean_ast(ast->children[i]);
		free(ast->children);
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
