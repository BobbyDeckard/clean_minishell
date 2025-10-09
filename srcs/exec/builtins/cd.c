/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:16:45 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/09 13:08:34 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static char	*cd_error(t_ast *node, int arg)
{
	t_cmd	cmd;
	char	*msg;
	int		len;

	cmd = node->cmd;
	len = ft_strlen(cmd.args[arg]) + 5;
	msg = (char *) malloc(len * sizeof(char));
	if (!msg)
		malloc_error(node, node->data, NULL);
	ft_strlcpy(msg, "cd: ", len);
	ft_strlcat(msg, cmd.args[arg], len);
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

static int	chdir_error(t_ast *node, char *oldpwd, int arg, int in_pipe)
{
	char	*error;

	error = cd_error(node, arg);
	perror(error);
	free(error);
	free(oldpwd);
	if (!in_pipe)
		close_all_redirs(node);
	return (set_exit_status(node, 1));
}

//	wtf...?
//	Test done in this directory
static int	print_oldpwd(t_ast *node, char **envp, int in_pipe)
{
	char	name[7];
	int		i;
	int		j;

	ft_strlcpy(name, "OLDPWD", 7);
	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (name[j] && name[j] == envp[i][j])
			j++;
		if (envp[i][j] == '=' && !name[j])
		{
			ft_putstr_fd(envp[i] + 6, node->cmd.fd_out);
			ft_putchar_fd('\n', node->cmd.fd_out);
			if (!in_pipe)
				close_all_redirs(node);
			return (set_exit_status(node, 0));
		}
	}
	ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
	if (!in_pipe)
		close_all_redirs(node);
	return (set_exit_status(node, 1));
}

int	cd(t_ast *node, int in_pipe)
{
	char	*oldpwd;

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
	if (!ft_strncmp(node->cmd.args[1], "-", 2))
		return (!print_oldpwd(node, node->data->envp, in_pipe));
	else if (chdir(node->cmd.args[1]) < 0)
		return (chdir_error(node, oldpwd, 1, in_pipe));
	make_updates(node, oldpwd);
	if (!in_pipe)
		close_all_redirs(node);
	return (set_exit_status(node, 0));
}
