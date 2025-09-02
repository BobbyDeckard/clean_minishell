/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 20:17:06 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/01 20:18:40 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_ast	*create_ast_node(t_node_type type)
{
	t_ast	*node;

	node = (t_ast *) malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
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

	node = create_ast_node(NODE_SUBSHELL);
	if (!node)
		return (NULL);
	node->children = (t_ast **) malloc(2 * sizeof(t_ast *));
	if (!node->children)
	{
		free(node);
		return (NULL);
	}
	else if (!data->root)
		data->root = node;
	else
		set_root_node(node, data->root);
	node->children[0] = child;
	node->children[1] = NULL;
	return (node);
}

t_ast	*create_operator_node(t_node_type type, t_ast *left, t_ast *right,
t_shell *data)
{
	t_ast	*node;

	node = create_ast_node(type);
	if (!node)
		return (NULL);
	node->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!node->children)
	{
		free(node);
		return (NULL);
	}
	else if (!data->root)
		data->root = node;
	node->root = data->root;
	node->data = *data;
	node->children[0] = left;
	node->children[1] = right;
	node->children[2] = NULL;
	return (node);
}

t_ast	*create_cmd_node(char **args, t_token **tokens, t_shell *data)
{
	t_ast	*node;

	node = create_ast_node(NODE_CMD);
	if (!node)
	{
		free_str_array(args);
		malloc_error(data->root, data, tokens);
	}
	else if (!data->root)
		data->root = node;
	else
		set_root_node(node, data->root);
	node->cmd.args = args;
	return (node);
}

t_ast	*create_redir_node(t_node_type type, char *file, t_shell *data)
{
	t_ast	*node;

	node = create_ast_node(type);
	if (!node)
		return (NULL);
	else if (!data->root)
		data->root = node;
	else
		set_root_node(node, data->root);
	node->file = file;
	return (node);
}
