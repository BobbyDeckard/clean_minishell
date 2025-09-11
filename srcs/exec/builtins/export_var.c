/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:43:15 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/11 16:02:05 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

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

int	create_var(t_ast *node, int size)
{
	int	len;

	len = ft_strlen(node->cmd.args[1]) + 1;
	node->data->envp = make_new_env(node, size + 1);
	node->data->envp[size] = (char *) malloc(len * sizeof(char));
	if (!node->data->envp[size])
		malloc_error(node, node->data, NULL);
	ft_strlcpy(node->data->envp[size], node->cmd.args[1], len);
	node->data->envp[++size] = NULL;
	return (set_exit_status(node, 0));
}

int	assign_var(t_ast *node, int size)
{
	char	*name;
	int		len;
	int		i;

	name = get_var_name(node);
	i = -1;
	while (node->data->envp[++i])
	{
		if (!ft_strncmp(name, node->data->envp[i], ft_strlen(name)))
			break ;
	}
	if (node->data->envp[i])
	{
		free(node->data->envp[i]);
		len = ft_strlen(node->cmd.args[1]) + 1;
		node->data->envp[i] = (char *) malloc(len * sizeof(char));
		if (!node->data->envp[i])
			malloc_error(node, node->data, NULL);
		ft_strlcpy(node->data->envp[i], node->cmd.args[1], len);
		return (set_exit_status(node, 0));
	}
	return (create_var(node, size));
}
