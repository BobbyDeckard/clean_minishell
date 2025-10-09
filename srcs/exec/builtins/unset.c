/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:28:23 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/09 14:28:03 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	is_cmd_arg(char **args, char *entry)
{
	int		i;
	int		j;

	i = -1;
	while (args[++i])
	{
		j = 0;
		while (entry[j] && entry[j] == args[i][j])
			j++;
		if (!entry[j] || entry[j] == '=')
			return (1);
	}
	return (0);
}

static void	filter_and_copy(t_ast *node, char **envp, char **new, char **args)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (envp[++i])
	{
		if (!is_cmd_arg(args, envp[i]))
			new[j++] = copy_env_entry(node, new, i);
	}
	new[j] = NULL;
}

static void	remove_paths(t_shell *data)
{
	int	i;

	if (!data->paths)
		return ;
	i = -1;
	while (data->paths[++i])
	{
		free(data->paths[i]);
		data->paths[i] = NULL;
	}
	free(data->paths);
	data->paths = NULL;
}

static int	nothing_to_unset(t_ast *node, int in_pipe)
{
	if (!in_pipe)
		close_all_redirs(node);
	return (set_exit_status(node, 1));
}

int	unset(t_ast *node, int in_pipe)
{
	char	**new;
	int		len;
	int		i;

	if (!in_pipe && make_redirs(node))
		return (set_exit_status(node, 1));
	if (char_arr_len(node->data->envp) == -1)
		return (nothing_to_unset(node, in_pipe));
	len = char_arr_len(node->data->envp) + 1;
	new = (char **) ft_calloc(len, sizeof(char *));
	if (!new)
		malloc_error(node, node->data, NULL);
	i = 0;
	while (node->cmd.args[++i])
	{
		if (!ft_strncmp(node->cmd.args[i], "PATH", 5))
			remove_paths(node->data);
	}
	filter_and_copy(node, node->data->envp, new, node->cmd.args);
	free_char_array(node->data->envp);
	node->data->envp = new;
	if (!in_pipe)
		close_all_redirs(node);
	return (set_exit_status(node, 0));
}
