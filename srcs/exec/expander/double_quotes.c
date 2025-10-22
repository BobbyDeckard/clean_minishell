/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 09:40:13 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/22 15:12:30 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*get_name(t_ast *node, const char *str);
int		contains_dol(const char *str);
int		get_name_len(const char *str);
int		handle_exit_status(t_ast *node, t_cmd *cmd, int index);
int		remove_var(t_ast *node, t_cmd *cmd, int index);

static int	handle_var(t_ast *node, t_cmd *cmd, char *entry, int index)
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
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(new, cmd->args[index], i + 1);
	ft_strlcat(new, entry, len);
	ft_strlcat(new, cmd->args[index] + i + name_len, len);
	free(cmd->args[index]);
	cmd->args[index] = new;
	return (0);
}

static int	expand_cat(t_ast *node, t_cmd *cmd, char **envp, int index)
{
	char	*name;
	int		i;
	int		j;

	name = get_name(node, cmd->args[index]);
	if (!name)
		return (handle_exit_status(node, cmd, index));
	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (name[j] && name[j] == envp[i][j])
			j++;
		if (envp[i][j] == '=' && !name[j])
		{
			free(name);
			return (handle_var(node, cmd, envp[i] + j + 1, index));
		}
	}
	free(name);
	return (remove_var(node, cmd, index));
}

void	double_expand(t_ast *node, t_cmd *cmd, char **envp, int index)
{
	while (contains_dol(cmd->args[index]))
	{
		if (expand_cat(node, cmd, envp, index))
			return ;
	}
}
