/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:16:45 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/10 16:32:39 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*cd_error(t_ast *node, int arg);
void	close_all_redirs(t_ast *node);
void	getcwd_error(t_ast *node);
void	update_oldpwd(t_ast *node, int i, char *oldpwd);
void	update_pwd(t_ast *node, int i, char *oldpwd);
int		cd_home(t_ast *node, int in_pipe);
int		make_redirs(t_ast *node);
int		set_exit_status(t_ast *node, int status);
int		too_many_args_cd(t_ast *node, int in_pipe);

static void	update_both(t_ast *node, int i, int j, char *oldpwd)
{
	char	*cwd;
	int		len;

	free(oldpwd);
	free(node->shell->envp[j]);
	len = ft_strlen(node->shell->envp[i]) + 4;
	node->shell->envp[j] = (char *) malloc(len * sizeof(char));
	if (!node->shell->envp[j])
		malloc_error(node->root, node->shell, NULL);
	ft_strlcpy(node->shell->envp[j], "OLDPWD=", len);
	ft_strlcat(node->shell->envp[j], node->shell->envp[i] + 4, len);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		getcwd_error(node);
	free(node->shell->envp[i]);
	len = ft_strlen(cwd) + 5;
	node->shell->envp[i] = (char *) malloc(len * sizeof(char));
	if (!node->shell->envp[i])
	{
		free(cwd);
		malloc_error(node->root, node->shell, NULL);
	}
	ft_strlcpy(node->shell->envp[i], "PWD=", len);
	ft_strlcat(node->shell->envp[i], cwd, len);
	free(cwd);
}

//	If there is no PWD, oldpwd sets to NULL !!!ONCE!!!
static void	make_updates(t_ast *node, char *oldpwd)
{
	int	i;
	int	j;

	i = -1;
	while (node->shell->envp[++i])
	{
		if (!ft_strncmp(node->shell->envp[i], "PWD", 3))
			break ;
	}
	j = -1;
	while (node->shell->envp[++j])
	{
		if (!ft_strncmp(node->shell->envp[j], "OLDPWD", 6))
			break ;
	}
	if (node->shell->envp[i] && node->shell->envp[j])
		return (update_both(node, i, j, oldpwd));
	else if (node->shell->envp[i])
		return (update_pwd(node, i, oldpwd));
	else if (node->shell->envp[j])
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

	if (make_redirs(node))
		return (set_exit_status(node, 1));
	else if (!node->cmd.args[1])
		return (cd_home(node, in_pipe));
	else if (node->cmd.args[2])
		return (too_many_args_cd(node, in_pipe));
	else if (!ft_strncmp(node->cmd.args[1], "-", 2))
		return (!print_oldpwd(node, node->shell->envp, in_pipe));
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		getcwd_error(node);
	else if (chdir(node->cmd.args[1]) < 0)
		return (chdir_error(node, oldpwd, 1, in_pipe));
	make_updates(node, oldpwd);
	if (!in_pipe)
		close_all_redirs(node);
	return (set_exit_status(node, 0));
}
