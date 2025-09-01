/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:40:45 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/01 14:21:03 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_ast	*create_subshell_node(t_ast *child, t_shell *data)
{
	t_ast	*node;

	node = create_ast_node(NODE_SUBSHELL, *data);
	if (!node)
		return (NULL);
	node->children = (t_ast **) malloc(2 * sizeof(t_ast *));
	if (!node->children)
	{
		free(node);
		return (NULL);
	}
	node->children[0] = child;
	node->children[1] = NULL;
	return (node);
}

t_ast	*create_operator_node(t_node_type type, t_ast *left, t_ast *right, t_shell *data)
{
	t_ast	*node;

	node = create_ast_node(type, *data);
	if (!node)
		return (NULL);	// starting to lose track of malloc'ed elements at this point...
	node->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!node->children)
	{
		free(node);
		return (NULL);
	}
	node->children[0] = left;
	node->children[1] = right;
	node->children[2] = NULL;
	return (node);
}

t_ast	*create_cmd_node(char **args, t_token **tokens, t_shell *data)
{
	t_ast	*node;

	node = create_ast_node(NODE_CMD, *data);
	if (!node)
	{
		free_str_array(args);
		malloc_error(NULL, data, tokens);	// will need to add a pointer to root to free already malloc'ed nodes right ?
	}
	node->cmd.args = args;
	return (node);
}
