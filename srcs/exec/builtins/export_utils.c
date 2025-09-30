/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:16:44 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/30 18:28:56 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	create_env_error(t_ast *node, char **new, int i)
{
	while (--i >= 0)
		free(new[i]);
	free(new);
	malloc_error(node, node->data, NULL);
}

int	var_exists(char *name, char **envp)
{
	int	i;
	int	j;

	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (name[j] && name[j] == envp[i][j])
			j++;
		if (!name[j] && (!envp[i][j] || envp[i][j] == '='))
			return (1);
	}
	return (0);
}
