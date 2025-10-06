/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cont_words.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 17:41:02 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/06 13:03:48 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void	check_prevs(t_token *current)
{
	if (!current->previous || !current->previous->previous)
		return ;
	if (current->previous->type == SPACE)
		return ;
	current->type = WORD_CAT;
}

static void	check_prevs_and_next(t_token *current)
{
	if (!current->previous || !current->next)
		return ;
	if (current->previous->type == SPACE)
		return ;
	if (current->next->type != SPACE)
		current->next->type = WORD_CAT;
}

void	handle_contiguous_words(t_token **tokens)
{
	t_token_type	type;
	t_token			*current;

	current = *tokens;
	while (current)
	{
		type = current->type;
		if (is_word(type))
			check_prevs(current);
		else if (type == SINGLE_QUOTE || type == DOUBLE_QUOTE)
			check_prevs_and_next(current);
		current = current->next;
	}
}
