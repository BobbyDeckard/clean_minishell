/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 20:30:44 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/22 17:11:22 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*filter_spaces(t_ast *node, char *entry, int *flag);
int		contains_contig_spaces(const char *str);

int	get_name_len(const char *str)
{
	int	i;

	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

int	handle_exit_status(t_ast *node, t_cmd *cmd, int index)
{
	char	*exit_status;
	char	*new;
	int		len;
	int		i;

	exit_status = ft_itoa(node->shell->exit_status);
	if (!exit_status)
		malloc_error(node, node->shell, NULL);
	i = 0;
	while (cmd->args[index][i] && cmd->args[index][i] != '$')
		i++;
	len = ft_strlen(cmd->args[index]) + ft_strlen(exit_status) - 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
	{
		free(exit_status);
		malloc_error(node, node->shell, NULL);
	}
	ft_strlcpy(new, cmd->args[index], i + 1);
	ft_strlcat(new, exit_status, len);
	ft_strlcat(new, cmd->args[index] + i + 2, len);
	free(exit_status);
	free(cmd->args[index]);
	cmd->args[index] = new;
	return (0);
}

int	handle_var(t_ast *node, t_cmd *cmd, char *entry, int index)
{
	char	*new;
	int		filtered_spaces;
	int		name_len;
	int		len;
	int		i;

	i = 0;
	filtered_spaces = 0;
	while (cmd->args[index][i] && cmd->args[index][i] != '$')
		i++;
	name_len = get_name_len(cmd->args[index] + i);
	if (contains_contig_spaces(entry))
		entry = filter_spaces(node, entry, &filtered_spaces);
	len = ft_strlen(cmd->args[index]) + ft_strlen(entry) - name_len + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(new, cmd->args[index], i + 1);
	ft_strlcat(new, entry, len);
	ft_strlcat(new, cmd->args[index] + i + name_len, len);
	if (filtered_spaces)
		free(entry);
	free(cmd->args[index]);
	cmd->args[index] = new;
	return (0);
}

int	remove_arg(t_cmd *cmd, int i)
{
	if (!cmd->args[i])
		return (0);
	free(cmd->args[i]);
	while (cmd->args[i])
	{
//		printf("Replacing arg[%d] (%p): '%s' by arg[%d] (%p): '%s'\n", i, cmd->args[i], cmd->args[i], i + 1, cmd->args[i + 1], cmd->args[i + 1]);
		cmd->args[i] = cmd->args[i + 1];
		i++;
	}
	return (1);
}

int	remove_var(t_ast *node, t_cmd *cmd, int index)
{
	char	*new;
	int		name_len;
	int		len;
	int		i;

	i = 0;
	while (cmd->args[index][i] && cmd->args[index][i] != '$')
		i++;
	name_len = get_name_len(cmd->args[index] + i);
	if (i == 0 && (int) ft_strlen(cmd->args[index]) == name_len)
		return (remove_arg(cmd, index));
	len = ft_strlen(cmd->args[index]) - name_len + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(new, cmd->args[index], i + 1);
	ft_strlcat(new, cmd->args[index] + i + name_len, len);
	free(cmd->args[index]);
	cmd->args[index] = new;
	return (0);
}
