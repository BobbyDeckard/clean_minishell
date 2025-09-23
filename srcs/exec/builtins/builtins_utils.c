/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 22:43:29 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/23 23:05:54 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	update_bltn(t_shell *data)
{
	char	*minipath;
	int		len;
	int		i;

	i = -1;
	while (data->envp[++i])
	{
		if (!ft_strncmp(data->envp[i], "_=", 2))
		{
			free(data->envp[i]);
			minipath = get_minipath(NULL);
			len = ft_strlen(minipath) + 3;
			data->envp[i] = (char *) malloc(len * sizeof(char));
			if (!data->envp[i])
				update_error(data, NULL, i);
			ft_strlcpy(data->envp[i], "_=", len);
			ft_strlcat(data->envp[i], minipath, len);
			break ;
		}
	}
}
