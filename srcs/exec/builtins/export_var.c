/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:43:15 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/29 14:08:33 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	create_env(t_ast *node)
{
	char	**new;
	int		len;
	int		i;

	i = -1;
	while (node->cmd.args[++i])
		continue ;
	new = (char **) malloc(i * sizeof(char *));
	if (!new)
		malloc_error(node, node->data, NULL);
	i = -1;
	while (node->cmd.args[++i + 1])
	{
		len = ft_strlen(node->cmd.args[i + 1]) + 1;
		new[i] = (char *) malloc(len * sizeof(char));
		if (!new[i])
		{
			while (--i >= 0)
				free(new[i]);
			free(new);
			malloc_error(node, node->data, NULL);
		}
		ft_strlcpy(new[i], node->cmd.args[i + 1], len);
	}
	new[i] = NULL;
	node->data->envp = new;
	return (0);
}

static char	**make_new_env(t_ast *node, int size)
{
	char	**new;
	int		i;

	new = (char **) malloc(size * sizeof(char *));
	if (!new)
		malloc_error(node, node->data, NULL);
	i = -1;
	while (node->data->envp[++i])
		new[i] = copy_env_entry(node, new, i, i);
	new[i] = NULL;
	i = -1;
	while (node->data->envp[++i])
		free(node->data->envp[i]);
	free(node->data->envp);
	return (new);
}

static char	*get_var_name(t_ast *node)
{
	char	*name;
	int		i;

	i = 0;
	while (node->cmd.args[1][i] != '=')
		i++;
	name = (char *) malloc(i * sizeof(char));
	if (!name)
		malloc_error(node, node->data, NULL);
	name[--i] = 0;
	while (--i >= 0)
		name[i] = node->cmd.args[1][i];
	return (name);
}

int	create_var(t_ast *node, int size, int arg)
{
	int	len;

	if (var_exists(node->cmd.args[arg], node->data->envp))
		return (set_exit_status(node, 0));
	len = ft_strlen(node->cmd.args[arg]) + 1;
	node->data->envp = make_new_env(node, size + 1);
	node->data->envp[size] = (char *) malloc(len * sizeof(char));
	if (!node->data->envp[size])
		malloc_error(node, node->data, NULL);
	ft_strlcpy(node->data->envp[size], node->cmd.args[arg], len);
	node->data->envp[++size] = NULL;
	return (set_exit_status(node, 0));
}

int	assign_var(t_ast *node, int size, int arg)
{
	char	*name;
	int		len;
	int		i;

	name = get_var_name(node);
	len = ft_strlen(name);
	i = -1;
	while (node->data->envp[++i])
	{
		if (!ft_strncmp(name, node->data->envp[i], len))
			break ;
	}
	if (node->data->envp[i])
	{
		free(node->data->envp[i]);
		len = ft_strlen(node->cmd.args[arg]) + 1;
		node->data->envp[i] = (char *) malloc(len * sizeof(char));
		if (!node->data->envp[i])
			malloc_error(node, node->data, NULL);
		ft_strlcpy(node->data->envp[i], node->cmd.args[arg], len);
		free(name);
		return (set_exit_status(node, 0));
	}
	free(name);
	return (create_var(node, size, arg));
}
