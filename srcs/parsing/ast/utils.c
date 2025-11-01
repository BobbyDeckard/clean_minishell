/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:30:48 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/28 16:30:49 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_redir_token(t_t_type type);

int	count_redirs(t_token **list, int start, int end)
{
	t_token	*current;
	int		count;

	count = 0;
	current = get_token_at_index(list, start);
	//	Check start end condition
	while (current && start <= end)
	{
		if (is_redir_token(current->type))
			count++;
		current = current->next;
		start++;
	}
	return (count);
}
