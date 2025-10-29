/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 11:33:57 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/14 11:34:14 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	paren_open(t_token **list)
{
	t_token	*current;

	current = *list;
	while (current)
	{
		if (current->type == PAREN_OPEN)
			return (1);
		current = current->next;
	}
	return (0);
}

t_token	*get_last_close(t_token **list)
{
	t_token	*last_close;
	t_token	*current;

	current = *list;
	last_close = NULL;
	while (current)
	{
		if (current->type == PAREN_CLOSE)
			last_close = current;
		current = current->next;
	}
	return (last_close);
}
