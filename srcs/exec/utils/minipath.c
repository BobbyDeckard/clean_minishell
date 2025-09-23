/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minipath.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:12:48 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/23 23:21:10 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*get_minipath(char *str)
{
	static char	*path;

	if (!path && !str)
		return (NULL);
	else if (!path)
		path = str;
	return (path);
}

void	init_minipath(t_shell *data, char **envp)
{
	char	*path;
	int		len;
	int		i;

	i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "_=", 2))
		{
			len = ft_strlen(envp[i]) - 1;
			path = (char *) malloc(len * sizeof(char));
			if (!path)
				malloc_error(NULL, data, NULL);
			ft_strlcpy(path, envp[i] + 2, len);
			get_minipath(path);
			break ;
		}
	}
}
