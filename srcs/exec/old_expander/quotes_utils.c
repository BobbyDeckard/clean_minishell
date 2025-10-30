/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 18:41:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/30 10:44:56 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_whitespace(const char *str);
int	remove_arg(t_cmd *cmd, int i);

int	handle_empty_quotes(t_cmd *cmd, int index)
{
	if (cmd->args[index + 1] && is_whitespace(cmd->args[index + 1]))
		remove_arg(cmd, index);
	cmd->args[index][0] = 0;
	return (0);
}

int	contains_double(const char *str)
{
	int	i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i])
	{
		if (str[i] == '"')
			return (1);
	}
	return (0);
}

int	contains_single(const char *str)
{
	int	i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'')
			return (1);
	}
	return (0);
}

int	contains_quote(const char *str)
{
	if (contains_single(str))
		return (1);
	else if (contains_double(str))
		return (1);
	return (0);
}
