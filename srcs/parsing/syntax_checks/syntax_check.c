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

static int	valid_pipe_pos(t_token **token_list)
{
	t_token	*first;
	t_token	*last;

	first = *token_list;
	if (first->type == PIPE)
		return (0);
	last = first;
	while (last->next)
		last = last->next;
	if (last->type == PIPE)
		return (0);
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
		if (type == REDIR_OUT || type == REDIR_IN || type == REDIR_APPEND || type == HEREDOC)
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
	int		doubles;
	int		singles;

	doubles = 0;
	singles = 0;
	current = *token_list;
	while (current)
	{
		if (current->type == SINGLE_QUOTE)
			singles++;
		else if (current->type == DOUBLE_QUOTE)
			doubles++;
		current = current->next;
	}
	if (singles % 2 || doubles % 2)
		return (0);
	return (1);
}

int	valid_syntax(t_shell *data, t_token **token_list)
{
	if (!*token_list)
		return (1);
	else if (!valid_pipe_pos(token_list))
		return (ft_putstr_fd("Invalid pipe position:", 2), 0);
	else if (!valid_redir_target(token_list))
		return (ft_putstr_fd("Invalid redir target:", 2), 0);
	else if (!valid_quote_pairs(token_list))
		return (ft_putstr_fd("Unmatched quotes:", 2), 0);
	else if (!valid_parentheses(data, token_list))
		return (0);
	return (1);
}
