/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:22:30 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 17:49:40 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_ast	*create_node(t_shell *shell, t_n_type type);
t_ast	*parse_command(t_shell *shell, t_token **list, int start, int end);
void	clean_ast(t_ast *root);
void	cleanup(t_ast *node);
int		is_redir_token(t_t_type type);

static t_ast	*create_pipe_node(t_shell *shell, int count)
{
	t_ast	*node;
	int		i;

	node = create_node(shell, NODE_PIPE);
	if (!node)
		malloc_error(shell->root, shell, shell->tokens);
	set_root(shell, node);
	node->children = (t_ast **) malloc(count * sizeof(t_ast *));
	if (!node->children)
		malloc_error(shell->root, shell, shell->tokens);
	i = -1;
	while (++i < count)
		node->children[i] = NULL;
	return (node);
}

static int	parse_pipe_command(t_shell *shell, t_ast *node, int i, int start)
{
	t_token	*current;
	int		end;

	current = get_token_at_index(shell->tokens, start);
	if (!current)
		return (-1);
	else if (current->type == PIPE)
	{
		current = current->next;
		start++;
	}
	end = start;
	while (current && is_command_token(current->type))
	{
		current = current->next;
		end++;
	}
	node->children[i] = parse_command(shell, shell->tokens, start, end);
	while (current && !is_command_token(current->type))
	{
		current = current->next;
		end++;
	}
	return (end - 1);
}

static void	parse_pipe_error(t_shell *shell, t_ast *node)
{
	if (!node->children[0])
		free(node->children);
	clean_ast(node);
	ft_putstr_fd("Unexpected error while parsing pipe\n", 2);
	cleanup(shell->root);
}

t_ast	*parse_pipe(t_shell *shell, t_token **list, int start, int end)
{
	t_ast	*node;
	int		count;
	int		i;

	count = count_commands(list, start) + 1;
	if (!count)
		return (NULL);
	node = create_pipe_node(shell, count);
	set_root(shell, node);
	i = -1;
	while (++i + 1 < count && start <= end)
	{
		start = parse_pipe_command(shell, node, i, start);
		if (start == -1)
			parse_pipe_error(shell, node);
	}
	return (node);
}
