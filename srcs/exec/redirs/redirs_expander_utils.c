/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_expander_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 22:26:22 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/31 23:51:08 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	skip_double(t_rdr *rdr, int i)
{
	while (rdr->args[i] && ft_strncmp(rdr->args[i], "\"", 2))
		i++;
	if (rdr->args[i])
		i++;
	return (i);
}

int	rdr_skip_single(t_rdr *rdr, int i)
{
	while (rdr->args[i] && ft_strncmp(rdr->args[i], "'", 2))
		i++;
	if (rdr->args[i])
		i++;
	return (i);
}

int	rdr_count_args(t_rdr *rdr)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (rdr->args[i] && ++count)
	{
		if (!ft_strncmp(rdr->args[i], "\"", 2))
			i = skip_double(rdr, ++i);
		else if (!ft_strncmp(rdr->args[i], "'", 2))
			i = rdr_skip_single(rdr, ++i);
		else
			i++;
	}
	return (count);
}
