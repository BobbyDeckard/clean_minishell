/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 20:17:06 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/11 22:39:16 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_ast	*create_ast_node(t_shell *data, t_node_type type)
{
	t_ast	*node;

	node = (t_ast *) malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->data = data;
	node->file = NULL;
	node->children = NULL;
	node->root = NULL;
	node->cmd.args = NULL;
	node->cmd.path = NULL;
	node->cmd.fd_in = STDIN_FILENO;
	node->cmd.fd_out = STDOUT_FILENO;
	return (node);
}

t_ast	*create_subshell_node(t_ast *child, t_shell *data)
{
	t_ast	*node;

	printf("Creating subshell node\n");
	node = create_ast_node(data, NODE_SUBSHELL);
	if (!node)
		return (NULL);
	node->children = (t_ast **) malloc(2 * sizeof(t_ast *));
	if (!node->children)
	{
		free(node);
		return (NULL);
	}
	data->root = node;
	set_root_node(node, node);
	node->children[0] = child;
	node->children[1] = NULL;
	return (node);
}

t_ast	*create_operator_node(t_node_type type, t_ast *left, t_ast *right,
t_shell *data)
{
	t_ast	*node;

	node = create_ast_node(data, type);
	if (!node)
		return (NULL);
	node->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!node->children)
	{
		free(node);
		return (NULL);
	}
	data->root = node;
	set_root_node(node, node);
	node->children[0] = left;
	node->children[1] = right;
	node->children[2] = NULL;
	return (node);
}

t_ast	*create_cmd_node(t_shell *data, t_token **tokens, t_cmd cmd)
{
	t_ast	*node;

	node = create_ast_node(data, NODE_CMD);
	if (!node)
	{
		free_char_array(cmd.args);
		malloc_error(data->root, data, tokens);
	}
	data->root = node;
	set_root_node(node, node);
	node->cmd.args = cmd.args;
	node->cmd.exp = cmd.exp;
	node->cmd.arg_count = cmd.arg_count;
	return (node);
}

t_ast	*create_redir_node(t_node_type type, char *file, t_shell *data)
{
	t_ast	*node;

	node = create_ast_node(data, type);
	if (!node)
		return (NULL);
	data->root = node;
	set_root_node(node, node);
	node->file = file;
	return (node);
}
