/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:13:34 by imeulema          #+#    #+#             */
/*   Updated: 2025/08/31 14:04:52 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

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
	else if (!valid_operator(token_list))
		return (0);
//		return (ft_putstr_fd("Invalid pipe position:", 2), 0);
	else if (!valid_redir_target(token_list))
		return (0);
//		return (ft_putstr_fd("Invalid redir target:", 2), 0);
	else if (!valid_quote_pairs(token_list))
		return (0);
//		return (ft_putstr_fd("Unmatched quotes:", 2), 0);
	else if (!valid_parentheses(data, token_list))
		return (0);
	return (1);
}
