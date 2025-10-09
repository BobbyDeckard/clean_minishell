/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 13:49:59 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/09 13:55:26 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	free_content_or_cmd(t_ast *node, char *content, t_cmd cmd)
{
	if (!node && content)
		free(content);
	else if (!node && cmd.args)
		free_cmd(cmd);
}

int	count_redirs(t_token **tokens, int start, int end)
{
	t_token	*current;
	int		count;
	int		i;

	count = 0;
	i = start - 1;
	while (++i <= end)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		if (is_redir_token(current) && i + 1 <= end)
		{
			current = get_token_at_index(tokens, i + 1);
			if (current && is_word(current->type))
				count++;
		}
	}
	return (count);
}

t_ast	**free_redirs(t_ast **redirs, int i)
{
	while (--i >= 0)
		free(redirs[i]);
	free(redirs);
	return (NULL);
}

int	count_redir_args(t_token *current)
{
	int	i;

	i = 0;
	current = current->next;
	if (current && current->type == WORD)
	{
		current = current->next;
		i++;
	}
	while (current)
	{
		if (current->type == WORD_CAT)
			i++;
		else if (current->type != SINGLE_QUOTE && current->type != DOUBLE_QUOTE)
			break ;
		current = current->next;
	}
	return (i);
}

void	free_cmd(t_cmd cmd)
{
	int	i;

	i = -1;
	while (cmd.args[++i])
		free(cmd.args[i]);
	free(cmd.args);
	free(cmd.exp);
	free(cmd.cat);
}
