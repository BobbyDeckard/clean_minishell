/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 17:07:01 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/16 11:18:45 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

// REMOVE FILE FROM FINAL PROJECT

void	print_node_type(t_n_type type)
{
	if (type == NODE_CMD)
		printf("Command node\n");
	else if (type == NODE_PIPE)
		printf("Pipe node\n");
	else if (type == NODE_AND)
		printf("And node\n");
	else if (type == NODE_OR)
		printf("Or node\n");
	else if (type == NODE_REDIR_IN)
		printf("Redir in node\n");
	else if (type == NODE_REDIR_OUT)
		printf("Redir out node\n");
	else if (type == NODE_REDIR_APPEND)
		printf("Redir append node\n");
	else if (type == NODE_HEREDOC)
		printf("Heredoc node\n");
}

// imprime les enfants de la node actuelle
static void	print_child(t_ast node)
{
	print_node_type(node.type);
	if (node.type == NODE_CMD)
		printf("%s\n", node.cmd.args[0]);
}

// imprime chaque information pertinente pour la node actuelle
static void	print_node_info(t_ast *node)
{
	int	i;

	print_node_type(node->type);
	printf("Pointer address: %p\n", node);
	printf("Address of data: %p\n", node->shell);
	if (node->root)
	{
		printf("Root: ");
		print_node_type(node->root->type);
	}
	if (node->cmd.path)
		printf("Path:\n\t%s\n", node->cmd.path);
	if (node->cmd.args)
	{
		printf("Arguments (%p):\n", node->cmd.args);
		i = -1;
		while (node->cmd.args[++i])
		{
			printf("node->cmd.args[%d]: %p\n", i, node->cmd.args[i]);
			printf("\t'%s'\n", node->cmd.args[i]);
		}
	}
	if (node->rdr.args)
	{
		printf("Arguments (%p):\n", node->rdr.args);
		i = -1;
		while (node->rdr.args[++i])
		{
			printf("node->rdr.args[%d]: %p\n", i, node->rdr.args[i]);
			printf("\t'%s'\n", node->rdr.args[i]);
		}
	}
	if (node->type == NODE_CMD)
		printf("fd_in = %d\nfd_out = %d\n", node->cmd.fd_in, node->cmd.fd_out);
	if (node->rdr.file)
		printf("File: %s\n", node->rdr.file);
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

void	print_token_type(t_t_type type)
{
	if (type == REDIR_APPEND)
		printf("REDIR_APPEND");
	else if (type == DOUBLE_QUOTE)
		printf("DOUBLE_QUOTE");
	else if (type == SINGLE_QUOTE)
		printf("SINGLE_QUOTE");
	else if (type == PAREN_CLOSE)
		printf("PAREN_CLOSE");
	else if (type == PAREN_OPEN)
		printf("PAREN_OPEN");
	else if (type == WHITESPACE)
		printf("WHITESPACE");
	else if (type == REDIR_OUT)
		printf("REDIR_OUT");
	else if (type == REDIR_IN)
		printf("REDIR_IN");
	else if (type == HEREDOC)
		printf("HEREDOC");
	else if (type == WORD)
		printf("WORD");
	else if (type == PIPE)
		printf("PIPE");
	else if (type == AND)
		printf("AND");
	else if (type == OR)
		printf("OR");
}

void	print_token_list(t_token **token_list)
{
	t_token	*current;
	int		token_count;

	token_count = 0;
	if (!token_list || !*token_list)
	{
		printf("No tokens to display\n");
		return ;
	}
	printf("--- Token List ---\n");
	current = *token_list;
	while (current)
	{
		printf("Token %d --> Type: ", token_count++);
		print_token_type(current->type);
		printf(", Content: '%s'\n", current->content);
		current = current->next;
	}
	printf("\n");
}
