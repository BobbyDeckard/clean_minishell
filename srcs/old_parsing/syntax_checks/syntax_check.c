/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:13:34 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/14 10:39:14 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	valid_operator(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	while (current)
	{
		if (current->type == PIPE || current->type == AND
			|| current->type == OR)
		{
			if (!current->previous || !current->next)
				return (0);
			else if (is_logical_operator(current->previous))
				return (0);
			else if (is_logical_operator(current->next))
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
			|| type == HEREDOC || type == HEREDOC_EXP)
		{
			if (!current->next || (!is_word(current->next->type)
					&& current->next->type != DOUBLE_QUOTE
					&& current->next->type != SINGLE_QUOTE))
				return (0);
		}
		current = current->next;
	}
	return (1);
}

static int	valid_quote_pairs(t_token **token_list)
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
	handle_contiguous_words(token_list);
	handle_spaces(data, token_list);
	mark_for_expansion(token_list);
	if (!valid_quote_pairs(token_list))
		return (1);
	handle_quotes(data, token_list);
	remove_spaces(token_list);
	if (!valid_operator(token_list))
		return (2);
	else if (!valid_redir_target(token_list))
		return (3);
	else if (!check_parentheses(token_list))
		return (4);
	return (0);
}
