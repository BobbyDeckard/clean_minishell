/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaces_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 19:13:38 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/27 19:14:58 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_whitespace_char(const char c)
{
	if (c >= 9 && c <= 13)
		return (1);
	else if (c == 32)
		return (1);
	return (0);
}

int	contains_contig_spaces(const char *str)
{
	int	space;
	int	i;

	i = -1;
	space = 0;
	while (str[++i])
	{
		if (str[i] == ' ' && !space)
			space = !space;
		else if (str[i] == ' ')
			return (1);
		else if (space)
			space = !space;
	}
	return (0);
}
