/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:31:30 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/16 08:11:03 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_token	*get_token_at_index(t_token **list, int index)
{
	t_token	*current;
	int		i;

	if (!list || !*list || index < 0)
		return (NULL);
	current = *list;
	i = -1;
	while (current && ++i < index)
		current = current->next;
	return (current);
}

t_n_type	convert_types(t_t_type type)
{
	if (type == REDIR_APPEND)
		return (NODE_REDIR_APPEND);
	else if (type == REDIR_OUT)
		return (NODE_REDIR_OUT);
	else if (type == REDIR_IN)
		return (NODE_REDIR_IN);
	else if (type == HEREDOC)
		return (NODE_HEREDOC);
	else if (type == PIPE)
		return (NODE_PIPE);
	else if (type == AND)
		return (NODE_AND);
	else if (type == OR)
		return (NODE_OR);
	return (-1);
}

void	set_root(t_shell *shell, t_ast *node)
{
	if (!shell->root)
		shell->root = node;
}

t_ast	*create_node(t_shell *shell, t_n_type type)
{
	t_ast	*node;

	node = (t_ast *) malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->shell = shell;
	node->children = NULL;
	node->root = NULL;
	node->cmd.args = NULL;
	node->cmd.path = NULL;
	node->cmd.fd_in = STDIN_FILENO;
	node->cmd.fd_out = STDOUT_FILENO;
	node->rdr.args = NULL;
	node->rdr.file = NULL;
	node->rdr.fd = -1;
	node->rdr.type = -1;
	return (node);
}
