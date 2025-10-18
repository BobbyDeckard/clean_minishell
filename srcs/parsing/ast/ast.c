/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:15:22 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/16 07:47:02 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_ast	*parse_command(t_shell *shell, t_token **list, int start, int end);
t_ast	*parse_operator(t_shell *shell, int start, int end, int operator);
t_ast	*parse_parentheses(t_shell *shell, t_token **list, int start, int end);
int		find_op_precedence(t_token **list, int start, int end);
int		paren_pair(t_token **list, int start, int end);

static int	count_tokens(t_token **list)
{
	t_token	*current;
	int		count;

	count = 0;
	current = *list;
	while (current)
	{
		current = current->next;
		count++;
	}
	return (count);
}

static t_ast	*parse_non_op(t_shell *shell, t_token **list, int start, int end)
{
	t_token	*token;

	printf("In parse_list_portion() with start = %d and end = %d\n", start, end);
	token = get_token_at_index(list, start);
	while (token && token->type == WHITESPACE)
	{
		token = token->next;
		start++;
	}
	if (start < end && token && token->type == PAREN_OPEN)
		return (parse_parentheses(shell, list, start, end));
	return (parse_command(shell, list, start, end));
}

t_ast	*parse_sequence(t_shell *shell, t_token **list, int start, int end)
{
	int	operator;

	printf("In parse_sequence with start = %d and end = %d\n", start, end);
	if (start > end)
		return (NULL);
	else if (paren_pair(list, start, end))
			return (parse_sequence(shell, list, start + 1, end - 1));	// seems excessively speficic..?
	operator = find_op_precedence(list, start, end);
	printf("Found operator at position %d\n", operator);
	if (operator == -1)
		return (parse_non_op(shell, list, start, end));
	return (parse_operator(shell, start, end, operator));
}

t_ast	*create_ast(t_shell *shell, t_token **list)
{
	t_ast	*root;
	int		count;

	count = count_tokens(list);
//	printf("In create_ast(), counted %d tokens\n", count);
	root = parse_sequence(shell, list, 0, --count);
	return (root);
}
