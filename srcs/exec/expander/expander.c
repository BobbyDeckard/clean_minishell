/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 12:12:02 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/30 15:50:23 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	is_whitespace(const char *str)
{
	if (ft_strlen(str) != 1)
		return (0);
	else if (*str >= 9 && *str <= 13)
		return (1);
	else if (*str == 32)
		return (1);
	return (0);
}

static int	skip_double(t_cmd *cmd, int i)
{
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "\"", 2))
		i++;
	if (cmd->args[i])
		i++;
	return (i);
}

static int	skip_single(t_cmd *cmd, int i)
{
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "'", 2))
		i++;
	if (cmd->args[i])
		i++;
	return (i);
}

static int	skip_whitespaces(t_cmd *cmd, int i)
{
	while (cmd->args[i] && is_whitespace(cmd->args[i]))
		i++;
	return (i);
}

static int	skip_word(t_cmd *cmd, int i)
{
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "\"", 2)
			&& ft_strncmp(cmd->args[i], "'", 2) && !is_whitespace(cmd->args[i]))
		i++;
	return (i);
}

static int	count_args(t_cmd *cmd)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (cmd->args[i])
	{
		if (!ft_strncmp(cmd->args[i], "\"", 2) && ++count)
			i = skip_double(cmd, ++i);
		else if (!ft_strncmp(cmd->args[i], "'", 2) && ++count)
			i = skip_single(cmd, ++i);
		else if (is_whitespace(cmd->args[i]))
			i = skip_whitespaces(cmd, i);
		else if (++count)
			i = skip_word(cmd, i);
	}
	return (count);
}

void	expander(t_ast *node, t_cmd *cmd)
{
	int	count;

	count = count_args(cmd);
	printf("Args:\n");
	int i = -1;
	while (cmd->args[++i])
		printf("arg[%d]: '%s'\n", i, cmd->args[i]);
	printf("Counted %d arguments\n", count);
	(void) node;
}
