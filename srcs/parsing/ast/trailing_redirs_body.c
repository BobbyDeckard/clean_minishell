/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trailing_redirs_body.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:31:41 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/30 16:32:46 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	free_new(t_ast **new, int i)
{
	while (--i >= 0)
		free(new[i]);
	free(new);
}

int	count_children(t_ast *node)
{
	int	i;

	i = 0;
	while (node->children && node->children[i++])
		continue ;
	return (i);
}

t_ast	**malloc_new(t_ast *node, t_redir *redirs, int count, int size)
{
	t_ast	**new;

	new = (t_ast **) malloc(size * sizeof(t_ast *));
	if (!new)
		trailing_redir_error(node, node->data, redirs, count);
	return (new);
}

char	*get_file(t_redir *redirs, int j, t_ast **new, int i)
{
	char	*file;

	file = ft_strdup(redirs[j].file);
	if (!file)
		free_new(new, i);
	return (file);
}

void	replace_children(t_ast *node, t_ast **new)
{
	if (node->children)
		free(node->children);
	node->children = new;
}
