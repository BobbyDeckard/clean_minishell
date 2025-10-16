/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:28:37 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/14 16:51:12 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_token	*skip_redir(t_token **list, int *start, int end);
void	clean_ast(t_ast *ast);;
int		is_redir_token(t_t_type type);

static int	is_arg_token(t_t_type type)
{
	if (type == WORD)
		return (1);
	else if (type == DOUBLE_QUOTE)
		return (1);
	else if (type == SINGLE_QUOTE)
		return (1);
	return (0);
}

static t_token	*parse_arg(t_ast *node, t_token *current, int i)
{
	int	len;

	if (current->type == WORD)
	{
		len = ft_strlen(current->content) + 1;
		node->cmd.args[i] = (char *) malloc(len * sizeof(char));
		if (!node->cmd.args[i])
			return (NULL);
		ft_strlcpy(node->cmd.args[i], current->content, len);
	}
	else if (current->type == SINGLE_QUOTE || current->type == DOUBLE_QUOTE)
	{
		node->cmd.args[i] = (char *) malloc(2 * sizeof(char));
		if (!node->cmd.args[i])
			return (NULL);
		if (current->type == SINGLE_QUOTE)
			ft_strlcpy(node->cmd.args[i], "'", 2);
		else
			ft_strlcpy(node->cmd.args[i], "\"", 2);
	}
	return (current->next);
}

static void	parse_args_error(t_shell *shell, t_ast *node)
{
	if (!node->cmd.args[0])
		free(node->cmd.args);
	clean_ast(node);
	malloc_error(shell->root, shell, shell->tokens);
}

void	parse_args(t_shell *shell, t_ast *node, int start, int end)
{
	t_token	*current;
	int		i;

	i = -1;
	current = get_token_at_index(shell->tokens, start);
	while (current && start <= end)
	{
		if (is_redir_token(current->type))
			current = skip_redir(shell->tokens, &start, end);
		else if (is_arg_token(current->type))
		{
			current = parse_arg(node, current, ++i);
			if (!current)
				parse_args_error(shell, node);
			start++;
		}
		else
		{
			current = current->next;
			start++;
		}
}
}
