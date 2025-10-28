/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:28:37 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 20:02:28 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_token	*skip_redir(t_token **list, int *start, int end);
void	clean_ast(t_ast *ast);
int		is_redir_token(t_t_type type);

static int	is_arg_token(t_t_type type)
{
	if (type == WORD)
		return (1);
	else if (type == DOUBLE_QUOTE)
		return (1);
	else if (type == SINGLE_QUOTE)
		return (1);
	else if (type == WHITESPACE)
		return (1);
	return (0);
}

static int	parse_arg(t_ast *node, t_token *current, int i)
{
	int	len;

	if (current->content)
	{
		len = ft_strlen(current->content) + 1;
		node->cmd.args[i] = (char *) malloc(len * sizeof(char));
		if (!node->cmd.args[i])
			return (1);
		ft_strlcpy(node->cmd.args[i], current->content, len);
	}
	return (0);
}

static void	parse_args_error(t_shell *shell, t_ast *node)
{
	if (!node->cmd.args[0])
		free(node->cmd.args);
	malloc_error(shell->root, shell, shell->tokens);
}

t_token	*skip_spaces(t_token **list, int *start, int end)
{
	t_token	*current;

	current = get_token_at_index(list, *start);
	while (current && current->type == WHITESPACE && *start <= end)
	{
		current = current->next;
		(*start)++;
	}
	return (current);
}

void	parse_args(t_shell *shell, t_ast *node, int start, int end)
{
	t_token	*current;
	int		i;

	i = -1;
	current = skip_spaces(shell->tokens, &start, end);
	while (current && start <= end)
	{
		if (is_redir_token(current->type))
			current = skip_redir(shell->tokens, &start, end);
		else if (is_arg_token(current->type))
		{
			if (parse_arg(node, current, ++i))
				parse_args_error(shell, node);
			current = current->next;
			start++;
		}
		else
		{
			current = current->next;
			start++;
		}
	}
}
