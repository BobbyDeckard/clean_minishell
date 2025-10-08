/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:16:45 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/08 15:55:01 by imeulema         ###   ########.fr       */
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
	char	*cwd;
	int		len;

	free(oldpwd);
	free(node->data->envp[j]);
	len = ft_strlen(node->data->envp[i]) + 4;
	node->data->envp[j] = (char *) malloc(len * sizeof(char));
	if (!node->data->envp[j])
		malloc_error(node->root, node->data, NULL);
	ft_strlcpy(node->data->envp[j], "OLDPWD=", len);
	ft_strlcat(node->data->envp[j], node->data->envp[i] + 4, len);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		getcwd_error(node);
	free(node->data->envp[i]);
	len = ft_strlen(cwd) + 5;
	node->data->envp[i] = (char *) malloc(len * sizeof(char));
	if (!node->data->envp[i])
	{
		free(cwd);
		malloc_error(node->root, node->data, NULL);
	}
	ft_strlcpy(node->data->envp[i], "PWD=", len);
	ft_strlcat(node->data->envp[i], cwd, len);
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

int	cd(t_ast *node, int in_pipe)
{
	char	*oldpwd;
	char	*error;

	if (!in_pipe && make_redirs(node))
		return (set_exit_status(node, 1));
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		getcwd_error(node);
	if (!node->cmd.args[1])
	{
		if (!in_pipe)
			close_all_redirs(node);
		return (set_exit_status(node, 0));
	}
	if (chdir(node->cmd.args[1]) < 0)
	{
		error = cd_error(node);
		perror(error);
		free(error);
		free(oldpwd);
		if (!in_pipe)
			close_all_redirs(node);
		return (set_exit_status(node, 1));
	}
	make_updates(node, oldpwd);
	if (!in_pipe)
		close_all_redirs(node);
	return (set_exit_status(node, 0));
}
