/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 17:07:01 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/02 17:12:16 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

static void	print_node_type(t_ast node)
{
	int type;

	type = node.type;
	if (type == 0)
		printf("Command node\n");
	else if (type == 1)
		printf("Pipe node\n");
	else if (type == 2)
		printf("And-if node\n");
	else if (type == 3)
		printf("Or-if node\n");
	else if (type == 4)
		printf("Redir in node\n");
	else if (type == 5)
		printf("Redir out node\n");
	else if (type == 6)
		printf("Redir append node\n");
	else if (type == 7)
		printf("Heredoc node\n");

}

// imprime les enfants de la node actuelle
static void	print_child(t_ast node)
{
	print_node_type(node);
	if (node.type == NODE_CMD)
		printf("%s\n", node.cmd.args[0]);
}

// imprime chaque information pertinente pour la node actuelle
static void	print_node_info(t_ast *node)
{
	int	i;

	print_node_type(*node);
	printf("Pointer address: %p\n", node);
	if (node->root)
	{
		printf("Root: ");
		print_node_type(*node->root);
	}
	if (node->cmd.path)
		printf("Path:\n\t%s\n", node->cmd.path);
	if (node->cmd.args)
	{
		printf("Arguments:\n");
		i = -1;
		while (node->cmd.args[++i])
			printf("\t%s\n", node->cmd.args[i]);
	}
	if (node->type == NODE_CMD)
		printf("fd_in = %d\nfd_out = %d\n", node->cmd.fd_in, node->cmd.fd_out);
	if (node->file)
		printf("File: %s\n", node->file);
	if (node->children)
	{
		printf("Children:\n");
		i = -1;
		while (node->children[++i])
			print_child(*(node->children[i]));
	}
	printf("\n");
}

// fonction récursive qui permet d'imprimer la totalité de l'arbre à partir de sa racine
void	print_tree(t_ast *ast)
{
	int	i;

	print_node_info(ast);
	if (ast->children)
	{
		i = -1;
		while (ast->children[++i])
			print_tree(ast->children[i]);
	}
}
