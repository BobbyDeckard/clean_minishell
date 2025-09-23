/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trunc_cwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:40:32 by imeulema          #+#    #+#             */
/*   Updated: 2025/08/28 10:40:34 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

static void	bzero_cwd(char cwd[256])
{
	int	i;

	i = -1;
	while (++i < 256)
		cwd[i] = 0;
}

static void	trunc_cwd(char cwd[256], char full[256])
{
	int	i;

	i = 0;
	while (full[i])
		i++;
	while (full[i] != '/')
		i--;
	i++;
	bzero_cwd(cwd);
	ft_strlcat(cwd, "\e[1;36m", 256);
	ft_strlcat(cwd, full + i, 256);
	ft_strlcat(cwd, "\x1b[0m", 256);
	ft_strlcat(cwd, " ", 256);
}

void	get_trunc_cwd(char cwd[256], t_shell *data)
{
	char	full[256];

	bzero_cwd(full);
	getcwd(full, 256);
	if (!*full)
	{
		perror("getcwd");
		clean_data(data);
		exit(1);
	}
	trunc_cwd(cwd, full);
}
