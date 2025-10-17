/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:36:29 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/15 10:17:42 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

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

//	close_all_redirs(ast);
	printf("In clean_ast(), given node has address: %p\n", ast);
	if (ast->children)
	{
		printf("Found node %p to have children\n", ast);
		i = -1;
		while (ast->children[++i])
			clean_ast(ast->children[i]);
		free(ast->children);
	}
	if (ast->cmd.args)
	{
		printf("Found node %p to have cmd.args\n", ast);
		clean_args(ast->cmd.args);
		free(ast->cmd.args);
	}
	if (ast->cmd.path)
	{
		printf("Found node %p to have cmd.path\n", ast);
		free(ast->cmd.path);
	}
	if (ast->rdr.args)
	{
		printf("Found node %p to have rdr.args\n", ast);
		clean_args(ast->rdr.args);
		free(ast->rdr.args);
	}
	if (ast->rdr.file)
	{
		printf("Found node %p to have rdr.file\n", ast);
		free(ast->rdr.file);
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
