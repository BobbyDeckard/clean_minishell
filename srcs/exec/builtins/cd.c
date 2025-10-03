/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:16:45 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/23 21:56:29 by imeulema         ###   ########.fr       */
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

static void	update_both(t_ast *node, int i, int j, char *oldpwd)
{
	char	**envp;
	char	*cwd;
	int		len;

	free(oldpwd);
	envp = node->data->envp;
	free(envp[j]);
	len = ft_strlen(envp[i]) + 4;
	envp[j] = (char *) malloc(len * sizeof(char));
	if (!envp[j])
		malloc_error(node->root, node->data, NULL);
	ft_strlcpy(envp[j], "OLDPWD=", len);
	ft_strlcat(envp[j], envp[i] + 4, len);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		getcwd_error(node);
	free(envp[i]);
	len = ft_strlen(cwd) + 5;
	envp[i] = (char *) malloc(len * sizeof(char));
	if (!envp[i])
	{
		free(cwd);
		malloc_error(node->root, node->data, NULL);
	}
	ft_strlcpy(envp[i], "PWD=", len);
	ft_strlcat(envp[i], cwd, len);
	free(cwd);
}

//	If there is no PWD, oldpwd sets to NULL !!!ONCE!!!
static void	make_updates(t_ast *node, char *oldpwd)
{
	int	i;
	int	j;

	i = -1;
	while (node->root->data->envp[++i])
	{
		if (!ft_strncmp(node->root->data->envp[i], "PWD", 3))
			break ;
	}
	j = -1;
	while (node->root->data->envp[++j])
	{
		if (!ft_strncmp(node->root->data->envp[j], "OLDPWD", 6))
			break ;
	}
	if (node->root->data->envp[i] && node->root->data->envp[j])
		return (update_both(node, i, j, oldpwd));
	else if (node->root->data->envp[i])
		return (update_pwd(node, i, oldpwd));
	else if (node->root->data->envp[j])
		return (update_oldpwd(node, j, oldpwd));
}

int	cd(t_ast *node)
{
	char	*oldpwd;
	char	*error;

	if (make_redirs(node))
		return (set_exit_status(node, 1));
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		getcwd_error(node);
	if (!node->cmd.args[1])
		return (set_exit_status(node, 0));
	if (chdir(node->cmd.args[1]) < 0)
	{
		error = cd_error(node);
		perror(error);
		free(error);
		free(oldpwd);
		close_redirs(&node->cmd);
		unlink_heredoc(node);
		return (set_exit_status(node, 1));
	}
	make_updates(node, oldpwd);
	close_redirs(&node->cmd);
	unlink_heredoc(node);
	return (set_exit_status(node, 0));
}
