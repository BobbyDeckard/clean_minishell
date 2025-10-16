/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lone_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 07:50:48 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/16 11:44:57 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static t_ast *check_remaining_lone_redirs(t_shell *shell, t_ast *node, int start, int end);
t_n_type	convert_types(t_t_type type);
t_ast		*create_node(t_shell *shell, t_n_type type);
int			count_redir_args(t_token *current);
int			is_redir_arg(t_token *token);
int			is_redir_token(t_t_type type);

int	is_lone_redir(t_token **list, int start, int end)
{
	t_token	*current;

	current = get_token_at_index(list, start);
	if (!is_redir_token(current->type))
		return (0);
	while (current && (is_redir_token(current->type) || is_redir_arg(current)) && start <= end)
	{
		current = current->next;
		start++;
	}
	while (current && current->type == WHITESPACE && start <= end)
	{
		current = current->next;
		start++;
	}
	if (current && is_redir_token(current->type))
		return (is_lone_redir(list, start, end));
	else if (current)
		return (0);
	return (1);
}

static void	alloc_redir_args(t_shell *shell, t_ast *node, int count)
{
	node->rdr.args = (char **) malloc(count * sizeof(char *));
	if (!node->rdr.args)
		malloc_error(shell->root, shell, shell->tokens);
	while (count-- >= 0)
		node->rdr.args = NULL;
}

static void	make_arg_error(t_shell *shell, t_ast *node)
{
	if (!node->rdr.args[0])
		free(node->rdr.args);
	malloc_error(shell->root, shell, shell->tokens);
}

static char	*make_arg(t_shell *shell, t_ast *node, t_token *current)
{
	char	*str;
	int		len;

	str = NULL;
	if (current->content)
	{
		len = ft_strlen(current->content) + 1;
		str = (char *) malloc(len * sizeof(char));
		if (!str)
			make_arg_error(shell, node);
		ft_strlcpy(str, current->content, len);
	}
	else if (current->type == SINGLE_QUOTE || current->type == DOUBLE_QUOTE)
	{
		str = (char *) malloc(2 * sizeof(char));
		if (!str)
			make_arg_error(shell, node);
		if (current->type == SINGLE_QUOTE)
			ft_strlcpy(str, "'", 2);
		else
			ft_strlcpy(str, "\"", 2);
	}
	return (str);
}

static void	init_lone_redir_child(t_shell *shell, t_ast *node, t_n_type type)
{
	node->children = (t_ast **) malloc(2 * sizeof(t_ast *));
	if (!node->children)
		malloc_error(shell->root, shell, shell->tokens);
	node->children[0] = create_node(shell, type);
	node->children[1] = NULL;
}

static t_ast	*parse_lone_redir(t_shell *shell, t_ast *node, int start, int end)
{
	t_token	*current;
	int		count;
	int		i;

	current = get_token_at_index(shell->tokens, start);
	count = count_redir_args(current) + 1;
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

static t_ast *check_remaining_lone_redirs(t_shell *shell, t_ast *node, int start, int end)
{
	t_token	*current;

	if (start != end)
	{
		current = get_token_at_index(shell->tokens, start);
		while (current && current->type == WHITESPACE && start++ <= end)
			current = current->next;
		init_lone_redir_child(shell, node, convert_types(current->type));
		parse_lone_redir(shell, node->children[0], start, end);
	}
	return (node);
}

//	We did not prepare an AST structure for multiple lone redirs following
//	each other.
//	The simpler approach seems to link them as a linked list through the
//	children pointer.
t_ast	*parse_lone_redirs(t_shell *shell, t_token **list, int start, int end)
{
	t_token	*current;
	t_ast	*node;

	current = get_token_at_index(list, start);
	node = create_node(shell, convert_types(current->type));
	if (!node)
		malloc_error(shell->root, shell, shell->tokens);
	set_root(shell, node);
	return (parse_lone_redir(shell, node, start, end));
}
