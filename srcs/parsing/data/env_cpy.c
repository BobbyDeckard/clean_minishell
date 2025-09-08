/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cpy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:38:40 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/08 12:18:42 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	count_digits(int lvl)
{
	int	dig;

	dig = 0;
	while (lvl > 0)
	{
		lvl /= 10;
		dig++;
	}
	return (dig);
}

static void	set_shlvl(char **env_cpy)
{
	char	*level;
	int		len;
	int		lvl;
	int		i;

	i = -1;
	while (env_cpy[++i])
	{
		if (!ft_strncmp(env_cpy[i], "SHLVL=", 6))
		{
			lvl = ft_atoi(env_cpy[i] + 6) + 1;
			free(env_cpy[i]);
			len = count_digits(lvl) + 8;
			env_cpy[i] = (char *) malloc(len * sizeof(char));
			if (!env_cpy)
				set_shlvl_malloc_error(env_cpy, i);
			ft_strlcat(env_cpy[i], "SHLVL=", len);
			level = ft_itoa(lvl);
			ft_strlcat(env_cpy[i], level, len);
			free(level);
		}
	}
}

static char	**init_env_cpy(char **envp, int entries)
{
	char	**env_cpy;
	int		len;
	int		i;

	env_cpy = (char **) malloc((entries + 1) * sizeof(char *));
	if (!env_cpy)
		malloc_error(NULL, NULL, NULL);
	i = -1;
	while (envp[++i])
	{
		len = ft_strlen(envp[i]) + 1;
		env_cpy[i] = (char *) malloc(len * sizeof(char));
		if (!env_cpy[i])
			env_cpy_malloc_error(env_cpy, i);
		ft_strlcat(env_cpy[i], envp[i], len);
	}
	env_cpy[i] = NULL;
	set_shlvl(env_cpy);
	return (env_cpy);
}

char	**copy_env(char **envp)
{
	char	**env_cpy;
	int		i;

	if (!envp)
		exit(1);
	else if (!*envp)
		return (create_env_cpy());
	i = 0;
	while (envp[i])
		i++;
	env_cpy = init_env_cpy(envp, i);
	set_shlvl(env_cpy);
	return (env_cpy);
}
