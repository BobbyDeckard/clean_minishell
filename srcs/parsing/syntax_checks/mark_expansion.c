/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mark_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 23:23:57 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/12 00:11:21 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	mark_for_expansion(t_shell *data, t_token **tokens)
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
		else if (current->type == ENV_VAR && !in_single)
			current->needs_expansion = 1;
		else if (current->type == EXIT_STATUS && !in_single)
			current->needs_expansion = 1;
		current = current->next;
	}
}
