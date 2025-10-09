/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:43:15 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/09 15:00:28 by imeulema         ###   ########.fr       */
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
	new = (char **) ft_calloc(i, sizeof(char *));
	if (!new)
		malloc_error(node, node->data, NULL);
	i = -1;
	while (node->cmd.args[++i + 1])
	{
		len = ft_strlen(node->cmd.args[i + 1]) + 1;
		new[i] = (char *) malloc(len * sizeof(char));
		if (!new[i])
			create_env_error(node, new, i);
		ft_strlcpy(new[i], node->cmd.args[i + 1], len);
	}
	new[i] = NULL;
	node->data->envp = new;
	close_all_redirs(node);
	return (0);
}

//	Since node->data->envp is immediately set to our new pointer,
//	there is no need to reset to NULL.
static char	**make_new_env(t_ast *node, int size)
{
	char	**new;
	int		i;

	new = (char **) ft_calloc(size, sizeof(char *));
	if (!new)
		malloc_error(node, node->data, NULL);
	i = -1;
	while (node->data->envp[++i])
		new[i] = copy_env_entry(node, new, i);
	new[i] = NULL;
	i = -1;
	while (node->data->envp[++i])
		free(node->data->envp[i]);
	free(node->data->envp);
	return (new);
}

static char	*get_var_name(t_ast *node, int arg)
{
	char	*name;
	int		i;

	i = 0;
	while (node->cmd.args[arg][i] != '=')
		i++;
	name = (char *) malloc(++i * sizeof(char));
	if (!name)
		malloc_error(node, node->data, NULL);
	name[--i] = 0;
	while (--i >= 0)
		name[i] = node->cmd.args[arg][i];
	return (name);
}

int	create_var(t_ast *node, int size, int arg)
{
	int	len;

	if (var_exists(node->cmd.args[arg], node->data->envp))
		return (set_exit_status(node, 0));
	len = ft_strlen(node->cmd.args[arg]) + 1;
	node->data->envp = make_new_env(node, size + 2);
	node->data->envp[size] = (char *) malloc(len * sizeof(char));
	if (!node->data->envp[size])
		malloc_error(node, node->data, NULL);
	ft_strlcpy(node->data->envp[size], node->cmd.args[arg], len);
	node->data->envp[++size] = NULL;
	close_all_redirs(node);
	return (set_exit_status(node, 0));
}

int	assign_var(t_ast *node, int size, int arg)
{
	char	*name;
	int		len;
	int		i;

	name = get_var_name(node, arg);
	len = ft_strlen(name);
	i = get_entry_index(name, node->data->envp);
	if (node->data->envp[i])
	{
		free(node->data->envp[i]);
		len = ft_strlen(node->cmd.args[arg]) + 1;
		node->data->envp[i] = (char *) malloc(len * sizeof(char));
		if (!node->data->envp[i])
			malloc_error(node, node->data, NULL);
		ft_strlcpy(node->data->envp[i], node->cmd.args[arg], len);
		free(name);
		close_all_redirs(node);
		return (set_exit_status(node, 0));
	}
	free(name);
	return (create_var(node, size, arg));
}
