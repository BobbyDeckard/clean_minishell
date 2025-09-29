/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:13:34 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/27 11:55:25 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	follows_quote(t_token *current)
{
	t_token	*next;
	t_token	*prev;
	int		quote;

	quote = 0;
	next = current->next;
	prev = current->previous;
	if (next->type == SINGLE_QUOTE || next->type == DOUBLE_QUOTE)
		quote++;
	if (prev->type == SINGLE_QUOTE || prev->type == DOUBLE_QUOTE)
		quote++;
	return (quote);
}

static int	valid_operator(t_token **token_list)
{
	t_token	*current;

	current = *token_list;
	while (current)
	{
		if (current->type == PIPE || current->type == AND
			|| current->type == OR)
		{
			if (!current->previous || !current->next)
				return (0);
			else if (follows_quote(current))
			{
				current = current->next;
				continue ;
			}
			else if (current->previous->type != WORD
				&& current->previous->type != PAREN_CLOSE)
				return (0);
			else if (current->next->type != WORD
				&& current->next->type != PAREN_OPEN)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

static int	valid_redir_target(t_token **token_list)
{
	t_token_type	type;
	t_token			*current;

	current = *token_list;
	while (current)
	{
		type = current->type;
		if (type == REDIR_OUT || type == REDIR_IN || type == REDIR_APPEND
			|| type == HEREDOC)
		{
			if (!current->next || current->next->type != WORD)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

static int	valid_quote_pairs(t_shell *data, t_token **token_list)
{
	t_token	*current;
	int		in_double;
	int		in_single;

	in_double = 0;
	in_single = 0;
	current = *token_list;
	while (current)
	{
		if (current->type == SINGLE_QUOTE && !in_double)
			in_single = !in_single;
		else if (current->type == DOUBLE_QUOTE && !in_single)
			in_double = !in_double;
		current = current->next;
	}
	return (!in_single && !in_double);
}

int	valid_syntax(t_shell *data, t_token **token_list)
{
	if (!*token_list)
		return (1);
	handle_spaces(data, token_list);
	mark_for_expansion(data, token_list);
	if (!valid_quote_pairs(data, token_list))
		return (1);
	else if (!valid_operator(token_list))
		return (2);
	else if (!valid_redir_target(token_list))
		return (3);
	else if (!valid_parentheses(data, token_list))
		return (4);
	handle_quotes(data, token_list);
	return (0);
}
