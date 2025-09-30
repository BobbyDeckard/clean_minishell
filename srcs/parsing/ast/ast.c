/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 14:43:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/03 12:00:05 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

/*
t_ast	*parse_parentheses(t_token **tokens, int start, int end, t_shell *data)
{
	t_ast	*node;
	char	**redirs;
	int		closing;
	int		count;

	closing = find_matching_parentheses(tokens, start, end);
	redirs = find_trailing_redirs(tokens, closing, end, &count);
	if (closing > start && closing <= end)
		node = parse_command_line(tokens, start + 1, closing - 1, data);
}
*/

int	count_trailing_redirs(t_token *current, int start)
{
	int	count;

	count = 0;
	while (current)
	{
		if (!is_redir_token(current))
			break ;
		current = current->next;
		if (current->type != WORD)
			break ;
		count++;
		current = current->next;
	}
	return (count);
}

void	trailing_redir_error(t_ast *node, t_shell *data, t_token **tokens, t_redir *redirs)
{
	//	Solve the freeing of redirs (have a dedicated function)
	free(redirs);
	malloc_error(node , data, tokens);
}

t_node_type	convert_redir_type(t_token_type token_type)
{
	t_node_type	type;

	if (token_type == REDIR_IN)
		return (NODE_REDIR_IN);
	else if (token_type == REDIR_OUT)
		return (NODE_REDIR_OUT);
	else if (token_type == REDIR_APPEND)
		return (NODE_REDIR_APPEND);
	else if (token_type == HEREDOC)
		return (NODE_HEREDOC);
	return (-1);
}

t_redir	*find_trailing_redirs(t_token **tokens, int start, int count, t_shell *data)
{
	t_token	*current;
	t_redir	*redirs;
	int		i;

//	printf("In find_trailing_redirs()\n");
	current = get_token_at_index(tokens, start);
	if (!current)
		return (NULL);
//	printf("About to malloc redirs with count = %d\n", count);
	redirs = (t_redir *) malloc(count * sizeof(t_redir));
	if (!redirs)
		malloc_error(NULL, data, tokens);
	i = -1;
	while (++i < count)
	{
//		printf("current is of type: ");
//		print_token_type(current->type);
//		printf("\n");
		redirs[i].type = convert_redir_type(current->type);
		current = current->next;
//		printf("current has content: '%s'\n", current->content);
		redirs[i].file = ft_strdup(current->content);
		if (!redirs[i].file)
			trailing_redir_error(NULL, data, tokens, redirs);
	}
	return (redirs);
}

void	set_trailing_redirs(t_shell *data, t_ast *node, t_redir *redirs, int count)
{
	t_ast	**new;
	char	*file;
	int		tot;
	int		i;
	int		j;

	i = 0;
	while (node->children && node->children[i++])
		continue ;
//	printf("In set_trailing_redirs(), i = %d after iterating through children\n", i);
	tot = i + count;
//	printf("tot = %d (%d + %d)\n", tot, i, count);
	new = (t_ast **) malloc((tot + 1) * sizeof(t_ast *));
	if (!new)
		trailing_redir_error(node, data, data->tokens, redirs);
	i = -1;
	while (++i + count < tot)
		new[i] = node->children[i];
//	printf("i = %d after transferring children\n", i);
	i--;
	j = -1;
	while (++i < tot && ++j < count)
	{
//		printf("Creating new[%d]\n", i);
		file = ft_strdup(redirs[j].file);
		if (!file)
		{
			while (--i + count > tot)
				free(new[i]);
			free(new);
			trailing_redir_error(node, data, data->tokens, redirs);
		}
		new[i] = create_redir_node(redirs[j].type, file, data);
		if (!new[i])
		{
			while (--i + count > tot)
				free(new[i]);
			free(new);
			trailing_redir_error(node, data, data->tokens, redirs);
		}
	}
//	printf("Setting new[%d] to NULL\n", i);
	new[i] = NULL;
	if (node->children)
		free(node->children);
	node->children = new;
}

void	apply_redirs(t_shell *data, t_ast *node, t_redir *redirs, int count)
{
	int	i;

	if (node->type == NODE_CMD)
		set_trailing_redirs(data, node, redirs, count);
	else if (node->children)
	{
		i = -1;
		while (node->children[++i])
			apply_redirs(data, node->children[i], redirs, count);
	}
}

t_ast	*parse_parentheses(t_token **tokens, int start, int end, t_shell *data)
{
	t_token	*current;
	t_redir	*redirs;
	t_ast	*node;
	int		closing;
	int		count;

//	printf("\nIn parse_parentheses with start = %d and end = %d\n", start, end);
	closing = find_matching_parentheses(tokens, start, end);
//	printf("Closing parenthesis is token %d\n\n", closing);
	if (closing > start && closing <= end)
		node = parse_command_line(tokens, start + 1, closing - 1, data);
//	printf("\nCommand parsed, return to parse_parentheses\n");
	current = get_token_at_index(tokens, closing + 1);
	count = count_trailing_redirs(current, closing + 1);
	redirs = find_trailing_redirs(tokens, closing + 1, count, data);
	apply_redirs(data, node, redirs, count);
	closing = -1;
	while (++closing < count)
		free(redirs[closing].file);
	free(redirs);
//	printf("Exiting parse_parentheses\n\n");
	return (node);
}

static t_ast	*create_subshell(t_token **tokens, int start, int end,
t_shell *data)
{
	t_token	*current;
	t_redir	*redirs;
	t_ast	*node;
	int		closing;

//	printf("In create_subshell() with start = %d and end = %d\n", start, end);
	current = get_token_at_index(tokens, start);
//	if (current->content)
//		printf("Current's content: '%s'\n", current->content);
//	else
//	{
//		printf("Current has no content but is of type ");
//		print_token_type(current->type);
//		printf("\n");
//	}
	if (start < end && current && current->type == PAREN_OPEN)
		return (parse_parentheses(tokens, start, end, data));
	return (parse_command(tokens, start, end, data));
}

t_ast	*parse_command_line(t_token **tokens, int start, int end,
t_shell *data)
{
	t_node_type	type;
	int			op_pos;

//	printf("In parse_command_line() with start = %d and end = %d\n", start, end);
	if (start > end)
		return (NULL);
	if (matching_parentheses(tokens, start, end))
	{
//		printf("Found matching parentheses, recalling function with incremented start and decremented end\n\n");
		return (parse_command_line(tokens, ++start, --end, data));
	}
	op_pos = find_lowest_precedence_op(tokens, start, end);
//	printf("Operator position: %d\n\n", op_pos);
	if (op_pos == -1)
		return (create_subshell(tokens, start, end, data));
	return (parse_operator(data, start, end, op_pos));
}

t_ast	*create_ast(t_token **token_list, t_shell *data)
{
	t_ast	*root;
	int		tokens;

	tokens = count_tokens(token_list);
	root = parse_command_line(token_list, 0, --tokens, data);
	set_root_node(root, root);
	return (root);
}
