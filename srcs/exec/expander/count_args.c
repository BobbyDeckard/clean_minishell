/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:05:58 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/30 16:48:22 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_whitespace(const char *str);

static int	skip_double(t_cmd *cmd, int i)
{
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "\"", 2))
		i++;
	if (cmd->args[i])
		i++;
	return (i);
}

int	skip_single(t_cmd *cmd, int i)
{
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "'", 2))
		i++;
	if (cmd->args[i])
		i++;
	return (i);
}

int	skip_whitespaces(t_cmd *cmd, int i)
{
	while (cmd->args[i] && is_whitespace(cmd->args[i]))
		i++;
	return (i);
}

static int	skip_arg(t_cmd *cmd, int i)
{
	while (cmd->args[i] && !is_whitespace(cmd->args[i]))
	{
		if (!ft_strncmp(cmd->args[i], "\"", 2))
			i = skip_double(cmd, ++i);
		else if (!ft_strncmp(cmd->args[i], "'", 2))
			i = skip_single(cmd, ++i);
		else
			i++;
	}
	return (i);
}

int	count_args(t_cmd *cmd)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (cmd->args[i])
	{
		if (is_whitespace(cmd->args[i]))
	  		i = skip_whitespaces(cmd, i);
		else
		{
			count++;
			i = skip_arg(cmd, i);
		}
	}
	return (count);
}
