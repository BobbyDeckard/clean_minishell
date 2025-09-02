/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:11:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/01 14:07:24 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	count_args(t_token **tokens, int start, int end)
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
		else if (current->type == WORD)
			count++;
		else if (is_redir_token(current))
			i++;
	}
	return (count);
}

static char	**extract_args(t_token **tokens, int start, int end, t_shell *data)
{
	t_token	*current;
	char	**args;
	int		count;
	int		i;
	int		j;

	count = count_args(tokens, start, end);
	args = (char **) malloc((count + 1) * sizeof(char *));
	if (!args)
		malloc_error(data->root, data, tokens);
	i = start - 1;
	j = -1;
	while (++i <= end && ++j < count)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		if (current->type == WORD)
			args[j] = sf_strdup(current->content, tokens, args, data);
		else if (is_redir_token(current))
			i++;
	}
	args[j] = NULL;
	return (args);
}

t_ast	*parse_command(t_token **tokens, int start, int end, t_shell *data)
{
	t_ast	**redirs;
	t_ast	*node;
	char	**args;

	args = extract_args(tokens, start, end, data);
	if (!args)
		return (NULL);
	node = create_cmd_node(args, tokens, data);
	redirs = extract_redirs(data, args, start, end);
	if (redirs)
		node->children = redirs;
	return (node);
}
