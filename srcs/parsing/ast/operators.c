/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 20:19:10 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/06 18:24:19 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	get_operator_precedence(t_token_type type)
{
	if (type == AND || type == OR)
		return (1);
	else if (type == PIPE)
		return (2);
	return (3);
}

static void	check_lowest_precedence(t_token *current, int *res, int *low_prec,
int i)
{
	int	cur_prec;

	cur_prec = get_operator_precedence(current->type);
	if (cur_prec <= *low_prec)
	{
		*low_prec = cur_prec;
		*res = i;
	}
}

int	find_lowest_precedence_op(t_token **tokens, int i, int end)
{
	t_token	*current;
	int		paren_lvl;
	int		low_prec;
	int		res;

	res = -1;
	low_prec = 2147483647;
	paren_lvl = 0;
	while (i <= end)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		else if (current->type == PAREN_OPEN)
			paren_lvl++;
		else if (current->type == PAREN_CLOSE)
			paren_lvl--;
		if (!paren_lvl && (current->type == PIPE || current->type == AND
				|| current->type == OR))
			check_lowest_precedence(current, &res, &low_prec, i);
		i++;
	}
	return (res);
}

t_ast	*parse_operator(t_shell *data, int start, int end, int op_pos)
{
	t_node_type	type;
	t_token		*current;
	t_ast		*right;
	t_ast		*left;
	t_ast		*node;

	current = get_token_at_index(data->tokens, op_pos);
	if (!current)
		return (parse_command(data->tokens, start, end, data));
	else if (current->type == PIPE)
		return (parse_pipe(data, start, end, op_pos));
	type = convert_types(current->type);
	printf("In parse_operator for ");
	print_node_type(type);
	printf(", start = %d, end = %d, op_pos = %d\n", start, end, op_pos);
	left = parse_command_line(data->tokens, start, op_pos - 1, data);
	right = parse_command_line(data->tokens, op_pos + 1, end, data);
	node = create_operator_node(type, left, right, data);
	if (!node)
	{
		clean_ast(left);
		clean_ast(right);
		malloc_error(data->root, data, data->tokens);
	}
	return (node);
}
