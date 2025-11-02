/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lone_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 07:50:48 by imeulema          #+#    #+#             */
/*   Updated: 2025/11/02 15:14:06 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static t_ast	*check_remaining_lone_redirs(t_shell *shell, t_ast *node,
					int start, int end);
t_n_type		convert_types(t_t_type type);
t_ast			*create_node(t_shell *shell, t_n_type type);
char			*make_arg(t_shell *shell, t_ast *node, t_token *current);
void			alloc_redir_args(t_shell *shell, t_ast *node, int count);
void			parse_redirs(t_shell *shell, t_ast *node, int start, int end);
int				count_redir_args(t_token *current);
int				count_redirs(t_token **list, int start, int end);
int				is_redir_arg(t_token *token);
int				is_redir_token(t_t_type type);

static void	init_lone_redir_child(t_shell *shell, t_ast *node, t_n_type type)
{
	node->children = (t_ast **) malloc(2 * sizeof(t_ast *));
	if (!node->children)
		malloc_error(shell->root, shell, shell->tokens);
	node->children[0] = create_node(shell, type);
	node->children[1] = NULL;
}

static t_ast	*parse_lone_redir(t_shell *shell, t_ast *node, int start,
int end)
{
	t_token	*current;
	int		count;
	int		i;

	current = get_token_at_index(shell->tokens, start);
	count = count_redir_args(current->next) + 1;
	if (count == 1)
		return (node);
	alloc_redir_args(shell, node, count);
	i = -1;
	current = current->next;
	while (current && is_redir_arg(current) && start <= end)
	{
		node->rdr.args[++i] = make_arg(shell, node, current);
		current = current->next;
		start++;
	}
	return (check_remaining_lone_redirs(shell, node, start, end));
}

static t_ast	*check_remaining_lone_redirs(t_shell *shell, t_ast *node,
int start, int end)
{
	t_token	*current;

	if (start != end)
	{
		current = get_token_at_index(shell->tokens, start);
		if (!current)
			return (node);
		else if (is_redir_token(current->type))
			current = current->next;
		while (current && current->type == WHITESPACE && start++ <= end)
			current = current->next;
		init_lone_redir_child(shell, node, convert_types(current->type));
		parse_lone_redir(shell, node->children[0], start, end);
	}
	return (node);
}

static t_ast	*create_solo_redir_node(t_shell *shell, int redirs)
{
	t_ast	*node;

	node = create_node(shell, NODE_SOLO_REDIR);
	if (!node)
		malloc_error(shell->root, shell, shell->tokens);
	set_root(shell, node);
	if (redirs == 1)
		return (node);
	node->children = (t_ast **) malloc(redirs * sizeof(t_ast *));
	if (!node->children)
	{
		free(node);
		malloc_error(shell->root, shell, shell->tokens);
	}
	while (--redirs >= 0)
		node->children[redirs] = NULL;
	return (node);
}

t_ast	*parse_lone_redirs(t_shell *shell, t_token**list, int start, int end)
{
	t_ast	*node;
	int		count;

	count = count_redirs(list, start, end) + 1;
	node = create_solo_redir_node(shell, count);
	parse_redirs(shell, node, start, end);
	return (node);
}
