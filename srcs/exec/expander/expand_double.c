/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_double.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 14:33:55 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/31 23:02:38 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*get_name(t_ast *node, const char *str);
void	expand_exit_status(t_ast *node, t_cmd *cmd, int index);
int		is_exit_status(const char *str);
int		contains_dol(const char *str);
int		get_name_len(const char *str);
int		is_whitespace(const char *str);
int		skip_whitespaces(t_cmd *cmd, int i);

static void	handle_var(t_ast *node, t_cmd *cmd, char *entry, int index)
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
	new = (char *) ft_calloc(len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(new, cmd->args[index], i + 1);
	ft_strlcat(new, entry, len);
	ft_strlcat(new, cmd->args[index] + i + name_len, len);
	free(cmd->args[index]);
	cmd->args[index] = new;
}

static void	expand_var(t_ast *node, t_cmd *cmd, char *name, int index)
{
	char	**envp;
	int		i;
	int		j;

	i = -1;
	envp = node->shell->envp;
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
	return (handle_var(node, cmd, "", index));
}

static void	expand_arg(t_ast *node, t_cmd *cmd, int i)
{
	char	*name;

	while (contains_dol(cmd->args[i]))
	{
		if (is_exit_status(cmd->args[i]))
			expand_exit_status(node, cmd, i);
		else
		{
			name = get_name(node, cmd->args[i]);
			expand_var(node, cmd, name, i);
		}
	}
}

int	expand_in_double(t_ast *node, t_cmd *cmd, int i)
{
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "\"", 2))
	{
		if (is_whitespace(cmd->args[i]))
			i = skip_whitespaces(cmd, i);
		else if (contains_dol(cmd->args[i]))
			expand_arg(node, cmd, i++);
		else
			i++;
	}
	if (cmd->args[i])
		i++;
	return (i);
}
