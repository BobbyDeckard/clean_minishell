/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 08:35:12 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/05 11:01:29 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_word(t_token_type type)
{
	if (type == WORD || type == WORD_CAT)
		return (1);
	return (0);
}

int	count_tokens(t_token **token_list)
{
	t_token	*current;
	int		i;

	i = 0;
	current = *token_list;
	while (current)
	{
		current = current->next;
		i++;
	}
	return (i);
}

int	ft_wordlen(char *content)
{
	char	c;
	int		i;

	i = 0;
	c = content[i];
	while (c && !is_space(c) && !is_quote(c) && !is_parenthesis(c)
		&& !is_redirection(c) && !is_operator(c)
		&& !is_special_character(c) && is_command_char(c))
		c = content[++i];
	return (i);
}

void	free_tokens(t_token **token_list)
{
	t_token	*current;
	t_token	*next;

	if (!token_list || !*token_list)
		return ;
	current = *token_list;
	while (current)
	{
		next = current->next;
		if (current->content)
			free(current->content);
		free(current);
		current = next;
	}
	free(token_list);
}
