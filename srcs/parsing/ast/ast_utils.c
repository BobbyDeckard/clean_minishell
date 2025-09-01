/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:21:20 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/01 14:09:37 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	get_operator_precedence(t_token_type type)
{
	if (type == AND || type == OR)
		return (1);
	else if (type == PIPE)
		return (2);
	return (3);
}

static void	check_lowest_precedence(t_token *current, int *res, int *low_prec, int i)
{
	int	cur_prec;

	cur_prec = get_operator_precedence(current->type);
	if (cur_prec <= *low_prec)
	{
		*low_prec = cur_prec;
		*res = i;
	}
}

int	find_lowest_precedence_op(t_token **tokens, int i, int end)
{
	t_token	*current;
	int		paren_lvl;
	int		low_prec;
	int		cur_prec;
	int		res;

	res = -1;
	low_prec = 2147483647;
	paren_lvl = 0;
	while (i <= end)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		else if (current->type == PAREN_OPEN)
			paren_lvl++;
		else if (current->type == PAREN_CLOSE)
			paren_lvl--;
		if (!paren_lvl && (current->type == PIPE || current->type == AND || current->type == OR))
			check_lowest_precedence(current, &res, &low_prec, i);
		i++;
	}
	return (res);
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
