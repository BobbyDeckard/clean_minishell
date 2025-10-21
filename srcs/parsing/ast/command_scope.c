/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_scope.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:40:51 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/20 17:02:57 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_redir_arg(t_token *token);
int		is_redir_token(t_t_type type);

static t_token	*update_start(t_token *current, int *start)
{
	(*start)++;
	return (current->next);
}

static int	find_cmd_start(t_token *current, int start, int end)
{
	while (current && start <= end && is_redir_token(current->type))
	{
		current = update_start(current, &start);
		while (current && start <= end && current->type == WHITESPACE)
			current = update_start(current, &start);
		while (current && start <= end && is_redir_arg(current))
			current = update_start(current, &start);
		while (current && start <= end && current->type == WHITESPACE)
			current = update_start(current, &start);
	}
	return (start);
}

static int	find_cmd_end(t_token *current, int start, int end)
{
	while (current && start <= end && !is_redir_token(current->type))
		current = update_start(current, &start);
	if (current)
		return (--start);
	return (end);
}

//	smfh bash on my computer does receive args after redirs
//	Test on school's computers and maybe remove this function
void	find_cmd_scope(t_ast *node, t_token **list, int *start, int *end)
{
	t_token	*current;
	int		i;

	if (!node->children)
		return ;
	i = *start;
	current = get_token_at_index(list, *start);
	while (current && current->type == WHITESPACE && i <= *end)
	{
		current = current->next;
		i++;
	}
	if (is_redir_token(current->type))
		*start = find_cmd_start(current, *start, *end);
	*end = find_cmd_end(current, *start, *end);
}
