/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 17:07:01 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/05 15:40:58 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// REMOVE FILE FROM FINAL PROJECT

void	print_node_type(t_node_type type)
{
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
	printf("Address of data: %p\n", node->data);
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
			printf("\t%s\n", node->cmd.args[i]);
			if (node->cmd.exp[i])
				printf("\tNeeds expansion (%d)\n", node->cmd.exp[i]);
			if (node->cmd.cat[i])
				printf("\tNeeds to be cat\n");
		}
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

void	print_token_type(t_token_type type)
{
	if (type == DEFAULT)
		printf("DEFAULT");
	else if (type == WORD)
		printf("WORD");
	else if (type == WORD_CAT)
		printf("WORD_CAT");
	else if (type == SPACE_TKN)
		printf("SPACE");
	else if (type == ASSIGNMENT)
		printf("ASSIGNMENT");
	else if (type == QUOTE)
		printf("QUOTE");
	else if (type == SINGLE_QUOTE)
		printf("SINGLE_QUOTE");
	else if (type == DOUBLE_QUOTE)
		printf("DOUBLE_QUOTE");
	else if (type == REDIR)
		printf("REDIR");
	else if (type == REDIR_IN)
		printf("REDIR_IN");
	else if (type == REDIR_OUT)
		printf("REDIR_OUT");
	else if (type == REDIR_APPEND)
		printf("REDIR_APPEND");
	else if (type == HEREDOC || type == HEREDOC_EXP)
		printf("HEREDOC");
	else if (type == OPERATOR)
		printf("OPERATOR");
	else if (type == PIPE)
		printf("PIPE");
	else if (type == AND)
		printf("AND");
	else if (type == OR)
		printf("OR");
	else if (type == SEMICOLON)
		printf("SEMICOLON");
	else if (type == PARENTHESIS)
		printf("PARENTHESIS");
	else if (type == PAREN_OPEN)
		printf("PAREN_OPEN");
	else if (type == PAREN_CLOSE)
		printf("PAREN_CLOSE");
	else if (type == SPECIAL_CHARACTER)
		printf("SPECIAL_CHARACTER");
	else if (type == ENV_VAR)
		printf("ENV_VAR");
	else if (type == EXIT_STATUS)
		printf("EXIT_STATUS");
	else if (type == ESCAPE)
		printf("ESCAPE");
	else if (type == COMMENT)
		printf("COMMENT");
	else if (type == T_NEWLINE)
		printf("T_NEWLINE");
	else if (type == T_EOF)
		printf("T_EOF");
	else if (type == UNKNOWN)
		printf("UNKNOWN");
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
