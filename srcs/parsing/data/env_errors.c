/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 21:17:24 by imeulema          #+#    #+#             */
/*   Updated: 2025/08/28 11:15:02 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	set_shlvl_malloc_error(char **env_cpy, int i)
{
	int	j;

	j = -1;
	while (++j != i && env_cpy[j])
		free(env_cpy[j]);
	while (env_cpy[++j])
		free(env_cpy[j]);
	free(env_cpy);
	malloc_error(NULL);
}

void	env_cpy_malloc_error(char **env_cpy, int i)
{
	while (--i >= 0)
		free(env_cpy[i]);
	free(env_cpy);
	malloc_error(NULL);
}
