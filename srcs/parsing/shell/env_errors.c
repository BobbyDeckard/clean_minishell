/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 21:17:24 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 14:39:12 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	create__error(char **envp)
{
	int	i;

	i = -1;
	while (++i < 3)
		free(envp[i]);
	free(envp);
}

void	set_shlvl_malloc_error(char **env_cpy, int i)
{
	int	j;

	j = -1;
	while (++j != i && env_cpy[j])
		free(env_cpy[j]);
	while (env_cpy[++j])
		free(env_cpy[j]);
	free(env_cpy);
	malloc_error(NULL, NULL, NULL);
}

void	env_cpy_malloc_error(char **env_cpy, int i)
{
	while (--i >= 0)
		free(env_cpy[i]);
	free(env_cpy);
	malloc_error(NULL, NULL, NULL);
}
