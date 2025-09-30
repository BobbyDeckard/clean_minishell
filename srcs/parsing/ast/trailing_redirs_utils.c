/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trailing_redirs_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:02:59 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/30 16:04:21 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	count_trailing_redirs(t_token *current, int start)
{
	int	count;

	count = 0;
	while (current)
	{
		if (!is_redir_token(current))
			break ;
		current = current->next;
		if (current->type != WORD)
			break ;
		count++;
		current = current->next;
	}
	return (count);
}

void	trailing_redir_error(t_ast *node, t_shell *data, t_token **tokens, t_redir *redirs)
{
	//	Solve the freeing of redirs (have a dedicated function)
	free(redirs);
	malloc_error(node , data, tokens);
}

t_node_type	convert_redir_type(t_token_type token_type)
{
	t_node_type	type;

	if (token_type == REDIR_IN)
		return (NODE_REDIR_IN);
	else if (token_type == REDIR_OUT)
		return (NODE_REDIR_OUT);
	else if (token_type == REDIR_APPEND)
		return (NODE_REDIR_APPEND);
	else if (token_type == HEREDOC)
		return (NODE_HEREDOC);
	return (-1);
}

