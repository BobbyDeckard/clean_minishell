/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:42:40 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/23 23:21:30 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	update_error(t_shell *data, char *path, int i)
{
	if (path)
		free(path);
	data->envp[i] = NULL;
	while (data->envp[++i])
		free(data->envp[i]);
	malloc_error(NULL, data, NULL);
}

static void	update_(t_shell *data, char *path)
{
	int	len;
	int	i;

	i = -1;
	while (data->envp[++i])
	{
		if (!ft_strncmp(data->envp[i], "_=", 2))
		{
			free(data->envp[i]);
			len = ft_strlen(path) + 3;
			data->envp[i] = (char *) malloc(len * sizeof(char));
			if (!data->envp[i])
				update_error(data, path, i);
			ft_strlcpy(data->envp[i], "_=", len);
			ft_strlcat(data->envp[i], path, len);
			break ;
		}
	}
}

static void	found_path(t_ast *node, t_cmd *cmd, char *path)
{
	cmd->path = ft_strdup(path);
	free(path);
	update_(node->data, cmd->path);
}

void	get_cmd_path(t_ast *node, t_cmd *cmd, char **paths)
{
	char	*full_path;
	char	*name;
	int		i;

	name = cmd->args[0];
	i = -1;
	while (paths && paths[++i])
	{
		full_path = ft_strjoin(paths[i], name);
		if (!access(full_path, F_OK))
			return (found_path(node, cmd, full_path));
		free(full_path);
	}
	cmd->path = NULL;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": command not found\n", 2);
}
