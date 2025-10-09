/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:44:21 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/03 13:26:55 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

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

static int	check_matching_par(t_token *current, t_token *last_close, int end,
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

static int	check_paren_syntax(t_token *current, t_token *last_close)
{
	while (current)
	{
		if (current->type == PAREN_OPEN && current->previous
			&& !is_logical_operator(current->previous))
			return (0);
		else if (current->type == PAREN_CLOSE && current->next
			&& !is_logical_operator(current->next))
			return (0);
		if (current == last_close)
			break ;
		current = current->next;
	}
	return (1);
}

int	check_parentheses(t_token **tokens)
{
	t_token	*last_close;

	last_close = get_last_close(tokens);
	if (!check_matching_par(*tokens, last_close, 0, 0))
		return (0);
	else if (!check_paren_syntax(*tokens, last_close))
		return (0);
	return (1);
}
