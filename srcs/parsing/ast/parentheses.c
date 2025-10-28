/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:33:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 17:48:00 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_ast	*parse_sequence(t_shell *shell, t_token **list, int start, int end);

static int	check_between_paren(t_token **list, int start, int end)
{
	t_token	*current;
	int		lvl;

	lvl = 0;
	while (start <= end)
	{
		current = get_token_at_index(list, start);
		if (!current)
			break ;
		else if (current->type == PAREN_OPEN)
			lvl++;
		else if (current->type == PAREN_CLOSE)
		{
			lvl--;
			if (!lvl && start != end)
				return (0);
		}
		start++;
	}
	return (lvl == 0);
}

int	paren_pair(t_token **list, int start, int end)
{
	t_token	*current;

	current = get_token_at_index(list, start);
	if (!current || current->type != PAREN_OPEN)
		return (0);
	current = get_token_at_index(list, end);
	if (!current || current->type != PAREN_CLOSE)
		return (0);
	return (check_between_paren(list, start, end));
}

static int	find_matching_paren(t_token **list, int start, int end)
{
	t_token	*current;
	int		lvl;
	int		i;

	current = get_token_at_index(list, start);
	if (!current || current->type != PAREN_OPEN)
		return (-1);
	lvl = 1;
	i = start;
	while (++i <= end)
	{
		current = get_token_at_index(list, i);
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

t_ast	*parse_parentheses(t_shell *shell, t_token **list, int start, int end)
{
	t_ast	*node;
	int		closing;

	node = NULL;
	closing = find_matching_paren(list, start, end);
	if (closing > start && closing <= end)
		node = parse_sequence(shell, list, ++start, --closing);
	return (node);
}
