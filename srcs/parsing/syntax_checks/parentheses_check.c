/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:44:21 by imeulema          #+#    #+#             */
/*   Updated: 2025/08/31 14:02:10 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	check_token(t_token **stack, t_token *current, int *top)
{
	if (current->type == PAREN_OPEN)
	{
		if (current->next && current->next->type == PAREN_CLOSE)
			return (0);
//			return (ft_putstr_fd("Empty parentheses:", 2), 0);
		stack[++(*top)] = current;
	}
	else if (current->type == PAREN_CLOSE)
	{
		if (*top < 0)
			return (0);
		(*top)--;
	}
	return (1);
}

int	valid_parentheses(t_shell *data, t_token **token_list)
{
	t_token	**stack;
	t_token	*current;
	t_token	*temp;
	int		top;

	stack = (t_token **) malloc(count_tokens(token_list) * sizeof(t_token));
	if (!stack)
		malloc_error(NULL, data, token_list);
	temp = *token_list;
	while (temp)
		temp = temp->next;
	top = -1;
	current = *token_list;
	while (current && check_token(stack, current, &top))
		current = current->next;
	if (top >= 0)
		return (free(stack), 0);
	return (free(stack), 1);
}
