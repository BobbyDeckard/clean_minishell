/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 11:52:03 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/06 13:06:17 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

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
