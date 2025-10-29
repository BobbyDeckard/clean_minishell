/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 11:25:20 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/14 11:42:27 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_token	*get_last_close(t_token **list);
int		paren_open(t_token **list);

static int	check_match(t_token *current, t_token *last_close, int end,
int open)
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

static int	check_paren_prev(t_token *prev)
{
	t_t_type	type;

	while (prev && prev->type == WHITESPACE)
		prev = prev->prev;
	if (!prev)
		return (0);
	type = prev->type;
	if (type != AND && type != OR && type != PAREN_OPEN)
		return (1);
	return (0);
}

static int	check_paren_next(t_token *next)
{
	t_t_type	type;

	while (next && next->type == WHITESPACE)
		next = next->next;
	if (!next)
		return (0);
	type = next->type;
	if (type != AND && type != OR && type != PAREN_CLOSE)
		return (1);
	return (0);
}

static int	check_paren_syntax(t_token *current)
{
	while (current)
	{
		if (current->type == PAREN_OPEN && check_paren_prev(current->prev))
			return (1);
		else if (current->type == PAREN_CLOSE
			&& check_paren_next(current->next))
			return (1);
		current = current->next;
	}
	return (0);
}

int	check_parentheses(t_token **list)
{
	t_token	*last_close;

	last_close = get_last_close(list);
	if (paren_open(list) && !last_close)
		return (1);
	else if (!last_close)
		return (0);
	else if (!check_match(*list, last_close, 0, 0))
		return (1);
	else if (check_paren_syntax(*list))
		return (1);
	return (0);
}
