/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:47:16 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/28 16:47:17 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	is_logop(t_t_type type)
{
	if (type == AND || type == OR)
		return (1);
	return (0);
}

static int	is_paren(t_t_type type)
{
	if (type == PAREN_OPEN || type == PAREN_CLOSE)
		return (1);
	return (0);
}

static int	is_command_token(t_t_type type)
{
	if (type == WORD)
		return (1);
	else if (is_redir_token(type))
		return (1);
	else if (type == DOUBLE_QUOTE || type == SINGLE_QUOTE)
		return (1);
	else if (type == WHITESPACE)
		return (1);
	return (0);
}

static int	count_commands(t_token **list, int start)
{
	t_token	*current;
	int		count;

	current = get_token_at_index(list, start);
	if (!current)
		return (0);
	count = 1;
	while (current)
	{
		if (current->type == PIPE)
			count++;
		else if (is_logop(current->type) || is_paren(current->type))
			return (count);
		current = current->next;
	}
	return (count);
}
