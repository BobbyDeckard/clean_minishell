/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:36:29 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/22 17:11:41 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	close_all_redirs(t_ast *node);

void	clean_tokens(t_token **list)
{
	t_token	*current;
	t_token	*next;

	if (!list || !*list)
		return ;
	current = *list;
	while (current)
	{
		next = current->next;
		if (current->content)
			free(current->content);
		free(current);
		current = next;
	}
	free(list);
}

int	clean_shell(t_shell *shell)
{
	int	i;

	if (shell->command)
	{
		free(shell->command);
		shell->command = NULL;
	}
	if (shell->envp)
	{
		i = -1;
		while (shell->envp[++i])
			free(shell->envp[i]);
		free(shell->envp);
		shell->envp = NULL;
	}
	if (shell->paths)
	{
		i = -1;
		while (shell->paths[++i])
			free(shell->paths[i]);
		free(shell->paths);
		shell->paths = NULL;
	}
	return (shell->exit_status);
}

static void	clean_args(char **args)
{
	int	i;

	i = -1;
	while (args[++i])
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
		{
			clean_ast(ast->children[i]);
			ast->children[i] = NULL;
		}
		free(ast->children);
		ast->children = NULL;
	}
	if (ast->cmd.args)
	{
		clean_args(ast->cmd.args);
		free(ast->cmd.args);
		ast->cmd.args = NULL;
	}
	if (ast->cmd.path)
	{
		free(ast->cmd.path);
		ast->cmd.path = NULL;
	}
	if (ast->rdr.args)
	{
		clean_args(ast->rdr.args);
		free(ast->rdr.args);
		ast->rdr.args = NULL;
	}
	if (ast->rdr.file)
	{
		free(ast->rdr.file);
		ast->rdr.file = NULL;
	}
	free(ast);
}

void	cleanup(t_ast *node)
{
	if (!node)
		return ;
	clean_shell(node->shell);
	clean_ast(node->root);
}
