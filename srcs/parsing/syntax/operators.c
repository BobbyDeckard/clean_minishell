/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 11:05:00 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/14 11:37:49 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	check_op_prev(t_token *prev)
{
	t_t_type	type;

	while (prev && prev->type == WHITESPACE)
		prev = prev->prev;
	if (!prev)
		return (1);
	type = prev->type;
	if (type == AND || type == OR || type == PIPE)
		return (1);
	return (0);
}

static int	check_op_next(t_token *next)
{
	t_t_type	type;

	while (next && next->type == WHITESPACE)
		next = next->next;
	if (!next)
		return (1);
	type = next->type;
	if (type == AND || type == OR || type == PIPE)
		return (1);
	return (0);
}

int	check_operators(t_token **list)
{
	t_token		*current;

	current = *list;
	while (current)
	{
		if (current->type == AND || current->type == OR)
		{
			if (check_op_prev(current->prev))
				return (1);
			else if (check_op_next(current->next))
				return (1);
		}
		current = current->next;
	}
	return (0);
}
