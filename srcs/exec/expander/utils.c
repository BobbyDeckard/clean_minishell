/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 20:30:44 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/27 20:35:33 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	get_name_len(const char *str)
{
	int	i;

	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

void	handle_exit_status(t_ast *node, t_cmd *cmd, int index)
{
	char	*exit_status;
	char	*new;
	int		len;
	int		i;

	exit_status = ft_itoa(node->data->exit_status);
	i = 0;
	while (cmd->args[index][i] && cmd->args[index][i] != '$')
		i++;
	len = ft_strlen(cmd->args[index]) + ft_strlen(exit_status) - 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->data, NULL);
	ft_strlcpy(new, cmd->args[index], i + 1);
	ft_strlcat(new, exit_status, len);
	ft_strlcat(new, cmd->args[index] + i + 2, len);
	free(exit_status);
	free(cmd->args[index]);
	cmd->args[index] = new;
}

void	handle_var(t_ast *node, t_cmd *cmd, char *entry, int index)
{
	char	*new;
	int		name_len;
	int		len;
	int		i;

	i = 0;
	while (cmd->args[index][i] && cmd->args[index][i] != '$')
		i++;
	name_len = get_name_len(cmd->args[index] + i);
	len = ft_strlen(cmd->args[index]) + ft_strlen(entry) - name_len + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->data, NULL);
	ft_strlcpy(new, cmd->args[index], i + 1);
	ft_strlcat(new, entry, len);
	ft_strlcat(new, cmd->args[index] + i + name_len, len);
	free(cmd->args[index]);
	cmd->args[index] = new;
}

void	remove_var(t_ast *node, t_cmd *cmd, int index)
{
	char	*new;
	int		name_len;
	int		len;
	int		i;

	i = 0;
	while (cmd->args[index][i] && cmd->args[index][i] != '$')
		i++;
	name_len = get_name_len(cmd->args[index] + i);
	len = ft_strlen(cmd->args[index]) - name_len + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->data, NULL);
	ft_strlcpy(new, cmd->args[index], i + 1);
	ft_strlcat(new, cmd->args[index] + i + name_len, len);
	free(cmd->args[index]);
	cmd->args[index] = new;
}
