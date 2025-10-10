/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 21:22:40 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/10 16:32:24 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	too_many_args_cd(t_ast *node, int in_pipe)
{
	ft_putstr_fd("minishell: cd: too many arguments\n", 2);
	if (!in_pipe)
		close_all_redirs(node);
	return (set_exit_status(node, 1));
}

int	cd_home(t_ast *node, int in_pipe)
{
	char	*home;

	home = get_entry(node->data->envp, "HOME");
	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		set_exit_status(node, 1);
	}
	else if (chdir(home) < 0)
	{
		perror("chdir");
		set_exit_status(node, 1);
	}
	else
		set_exit_status(node, 0);
	if (!in_pipe)
		close_all_redirs(node);
	return (node->data->exit_status);
}

char	*cd_error(t_ast *node, int arg)
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

// Once unset, OLDPWD never comes back, unless export OLDPWD is called.
// Then, it is masked until set ?
void	update_pwd(t_ast *node, int i, char *oldpwd)
{
	char	**envp;
	char	*cwd;
	int		len;

	free(oldpwd);
	envp = node->data->envp;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		getcwd_error(node);
	free(envp[i]);
	len = ft_strlen(cwd) + 5;
	envp[i] = (char *) malloc(len * sizeof(char));
	if (!envp[i])
	{
		free(cwd);
		malloc_error(node, node->data, NULL);
	}
	ft_strlcpy(envp[i], "PWD=", len);
	ft_strlcat(envp[i], cwd, len);
	free(cwd);
}

// At launch, OLDPWD shouldn't already be set.
// If unset PWD is called before OLDPWD is initialized, cd once then env
// prints no WD variable, cd once more then env and there is OLDPWD correctly
// set.
// If unset PWD is called after OLDPWD is initialized, env only prints OLDPWD
// until cd is called, then env will not print any WD variable until cd is
// called once again, then env will only print OLDPWD correctly set.
void	update_oldpwd(t_ast *node, int i, char *oldpwd)
{
	char	**envp;
	int		len;

	envp = node->data->envp;
	free(envp[i]);
	len = ft_strlen(oldpwd) + 8;
	envp[i] = (char *) malloc(len * sizeof(char));
	if (!envp[i])
	{
		free(oldpwd);
		malloc_error(node, node->data, NULL);
	}
	ft_strlcpy(envp[i], "OLDPWD=", len);
	ft_strlcat(envp[i], oldpwd, len);
	free(oldpwd);
}
