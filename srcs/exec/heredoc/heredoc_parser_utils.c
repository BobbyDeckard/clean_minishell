/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_parser_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 16:44:35 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 16:50:04 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	get_double_body(t_rdr *rdr, char *file, int i, int len)
{
	while (rdr->args[++i] && ft_strncmp(rdr->args[i], "\"", 2))
		ft_strlcat(file, rdr->args[i], len);
	return (++i);
}

int	get_single_body(t_rdr *rdr, char *file, int i, int len)
{
	while (rdr->args[++i] && ft_strncmp(rdr->args[i], "'", 2))
		ft_strlcat(file, rdr->args[i], len);
	return (++i);
}
