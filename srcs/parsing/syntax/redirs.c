/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 11:17:09 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/14 11:25:02 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	is_redir_token(t_t_type type)
{
	if (type == REDIR_IN)
		return (1);
	else if (type == REDIR_OUT)
		return (1);
	else if (type == REDIR_APPEND)
		return (1);
	else if (type == HEREDOC)
		return (1);
	return (0);
}

static int	check_redir_prev(t_token *prev)
{
	t_t_type	type;

	while (prev && prev->type == WHITESPACE)
		prev = prev->prev;
	if (!prev)
		return (0);
	type = prev->type;
	if (type == PAREN_CLOSE || is_redir_token(type))
		return (1);
	return (0);
}

static int	check_redir_next(t_token *next)
{
	t_t_type	type;

	while (next && next->type == WHITESPACE)
		next = next->next;
	if (!next)
		return (1);
	type = next->type;
	if (type != WORD && type != DOUBLE_QUOTE && type != SINGLE_QUOTE)
		return (1);
	return (0);
}

int	check_redirs(t_token **list)
{
	t_token	*current;

	current = *list;
	while (current)
	{
		if (is_redir_token(current->type))
		{
			if (check_redir_prev(current->prev))
				return (1);
			else if (check_redir_next(current->next))
				return (1);
		}
		current = current->next;
	}
	return (0);
}
