/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 20:12:58 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/06 15:12:34 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	find_matching_parentheses(t_token **tokens, int start, int end)
{
	t_token	*current;
	int		lvl;
	int		i;

	current = get_token_at_index(tokens, start);
	if (!current || current->type != PAREN_OPEN)
		return (-1);
	lvl = 1;
	i = start;
	while (++i <= end)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		else if (current->type == PAREN_OPEN)
			lvl++;
		else if (current->type == PAREN_CLOSE)
		{
			lvl--;
			if (!lvl)
				return (i);
		}
	}
	return (-1);
}

static int	check_between_parentheses(t_token **tokens, int start, int end)
{
	t_token	*current;
	int		lvl;

	lvl = 0;
	while (start <= end)
	{
		current = get_token_at_index(tokens, start);
		if (!current)
			break ;
		else if (current->type == PAREN_OPEN)
			lvl++;
		else if (current->type == PAREN_CLOSE)
		{
			lvl--;
			if (lvl == 0 && start != end)
				return (0);
		}
		start++;
	}
	return (lvl == 0);
}

int	matching_parentheses(t_token **tokens, int start, int end)
{
	t_token	*current;

	current = get_token_at_index(tokens, start);
	if (!current || current->type != PAREN_OPEN)
		return (0);
	current = get_token_at_index(tokens, end);
	if (!current || current->type != PAREN_CLOSE)
		return (0);
	return (check_between_parentheses(tokens, start, end));
}

/*
static void	apply_redirs(t_shell *data, t_ast *node, t_redir *redirs, int count)
{
	int	i;

	if (node->type == NODE_CMD)
		set_trailing_redirs(data, node, redirs, count);
	else if (node->children)
	{
		i = -1;
		while (node->children[++i])
			apply_redirs(data, node->children[i], redirs, count);
	}
}
*/

t_ast	*parse_parentheses(t_token **tokens, int start, int end, t_shell *data)
{
//	t_token	*current;
//	t_redir	*redirs;
	t_ast	*node;
	int		closing;
//	int		count;

	node = NULL;
	closing = find_matching_parentheses(tokens, start, end);
	if (closing > start && closing <= end)
		node = parse_command_line(tokens, start + 1, closing - 1, data);
//	current = get_token_at_index(tokens, closing + 1);
//	count = count_trailing_redirs(current);
//	redirs = find_trailing_redirs(tokens, closing + 1, count, data);
//	apply_redirs(data, node, redirs, count);
	closing = -1;
//	while (++closing < count)
//		free(redirs[closing].file);
//	free(redirs);
	return (node);
}
