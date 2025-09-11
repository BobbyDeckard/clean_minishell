/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:44:46 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/08 17:47:10 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	count_nodes(t_ast **children)
{
	int	i;

	i = 0;
	while (children[i])
		i++;
	return (i);
}

int	*init_pids(t_ast *root, int count)
{
	int	*pids;
	int	i;

	pids = (int *) malloc(count * sizeof(int));
	if (!pids)
		malloc_error(root, root->data, NULL);
	i = -1;
	while (++i < count)
		pids[i] = -1;
	return (pids);
}
