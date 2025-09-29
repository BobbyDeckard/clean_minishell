/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:44:21 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/08 12:20:00 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	check_token(t_token **stack, t_token *current, int *top)
{
	if (current->type == PAREN_OPEN)
	{
		if (current->next && current->next->type == PAREN_CLOSE)
			return (0);
		stack[++(*top)] = current;
	}
	else if (current->type == PAREN_CLOSE)
	{
		if (*top < 0)
			return (0);
		(*top)--;
	}
	return (1);
}

int	valid_parentheses(t_shell *data, t_token **token_list)
{
	t_token	**stack;
	t_token	*current;
	t_token	*temp;
	int		top;

	stack = (t_token **) malloc(count_tokens(token_list) * sizeof(t_token));
	if (!stack)
		malloc_error(NULL, data, token_list);
	temp = *token_list;
	while (temp)
		temp = temp->next;
	top = -1;
	current = *token_list;
	while (current && check_token(stack, current, &top))
		current = current->next;
	if (top >= 0)
		return (free(stack), 0);
	return (free(stack), 1);
}

static t_token	*get_last_close(t_token **tokens)
{
	t_token	*last_close;
	t_token	*current;

	current = *tokens;
	while (current)
	{
		if (current->type == PAREN_CLOSE)
			last_close = current;
		current = current->next;
	}
	return (last_close);
}

static int	paren_check_body(t_token *current, t_token *last_close, int end, int open)
{
	int	start;

	start = 1;
	while (current)
	{
		if (current->type == PAREN_CLOSE && start)
			return (0);
		else if (current->type == PAREN_CLOSE)
		{
			if (current == last_close)
				end = 1;
			open--;
		}
		else if (current->type == PAREN_OPEN && end)
			return (0);
		else if (current->type == PAREN_OPEN)
		{
			start = 0;
			open++;
		}
		current = current->next;
	}
	return (open == 0);
}

int	check_parentheses(t_shell *data, t_token **tokens)
{
	t_token	*last_close;

	last_close = get_last_close(tokens);
	return (paren_check_body(*tokens, last_close, 0, 0));
}
