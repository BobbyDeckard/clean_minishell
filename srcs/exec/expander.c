/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 23:13:35 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/12 15:21:18 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

static void	remove_var(t_ast *node, t_cmd *cmd, int i)
{
	char	*ptr;
	int		j;

	while (cmd->args[i + 1])
	{
		cmd->args[i] = cmd->args[i + 1];
		i++;
	}
}

static void	expand_var(t_ast *node, char *entry, int index)
{
	int	len;

	len = ft_strlen(entry) + 1;
	node->cmd.args[index] = (char *) malloc(len * sizeof(char));
	if (!node->cmd.args[index])
		malloc_error(node, node->data, NULL);
	ft_strlcpy(node->cmd.args[index], entry, len);
}

static void	expand(t_ast *node, t_cmd *cmd, char **envp, int index)
{
	char	*name;
	int		i;
	int		j;

	name = cmd->args[index] + 1;
	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (name[j] && name[j] == envp[i][j])
			j++;
		if (envp[i][j] == '=')
		{
			free(cmd->args[index]);
			return (expand_var(node, envp[i] + j + 1, index));
		}
	}
	free(cmd->args[index]);
	remove_var(node, cmd, index);
}

static void	expand_exit_status(t_ast *node, t_cmd *cmd, t_shell *data,
int index)
{
	char	*str;
	int		digits;

	digits = count_digits(data->exit_status);
	str = (char *) malloc((digits + 1) * sizeof(char));
	if (!str)
		malloc_error(node, data, NULL);
	str = ft_itoa(data->exit_status);
	free(cmd->args[index]);
	cmd->args[index] = str;
}

void	expander(t_ast *node, t_cmd *cmd)
{
	int	i;

	i = -1;
	while (cmd->args[++i])
	{
		if (cmd->exp[i] == 1)
			expand(node, cmd, node->data->envp, i);
		else if (cmd->exp[i] == 2)
			expand_exit_status(node, cmd, node->data, i);
	}
}
