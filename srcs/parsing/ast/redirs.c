/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:45:12 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/20 16:40:05 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_n_type	convert_types(t_t_type type);
char		*extract_content(t_token *token);
void		parse_redir_error(t_shell *shell, t_ast *node, int i);
int			create_redir_node(t_shell *shell, t_ast *node, t_n_type type, int count);

int	is_redir_arg(t_token *token)
{
	if (token->type == WORD)
		return (1);
	else if (token->type == DOUBLE_QUOTE)
		return (1);
	else if (token->type == SINGLE_QUOTE)
		return (1);
	return (0);
}

void	print_token_type(t_t_type type);
int	is_redir_token(t_t_type type)
{
	if (type == REDIR_IN)
		return (1);
	else if (type == REDIR_OUT)
		return (1);
	else if (type == REDIR_APPEND)
		return (1);
	else if (type == HEREDOC)
		return (1);
	return (0);
}

int	count_redir_args(t_token *current)
{
	int	count;

	if (!current)
		return (0);
	while (current && current->type == WHITESPACE)
		current = current->next;
	count = 0;
	while (current && is_redir_arg(current))
	{
		current = current->next;
		count++;
	}
	return (count);
}

static t_token	*parse_redir(t_shell *shell, t_ast *node, t_token *current, int *start)
{
	t_t_type	type;
	int			count;
	int			i;
	int			j;

	if (!current)
		return (NULL);
	type = current->type;
	current = current->next;
	(*start)++;
	while (current->type == WHITESPACE)
	{
		current = current->next;
		(*start)++;
	}
	count = count_redir_args(current) + 1;
	i = create_redir_node(shell, node, convert_types(type), count);
	j = -1;
	while (current && is_redir_arg(current))
	{
		node->children[i]->rdr.args[++j] = extract_content(current);
		if (!node->children[i]->rdr.args[j])
			parse_redir_error(shell, node, i);
		current = current->next;
		(*start)++;
	}
	return (current);
}

void	parse_redirs(t_shell *shell, t_ast *node, int start, int end)
{
	t_token	*current;

	if (!node->children)
		return ;
	current = get_token_at_index(shell->tokens, start);
	while (current && start <= end)
	{
		if (is_redir_token(current->type))
			current = parse_redir(shell, node, current, &start);
		else
		{
			current = current->next;
			start++;
		}
	}
}
