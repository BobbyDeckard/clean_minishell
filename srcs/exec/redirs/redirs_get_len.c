/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_get_len.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 15:07:50 by imeulema          #+#    #+#             */
/*   Updated: 2025/11/02 15:09:45 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	get_double_len(t_rdr *rdr, int i, int *ptr)
{
	int	len;

	len = *ptr;
	while (rdr->args[i] && ft_strncmp(rdr->args[i], "\"", 2))
		len += ft_strlen(rdr->args[i++]);
	if (rdr->args[i])
		i++;
	*ptr = len;
	return (i);
}

static int	get_single_len(t_rdr *rdr, int i, int *ptr)
{
	int	len;

	len = *ptr;
	while (rdr->args[i] && ft_strncmp(rdr->args[i], "'", 2))
		len += ft_strlen(rdr->args[i++]);
	if (rdr->args[i])
		i++;
	*ptr = len;
	return (i);
}

int	rdr_get_len(t_rdr *rdr, int i)
{
	int	len;

	len = 0;
	while (rdr->args[i])
	{
		if (!ft_strncmp(rdr->args[i], "\"", 2))
			i = get_double_len(rdr, ++i, &len);
		else if (!ft_strncmp(rdr->args[i], "'", 2))
			i = get_single_len(rdr, ++i, &len);
		else
			len += ft_strlen(rdr->args[i++]);
	}
	return (len);
}
