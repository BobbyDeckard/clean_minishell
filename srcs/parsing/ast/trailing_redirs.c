/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trailing_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:01:47 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/30 17:07:13 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_redir	*find_trailing_redirs(t_token **tokens, int start, int count,
t_shell *data)
{
	t_token	*current;
	t_redir	*redirs;
	int		i;

	current = get_token_at_index(tokens, start);
	if (!current)
		return (NULL);
	redirs = (t_redir *) malloc(count * sizeof(t_redir));
	if (!redirs)
		malloc_error(NULL, data, tokens);
	i = -1;
	while (++i < count)
	{
		redirs[i].type = convert_redir_type(current->type);
		current = current->next;
		redirs[i].file = ft_strdup(current->content);
		if (!redirs[i].file)
			trailing_redir_error(NULL, data, redirs, count);
	}
	return (redirs);
}

void	set_trailing_redirs(t_shell *data, t_ast *node, t_redir *redirs,
int count)
{
	t_ast	**new;
	char	*file;
	int		tot;
	int		i;
	int		j;

	i = count_children(node);
	tot = i + count;
	new = malloc_new(node, redirs, count, tot + 1);
	i = copy_existing_children(new, node, i);
	j = -1;
	while (++i < tot && ++j < count)
	{
		file = ft_strdup(redirs[j].file);
		if (!file)
			trailing_redir_error(node, data, redirs, count);
		new[i] = create_redir_node(redirs[j].type, file, data);
		if (!new[i])
		{
			free_new(new, i);
			trailing_redir_error(node, data, redirs, count);
		}
	}
	new[i] = NULL;
	replace_children(node, new);
}
