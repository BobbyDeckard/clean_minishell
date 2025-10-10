/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env_cpy.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:59:32 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/10 16:01:55 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static char	*create__(char **envp)
{
	char	*cwd;
	char	*new;
	int		len;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		create__error(envp);
	}
	len = ft_strlen(cwd) + 13;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
	{
		free(cwd);
		create__error(envp);
		malloc_error(NULL, NULL, NULL);
	}
	ft_strlcpy(new, "_=", len);
	ft_strlcat(new, cwd, len);
	ft_strlcat(new, "/minishell", len);
	return (new);
}

static char	*create_shlvl(char **envp)
{
	char	*new;

	new = (char *) malloc(8 * sizeof(char));
	if (!new)
	{
		free(envp[0]);
		free(envp[1]);
		free(envp);
		malloc_error(NULL, NULL, NULL);
	}
	ft_strlcpy(new, "SHLVL=1", 8);
	return (new);
}

static char	*create_pwd(char **envp)
{
	char	*new;
	char	*cwd;
	int		len;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		free(envp[0]);
		free(envp);
		exit(1);
	}
	len = ft_strlen(cwd) + 5;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
	{
		free(envp[0]);
		free(envp);
		free(cwd);
		malloc_error(NULL, NULL, NULL);
	}
	ft_strlcpy(new, "PWD=", len);
	ft_strlcat(new, cwd, len);
	free(cwd);
	return (new);
}

static char	*create_oldpwd(void)
{
	char	*old;

	old = (char *) malloc(7 * sizeof(char));
	if (!old)
		malloc_error(NULL, NULL, NULL);
	ft_strlcpy(old, "OLDPWD", 7);
	return (old);
}

char	**create_env_cpy(t_shell *data)
{
	char	**envp;

	envp = (char **) malloc(5 * sizeof(char *));
	if (!envp)
		malloc_error(NULL, NULL, NULL);
	envp[0] = create_oldpwd();
	envp[1] = create_pwd(envp);
	envp[2] = create_shlvl(envp);
	envp[3] = create__(envp);
	envp[4] = NULL;
	data->shlvl = 1;
	return (envp);
}
