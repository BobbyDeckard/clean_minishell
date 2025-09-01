/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:23:25 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/01 13:34:29 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static t_ast *create_redir_node(t_node_type type, char *file, t_shell data)
{
	t_ast	*node;

	node = create_ast_node(type, data);
	if (!node)
		return (NULL);
	node->file = file;
	return (node);
}

int	count_redirs(t_token **tokens, int start, int end)
{
	t_token	*current;
	int		count;
	int		i;

	count = 0;
	i = start - 1;
	while (++i <= end)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		if (is_redir_token(current) && i + 1 <= end)
		{
			current = get_token_at_index(tokens, i + 1);
			if (current && current->type == WORD)
				count++;
		}
	}
	return (count);
}

static	t_ast	*redir_node_helper(t_ast **redirs, t_shell *data, t_token *current, t_token *target, int j)
{
	t_node_type	type;
	t_ast		*node;
	char		*content;

	type = convert_types(current->type);
	content = ft_strdup(target->content);
	if (!content)
	{
		while (--j >= 0)
			free(redirs[j]);
		free(redirs);
		return (NULL);
	}
	node = create_redir_node(type, content, *data);
	if (!node)
	{
		while (--j >= 0)
			free(redirs[j]);
		free(redirs);
	}
	return (node);
}

t_ast	**extract_redirs_body(t_ast **redirs, t_shell *data, int start, int end, int count)
{
	t_token	*current;
	t_token	*target;
	int		j;

	--start;
	j = 0;
	while (++start <= end && j < count)
	{
		current = get_token_at_index(data->tokens, start);
		if (!current)
			break ;
		else if (is_redir_token(current) && start + 1 <= end)
		{
			target = get_token_at_index(data->tokens, start + 1);
			if (target && target->type == WORD)
			{
				redirs[j] = redir_node_helper(redirs, data, current, target, j);
				if (!redirs[j])
					return (NULL);
				j++;
			}
			start++;
		}
	}
	return (redirs);
}
