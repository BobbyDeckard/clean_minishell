/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 20:12:58 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/01 20:13:48 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	find_matching_parentheses(t_token **tokens, int open_pos, int end)
{
	t_token	*current;
	int		lvl;
	int		i;

	current = get_token_at_index(tokens, open_pos);
	if (!current || current->type != PAREN_OPEN)
		return (-1);
	lvl = 1;
	i = open_pos;
	while (++i <= end)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		else if (current->type == PAREN_OPEN)
			lvl++;
		else if (current->type == PAREN_CLOSE)
		{
			lvl--;
			if (!lvl)
				return (i);
		}
	}
	return (-1);
}

static int	check_between_parentheses(t_token **tokens, int start, int end)
{
	t_token	*current;
	int		lvl;

	lvl = 0;
	while (start <= end)
	{
		current = get_token_at_index(tokens, start);
		if (!current)
			break ;
		else if (current->type == PAREN_OPEN)
			lvl++;
		else if (current->type == PAREN_CLOSE)
		{
			lvl--;
			if (lvl == 0 && start != end)
				return (0);
		}
		start++;
	}
	return (lvl == 0);
}

int	matching_parentheses(t_token **tokens, int start, int end)
{
	t_token	*current;

	current = get_token_at_index(tokens, start);
	if (!current || current->type != PAREN_OPEN)
		return (0);
	current = get_token_at_index(tokens, end);
	if (!current || current->type != PAREN_CLOSE)
		return (0);
	return (check_between_parentheses(tokens, start, end));
}
