/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 11:52:03 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/05 11:15:13 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void	check_next(t_token *current)
{
	if (current->type != SINGLE_QUOTE && current->type != DOUBLE_QUOTE)
		return ;
	if (current->next && current->next->type == WORD)
		current->next->type = WORD_CAT;
	return ;
}

void	handle_contiguous_quotes(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	while (current)
	{
		if (current->type == SINGLE_QUOTE && current->next)
			check_next(current->next);
		else if (current->type == DOUBLE_QUOTE && current->next)
			check_next(current->next);
		current = current->next;
	}
}

void	handle_quotes(t_shell *data, t_token **tokens)
{
	t_token	*current;
	int		in_double;
	int		in_single;

	in_double = 0;
	in_single = 0;
	current = *tokens;
	while (current)
	{
		if (current->type == SINGLE_QUOTE && !in_double)
			in_single = !in_single;
		else if (current->type == SINGLE_QUOTE)
			current = cat_word(data, current, current->previous, current->next);
		else if (current->type == DOUBLE_QUOTE && !in_single)
			in_double = !in_double;
		else if (current->type == DOUBLE_QUOTE)
			current = cat_word(data, current, current->previous, current->next);
		else if (current->content && (in_double || in_single))
			current = cat_word(data, current, current->previous, current->next);
		current = current->next;
	}
}
