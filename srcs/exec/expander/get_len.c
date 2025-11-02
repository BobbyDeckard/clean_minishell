/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_len.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 15:01:22 by imeulema          #+#    #+#             */
/*   Updated: 2025/11/02 15:03:16 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_whitespace(const char *str);

static int	get_double_len(t_cmd *cmd, int i, int *ptr)
{
	int	len;

	len = *ptr;
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "\"", 2))
		len += ft_strlen(cmd->args[i++]);
	if (cmd->args[i])
		i++;
	*ptr = len;
	return (i);
}

static int	get_single_len(t_cmd *cmd, int i, int *ptr)
{
	int	len;

	len = *ptr;
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "'", 2))
		len += ft_strlen(cmd->args[i++]);
	if (cmd->args[i])
		i++;
	*ptr = len;
	return (i);
}

int	get_len(t_cmd *cmd, int i)
{
	int	len;

	len = 0;
	while (cmd->args[i] && !is_whitespace(cmd->args[i]))
	{
		if (!ft_strncmp(cmd->args[i], "\"", 2))
			i = get_double_len(cmd, ++i, &len);
		else if (!ft_strncmp(cmd->args[i], "'", 2))
			i = get_single_len(cmd, ++i, &len);
		else
			len += ft_strlen(cmd->args[i++]);
	}
	return (len);
}
