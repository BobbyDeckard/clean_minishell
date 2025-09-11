/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaces.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:39:11 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/11 19:57:10 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static t_token	*remove_token(t_token **tokens, t_token *current)
{
	t_token	*ptr;

	if (*tokens == current)
	{
		*tokens = current->next;
		if (current->next)
			current->next->previous = NULL;
	}
	else
	{
		current->previous->next = current->next;
		if (current->next)
			current->next->previous = current->previous;
	}
	ptr = current->next;
	if (current->content)
		free(current->content);
	free(current);
	return (ptr);
}

void	handle_spaces(t_shell *data, t_token **tokens)
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
		else if (current->type == DOUBLE_QUOTE && !in_single)
			in_double = !in_double;
		else if (current->type == SPACE && (!in_double || !in_single))
			current = remove_token(tokens, current);
		else
			current = current->next;
	}
}
