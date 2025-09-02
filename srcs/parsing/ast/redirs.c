/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 20:14:43 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/01 20:16:24 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

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

static t_ast	**free_redirs(t_ast **redirs, int i)
{
	while (--i >= 0)
		free(redirs[i]);
	free(redirs);
	return (NULL);
}

static	t_ast	*redir_node_helper(t_shell *data, t_token *current,
t_token *target)
{
	t_node_type	type;
	t_ast		*node;
	char		*content;

	type = convert_types(current->type);
	content = ft_strdup(target->content);
	if (!content)
		return (NULL);
	node = create_redir_node(type, content, data);
	if (!node)
		free(content);
	return (node);
}

static t_ast	**extract_redirs_body(t_ast **redirs, t_shell *data, int sec[3])
{
	t_token	*current;
	t_token	*target;
	int		j;

	--(sec[0]);
	j = 0;
	while (++(sec[0]) <= sec[1] && j < sec[2])
	{
		current = get_token_at_index(data->tokens, sec[0]);
		if (!current)
			break ;
		else if (is_redir_token(current) && sec[0] + 1 <= sec[1])
		{
			target = get_token_at_index(data->tokens, sec[0] + 1);
			if (target && target->type == WORD)
			{
				redirs[j] = redir_node_helper(data, current, target);
				if (!redirs[j])
					return (free_redirs(redirs, j));
				j++;
			}
			(sec[0])++;
		}
	}
	return (redirs);
}

t_ast	**extract_redirs(t_shell *data, char **args, int start, int end)
{
	t_token	**tokens;
	t_ast	**redirs;
	int		sec[3];
	int		count;

	tokens = data->tokens;
	count = count_redirs(tokens, start, end);
	sec[0] = start;
	sec[1] = end;
	sec[2] = count;
	if (!count)
		return (NULL);
	redirs = (t_ast **) malloc(++count * sizeof(t_ast *));
	if (!redirs)
	{
		free_str_array(args);
		malloc_error(data->root, data, tokens);
	}
	redirs = extract_redirs_body(redirs, data, sec);
	if (!redirs)
	{
		free_str_array(args);
		malloc_error(data->root, data, tokens);
	}
	return (redirs);
}
