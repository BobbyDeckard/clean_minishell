/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:58:32 by imeulema          #+#    #+#             */
/*   Updated: 2025/11/02 15:14:19 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_n_type	convert_types(t_t_type type);
t_ast		*create_node(t_shell *shell, t_n_type type);
t_ast		*parse_command(t_shell *shell, t_token **list, int start, int end);
t_ast		*parse_pipe(t_shell *shell, t_token **list, int start, int end);
t_ast		*parse_sequence(t_shell *shell, t_token **list, int start, int end);

static int	get_op_precedence(t_t_type type)
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

	cur_prec = get_op_precedence(token->type);
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

	res = -1;
	prec = 2147483647;
	lvl = 0;
	while (start <= end)
	{
		current = get_token_at_index(list, start);
		if (!current)
			break ;
		else if (current->type == PAREN_OPEN)
			lvl++;
		else if (current->type == PAREN_CLOSE)
			lvl--;
		if (!lvl && (current->type == PIPE || current->type == AND
				|| current->type == OR))
			check_precedence(current, &res, &prec, start);
		start++;
	}
	return (res);
}

static t_ast	*create_op_node(t_shell *shell, t_n_type type)
{
	t_ast	*node;

	node = create_node(shell, type);
	if (!node)
		return (NULL);
	set_root(shell, node);
	node->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!node->children)
	{
		free(node);
		return (NULL);
	}
	node->children[0] = NULL;
	node->children[1] = NULL;
	node->children[2] = NULL;
	return (node);
}

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
	if (!node)
		return (NULL);
	set_root(shell, node);
	node->children[0] = parse_sequence(shell, shell->tokens, start,
			operator - 1);
	if (!node->children[0])
	{
		clean_ast(node);
		return (NULL);
	}
	node->children[1] = parse_sequence(shell, shell->tokens, operator + 1, end);
	if (!node->children[1])
	{
		clean_ast(node);
		return (NULL);
	}
	return (node);
}
