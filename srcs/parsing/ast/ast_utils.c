/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:00:51 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/01 20:23:25 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	set_root_node(t_ast *ast, t_ast *root)
{
	int	i;

	ast->root = root;
	if (ast->children)
	{
		i = -1;
		while (ast->children[++i])
			set_root_node(ast->children[i], root);
	}
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
	else if (type == REDIR_IN)
		return (NODE_REDIR_IN);
	else if (type == REDIR_OUT)
		return (NODE_REDIR_OUT);
	else if (type == REDIR_APPEND)
		return (NODE_REDIR_APPEND);
	else if (type == HEREDOC)
		return (NODE_HEREDOC);
	return (NODE_CMD);
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
