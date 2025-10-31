/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 14:31:19 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/31 14:31:35 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_exit_status(const char *str)
{
	while (*str && *str != '$')
		str++;
	if (*(str + 1) && *(str + 1) == '?')
		return (1);
	return (0);
}

void	expand_exit_status(t_ast *node, t_cmd *cmd, int index)
{
	char	*status;
	char	*new;
	int		len;
	int		i;

	status = ft_itoa(node->shell->exit_status);
	if (!status)
		malloc_error(node, node->shell, NULL);
	i = 0;
	while (cmd->args[index][i] && cmd->args[index][i] != '$')
		i++;
	len = ft_strlen(cmd->args[index]) + ft_strlen(status) - 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
	{
		free(status);
		malloc_error(node, node->shell, NULL);
	}
	ft_strlcpy(new, cmd->args[index], i + 1);
	ft_strlcat(new, status, len);
	ft_strlcat(new, cmd->args[index] + i + 2, len);
	free(status);
	free(cmd->args[index]);
	cmd->args[index] = new;
}
