/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:16:45 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/08 18:08:20 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static char	*cd_error(t_ast *node)
{
	t_cmd	cmd;
	char	*msg;
	int		len;

	cmd = node->cmd;
	len = ft_strlen(cmd.args[1]) + 5;
	msg = (char *) malloc(len * sizeof(char));
	if (!msg)
		malloc_error(node, node->data, NULL);
	ft_strlcpy(msg, "cd: ", len);
	ft_strlcat(msg, cmd.args[1], len);
	return (msg);
}

static void	update_both(t_ast *node, int i, int j)
{
	char	**envp;
	char	*cwd;
	int		len;

	envp = node->data->envp;
	free(envp[j]);
	len = ft_strlen(envp[i]) + 4;
	envp[j] = (char *) malloc(len * sizeof(char));
	if (!envp[j])
		malloc_error(node->root, node->data, NULL);
	ft_strlcpy(envp[j], "OLDPWD=", len);
	ft_strlcat(envp[j], envp[i] + 4, len);
	free(envp[i]);
	cwd = getcwd(NULL, 0);
	len = ft_strlen(cwd) + 5;
	envp[i] = (char *) malloc(len * sizeof(char));
	if (!envp[i])
		malloc_error(node->root, node->data, NULL);
	ft_strlcpy(envp[i], "PWD=", len);
	ft_strlcat(envp[i], cwd, len);
	free(cwd);
}

static void	update_pwd(t_ast *node)
{
	int	i;
	int	j;

	i = -1;
	while (node->root->data->envp[++i])
	{
		if (!ft_strncmp(node->root->data->envp[i], "PWD=", 4))
			break ;
	}
	j = -1;
	while (node->root->data->envp[++j])
	{
		if (!ft_strncmp(node->root->data->envp[j], "OLDPWD=", 7))
			break ;
	}
	if (node->root->data->envp[i] && node->root->data->envp[j])
		update_both(node, i, j);
	// shouldn't there be updates to each individually ?
}

int	cd(t_ast *node)
{
	char	*error;

	if (make_redirs(node))
		return (set_exit_status(node, 1));
	if (chdir(node->cmd.args[1]) < 0)
	{
		error = cd_error(node);
		perror(error);
		free(error);
		close_redirs(node->cmd);
		unlink_heredoc(node);
		return (set_exit_status(node, 1));
	}
	update_pwd(node);
	close_redirs(node->cmd);
	unlink_heredoc(node);
	return (set_exit_status(node, 0));
}
