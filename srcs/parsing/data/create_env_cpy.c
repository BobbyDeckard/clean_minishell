/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env_cpy.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:59:32 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/29 13:32:33 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void	create__(char **envp)
{
	envp[2] = (char *) malloc(15 * sizeof(char));
	if (!envp[2])
	{
		free(envp[1]);
		free(envp[0]);
		free(envp);
		malloc_error(NULL, NULL, NULL);
	}
	ft_strlcat(envp[2], "_=/usr/bin/env", 15);
}

static void	create_shlvl(char **envp)
{
	envp[1] = (char *) malloc(8 * sizeof(char));
	if (!envp[1])
	{
		free(envp[0]);
		free(envp);
		malloc_error(NULL, NULL, NULL);
	}
	ft_strlcat(envp[1], "SHLVL=1", 8);
}

static void	create_pwd(char **envp)
{
	char	*cwd;
	int		len;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		getcwd_error(envp);
	len = ft_strlen(cwd + 5);
	envp[0] = (char *) malloc(len * sizeof(char));
	if (!envp[0])
	{
		free(envp);
		free(cwd);
		malloc_error(NULL, NULL, NULL);
	}
	ft_strlcat(envp[0], "PWD=", len);
	ft_strlcat(envp[0], cwd, len);
	free(cwd);
}

char	**create_env_cpy(void)
{
	char	**envp;
	int		len;

	envp = (char **) malloc(4 * sizeof(char *));
	if (!envp)
		malloc_error(NULL, NULL, NULL);
	create_pwd(envp);
	create_shlvl(envp);
	create__(envp);
	envp[3] = NULL;
	return (envp);
}
