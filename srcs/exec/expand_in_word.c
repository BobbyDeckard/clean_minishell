/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_in_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:18:41 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/27 14:49:52 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/*
static void	cat_var_in_word(t_ast *node, t_cmd *cmd, char *entry, int index)
{
	char	*new;
	int		varlen;
	int		len;
	int		i;
	int		j;

	varlen = name_len(node->cmd.args[index] + offset);
	len = ft_strlen(entry) + ft_strlen(node->cmd.args[index]) - varlen + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->data, NULL);
	i = -1;
	while (node->cmd.args[index][++i] != '$')
		new[i] = node->cmd.args[index][i];
	new[i] = 0;
	ft_strlcat(new, entry, len);
	ft_strlcat(new, node->cmd.args[index] + offset + varlen, len);
	free(node->cmd.args[index]);
	node->cmd.args[index] = new;
}

static void	remove_var_from_word(t_ast *node, t_cmd *cmd, int index)
{
	char	*new;
	int		len;

	len = ft_strlen(node->cmd.args[index]) - name_len(node->cmd.args[index] + offset) + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->data, NULL);
}
*/

static int	contains_dol(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '$')
			return (1);
	}
	return (0);
}

static int	get_name_len(const char *str)
{
	int	i;
	
	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

static char	*get_name(t_ast *node, const char *str)
{
	char	*name;
	int		len;
	int		i;

	while (*str && *str != '$')
		str++;
	len = get_name_len(str) + 1;
	name = (char *) malloc(len * sizeof(char));
	if (!name)
		malloc_error(node, node->data, NULL);
	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		name[i - 1] = str[i];
		i++;
	}
	name[i - 1] = 0;
	return (name);
}

static void	cat_var_in_word(t_ast *node, t_cmd *cmd, char *entry, int index)
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

static void	expand_cat(t_ast *node, t_cmd *cmd, char **envp, int index)
{
	char	*name;
	int		name_len;
	int		i;
	int		j;

	name = get_name(node, cmd->args[index]);
	printf("In expand_cat(), name is %s\n", name);
	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (name[j] && name[j] == envp[i][j])
			j++;
		if (envp[i][j] == '=' && !name[j])
		{
			free(name);
			return (cat_var_in_word(node, cmd, envp[i] + j + 1, index));
		}
	}
	free(name);
//	return (remove_var_from_word(node, cmd, index));
}

void	expand_in_word(t_ast *node, t_cmd *cmd, char **envp, int index)
{
	int	i = 0;
	while (contains_dol(cmd->args[index]) && ++i < 2)
		expand_cat(node, cmd, envp, index);
}
