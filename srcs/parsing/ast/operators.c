/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:58:32 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/16 08:39:37 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_n_type	convert_types(t_t_type type);
t_ast		*create_node(t_shell *shell, t_n_type type);
t_ast		*parse_command(t_shell *shell, t_token **list, int start, int end);
t_ast		*parse_pipe(t_shell *shell, t_token **list, int start, int end);
t_ast		*parse_sequence(t_shell *shell, t_token **list, int start, int end);
void		clean_ast(t_ast *ast);

static int get_op_precedence(t_t_type type)
{
	if (type == AND || type == OR)
		return (1);
	else if (type == PIPE)
			return (2);
	return (3);
}

static void	check_precedence(t_token *token, int *res, int *prec, int start)
{
	int	cur_prec;

//	printf("In check_precedence()\n");
	cur_prec = get_op_precedence(token->type);
//	printf("cur_prec = %d\n", cur_prec);
	if (cur_prec <= *prec)
	{
		*prec = cur_prec;
		*res = start;
	}
}

int	find_op_precedence(t_token **list, int start, int end)
{
	t_token	*current;
	int		prec;
	int		lvl;
	int		res;

	printf("\nIn find_op_precedence() with start = %d and end = %d\n", start, end);
	res = -1;
	prec = 2147483647;
	lvl = 0;
	while (start <= end)
	{
		current = get_token_at_index(list, start);
		printf("Checking token at index %d\n", start);
		if (!current)
			break ;
		else if (current->content)
			printf("Current has content '%s'\n", current->content);
		else
			printf("Current has no content\n");
		if (current->type == PAREN_OPEN)
		{
			lvl++;
			printf("Found paren_open token, lvl = %d\n",lvl);
		}
		else if (current->type == PAREN_CLOSE)
		{
			lvl--;
			printf("Found paren_close token, lvl = %d\n", lvl);
		}
		if (!lvl && (current->type == PIPE || current->type == AND || current->type == OR))
			check_precedence(current, &res, &prec, start);
		start++;
	}
	printf("\n");
	return (res);
}

static t_ast	*create_op_node(t_shell *shell, t_n_type type)
{
	t_ast	*node;

	node = create_node(shell, type);
	if (!node)
		malloc_error(shell->root, shell, shell->tokens);
	set_root(shell, node);
	node->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!node->children)
		malloc_error(shell->root, shell, shell->tokens);
	node->children[0] = NULL;
	node->children[1] = NULL;
	node->children[2] = NULL;
	return (node);
}

// Create node before creating children and have all of them linked to the tree in order to free properly with malloc_error
t_ast	*parse_operator(t_shell *shell, int start, int end, int operator)
{
	t_n_type	type;
	t_token		*current;
	t_ast		*node;

	current = get_token_at_index(shell->tokens, operator);
	if (!current)
		return (parse_command(shell, shell->tokens, start, end));
	else if (current->type == PIPE)
		return (parse_pipe(shell, shell->tokens, start, end));
	type = convert_types(current->type);
	node = create_op_node(shell, type);
	set_root(shell, node);
	node->children[0] = parse_sequence(shell, shell->tokens, start, operator - 1);
	node->children[1] = parse_sequence(shell, shell->tokens, operator + 1, end);
	return (node);
}
