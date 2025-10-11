/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:47:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/09 13:47:38 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	count_commands(t_token **tokens, int start)
{
	t_token	*current;
	int		count;

	current = get_token_at_index(tokens, start);
	if (!current)
		return (0);
	count = 1;
	while (current)
	{
		if (current->type == PIPE)
			count++;
		else if (current->type == AND || current->type == OR)
			return (count);
		current = current->next;
	}
	return (count);
}

static int	is_pipe_or_op(t_token *current)
{
	if (current->type == AND)
		return (1);
	else if (current->type == OR)
		return (1);
	else if (current->type == PIPE)
		return (1);
	return (0);
}

static t_ast	*parse_pipe_commands(t_shell *data, int start, int end,
int done)
{
	t_token	*current;
	int		cmd_end;

	current = get_token_at_index(data->tokens, start);
	if (!current)
		return (NULL);
	while (current && done > 0)
	{
		if (current->type == PIPE)
			done--;
		current = current->next;
		start++;
	}
	cmd_end = start - 1;
	while (current && !is_pipe_or_op(current) && cmd_end < end)
	{
		current = current->next;
		cmd_end++;
	}
	return (parse_command(data->tokens, start, cmd_end, data));
}

static t_ast	*create_pipe_node(t_ast **children, t_shell *data)
{
	t_ast	*node;

	node = create_ast_node(data, NODE_PIPE);
	if (!node)
		return (NULL);
	node->children = children;
	data->root = node;
	set_root_node(node, node);
	return (node);
}

t_ast	*parse_pipe(t_shell *data, int start, int end)
{
	t_ast	**children;
	int		commands;
	int		i;

	commands = count_commands(data->tokens, start);
	if (!commands)
		return (NULL);
	children = (t_ast **) malloc((commands + 1) * sizeof(t_ast *));
	if (!children)
		malloc_error(data->root, data, data->tokens);
	i = -1;
	while (++i < commands)
	{
		children[i] = parse_pipe_commands(data, start, end, i);
		if (!children[i])
		{
			while (--i >= 0)
				free(children[i]);
			free(children);
			malloc_error(data->root, data, data->tokens);
		}
	}
	children[i] = NULL;
	return (create_pipe_node(children, data));
}
