/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:07:30 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/31 14:27:50 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	contains_dol(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '$')
			return (1);
	}
	return (0);
}

int	is_whitespace(const char *str)
{
	if (ft_strlen(str) != 1)
		return (0);
	else if (*str >= 9 && *str <= 13)
		return (1);
	else if (*str == 32)
		return (1);
	return (0);
}

void	handle_var_error(t_ast *node, char *entry, int spaces)
{
	if (spaces)
		free(entry);
	malloc_error(node, node->shell, NULL);
}
