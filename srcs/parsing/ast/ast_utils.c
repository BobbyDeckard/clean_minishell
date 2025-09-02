/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:00:51 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/01 11:39:58 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"
#include <unistd.h>

t_ast	*create_ast_node(t_node_type type, t_shell data)
{
	t_ast	*node;

	node = (t_ast *) malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->file = NULL;
	node->children = NULL;
	node->root = NULL;
	node->data = data;
	node->cmd.args = NULL;
	node->cmd.path = NULL;
	node->cmd.fd_in = STDIN_FILENO;
	node->cmd.fd_out = STDOUT_FILENO;
	return (node);
}

int	is_redir_token(t_token *token)
{
	if (!token)
		return (0);
	else if (token->type == REDIR_IN)
		return (1);
	else if (token->type == REDIR_OUT)
		return (1);
	else if (token->type == REDIR_APPEND)
		return (1);
	else if (token->type == HEREDOC)
		return (1);
	return (0);
}

t_node_type	convert_types(t_token_type type)
{
	if (type == PIPE)
		return (NODE_PIPE);
	else if (type == AND)
		return (NODE_AND_IF);
	else if (type == OR)
		return (NODE_OR_IF);
	return (NODE_CMD);
}

int	find_matching_parentheses(t_token **tokens, int open_pos, int end)
{
	t_token	*current;
	int		lvl;
	int		i;

	current = get_token_at_index(tokens, open_pos);
	if (!current || current->type != PAREN_OPEN)
		return (-1);
	lvl = 1;
	i = open_pos;
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

t_token	*get_token_at_index(t_token **tokens, int index)
{
	t_token	*current;
	int		i;

	if (!tokens || !*tokens || index < 0)
		return (NULL);
	current = *tokens;
	i = -1;
	while (current && ++i < index)
		current = current->next;
	return (current);
}
