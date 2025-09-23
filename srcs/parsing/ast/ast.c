/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 14:43:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/03 12:00:05 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static t_ast	*create_subshell(t_token **tokens, int start, int end,
t_shell *data)
{
	t_token	*current;
	int		closing;

	printf("In create_subshell()\n");
	current = get_token_at_index(tokens, start);
	if (start < end && current && current->type == PAREN_OPEN)
	{
		closing = find_matching_parentheses(tokens, start, end);
		if (closing > start && closing <= end)
			return (create_subshell_node(parse_command_line(tokens, ++start,
						--closing, data), data));
	}
	return (parse_command(tokens, start, end, data));
}

t_ast	*parse_command_line(t_token **tokens, int start, int end,
t_shell *data)
{
	t_node_type	type;
	int			op_pos;

	printf("In parse_command_line()\n");
	if (start > end)
		return (NULL);
	if (matching_parentheses(tokens, start, end))
		return (parse_command_line(tokens, ++start, --end, data));
	op_pos = find_lowest_precedence_op(tokens, start, end);
	if (op_pos == -1)
		return (create_subshell(tokens, start, end, data));
	return (parse_operator(data, start, end, op_pos));
}

t_ast	*create_ast(t_token **token_list, t_shell *data)
{
	t_ast	*root;
	int		tokens;

	tokens = count_tokens(token_list);
	root = parse_command_line(token_list, 0, --tokens, data);
	int	j = -1;
	printf("Printing root->cmd.args from create_ast() before setting root\n");
	while (root->cmd.args[++j])
		printf("root->cmd.args[%d]: '%s'\n", j, root->cmd.args[j]);
	printf("\n");
	set_root_node(root, root);
	j = -1;
	printf("Printing root->cmd.args from create_ast() after setting root\n");
	while (root->cmd.args[++j])
		printf("root->cmd.args[%d]: '%s'\n", j, root->cmd.args[j]);
	printf("\n");
	return (root);
}
