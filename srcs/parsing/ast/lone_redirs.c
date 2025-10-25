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

//	printf("In is_lone_redir() with start = %d and end = %d\n", start, end);
	current = get_token_at_index(list, start);
	if (!is_redir_token(current->type))
		return (0);
	current = current->next;
	start++;
	while (current && current->type == WHITESPACE && start <= end)
	{
//		printf("Skipping whitespaces, start = %d\n", start);
		current = current->next;
		start++;
	}
	while (current && is_redir_arg(current) && start <= end)
	{
//		printf("Iterating through redirs args, start = %d\n", start);
//		if (current->content)
//			printf("Current has content '%s'\n", current->content);
//		else
//			printf("Current has no content\n");
		current = current->next;
		start++;
	}
	while (current && current->type == WHITESPACE && start <= end)
	{
//		printf("Skipping whitespaces, start = %d\n", start);
		current = current->next;
		start++;
	}
	if (current && is_redir_token(current->type))
	{
//		printf("About to check lone redirs further down (start = %d, end = %d\n", start, end);
		return (is_lone_redir(list, start, end));
	}
	else if (current)
		return (0);
	return (1);
}

static void	alloc_redir_args(t_shell *shell, t_ast *node, int count)
{
//	printf("In alloc_redir_args() with count = %d\n", count);
	node->rdr.args = (char **) malloc(count * sizeof(char *));
	if (!node->rdr.args)
		malloc_error(shell->root, shell, shell->tokens);
	while (--count >= 0)
		node->rdr.args[count] = NULL;
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

//	printf("In make_arg()\n");
	str = NULL;
	if (current->content)
	{
//		printf("Redir arg has content: '%s'\n", current->content);
		len = ft_strlen(current->content) + 1;
		str = (char *) malloc(len * sizeof(char));
		if (!str)
			make_arg_error(shell, node);
		ft_strlcpy(str, current->content, len);
	}
	else if (current->type == SINGLE_QUOTE || current->type == DOUBLE_QUOTE)
	{
//		printf("Redir arg is quote\n");
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

//	printf("In parse_lone_redir()\n");
	current = get_token_at_index(shell->tokens, start);
//	printf("Fetched token at index %d\n", start);
	count = count_redir_args(current->next) + 1;
//	printf("Counted %d redir args\n", count);
	if (count == 1)
		return (node);
	alloc_redir_args(shell, node, count);
//	printf("Allocated redir args\n");
	i = -1;
	current = current->next;
	while (current && is_redir_arg(current) && start <= end)
	{
//		printf("In while loop, start = %d\n", start);
		node->rdr.args[++i] = make_arg(shell, node, current);
//		printf("Made arg[%d]: '%s'\n", i, node->rdr.args[i]);
		current = current->next;
		start++;
	}
	return (check_remaining_lone_redirs(shell, node, start, end));
}

static t_ast *check_remaining_lone_redirs(t_shell *shell, t_ast *node, int start, int end)
{
	t_token	*current;

//	printf("In check_remaining_lone_redirs()\n");
	if (start != end)
	{
		current = get_token_at_index(shell->tokens, start);
		if (!current)
			return (node);
		else if (is_redir_token(current->type))
		   current = current->next;
//		printf("Current before skipping whitespaces: '%s'\n", current->content);
		while (current && current->type == WHITESPACE && start++ <= end)
			current = current->next;
//		printf("Current after skipping whitespaces: '%s'\n", current->content);
		init_lone_redir_child(shell, node, convert_types(current->type));
		parse_lone_redir(shell, node->children[0], start, end);
	}
	return (node);
}

void	parse_redirs(t_shell *shell, t_ast *node, int start, int end);
int		count_redirs(t_token **list, int start, int end);

static t_ast	*create_solo_redir_node(t_shell *shell, int redirs)
{
	t_ast	*node;
	int		i;

	node = create_node(shell, NODE_SOLO_REDIR);
	if (!node)
		malloc_error(shell->root, shell, shell->tokens);
	set_root(shell, node);
	if (redirs == 1)
		return (node);
	node->children = (t_ast **) malloc(redirs * sizeof(t_ast *));
	if (!node->children)
		malloc_error(shell->root, shell, shell->tokens);
	i = -1;
	while (++i < redirs)
		node->children[i] = NULL;
	return (node);
}

t_ast	*parse_lone_redirs(t_shell *shell, t_token**list, int start, int end)
{
	t_ast	*node;
	int		count;

	count = count_redirs(list, start, end) + 1;
	node = create_solo_redir_node(shell, count);
	set_root(shell, node);
	parse_redirs(shell, node, start, end);
	return (node);
}

/*
//	We did not prepare an AST structure for multiple lone redirs following
//	each other.
//	The simpler approach seems to link them as a linked list through the
//	children pointer.
t_ast	*parse_lone_redirs(t_shell *shell, t_token **list, int start, int end)
{
	t_token	*current;
	t_ast	*node;

	printf("In parse_lone_redirs()\n");
	current = get_token_at_index(list, start);
	node = create_node(shell, convert_types(current->type));
	if (!node)
		malloc_error(shell->root, shell, shell->tokens);
	set_root(shell, node);
	return (parse_lone_redir(shell, node, start, end));
}
*/
