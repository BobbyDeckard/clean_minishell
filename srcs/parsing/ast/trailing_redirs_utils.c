/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trailing_redirs_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:02:59 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/05 11:05:09 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	trailing_redir_error(t_ast *node, t_shell *data, t_redir *redirs,
int count)
{
	int	i;

	i = -1;
	while (++i < count)
		free(redirs[i].file);
	free(redirs);
	malloc_error(node, data, data->tokens);
}

int	count_trailing_redirs(t_token *current)
{
	int	count;

	count = 0;
	while (current)
	{
		if (!is_redir_token(current))
			break ;
		current = current->next;
		if (!is_word(current->type))
			break ;
		count++;
		current = current->next;
	}
	return (count);
}

t_node_type	convert_redir_type(t_token_type token_type)
{
	if (token_type == REDIR_IN)
		return (NODE_REDIR_IN);
	else if (token_type == REDIR_OUT)
		return (NODE_REDIR_OUT);
	else if (token_type == REDIR_APPEND)
		return (NODE_REDIR_APPEND);
	else if (token_type == HEREDOC)
		return (NODE_HEREDOC);
	else if (token_type == HEREDOC_EXP)
		return (NODE_HEREDOC_EXP);
	return (-1);
}

int	copy_existing_children(t_ast **new, t_ast *node, int count)
{
	int	i;

	i = -1;
	while (++i < count)
		new[i] = node->children[i];
	return (--i);
}
