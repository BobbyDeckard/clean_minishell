/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_characters.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 10:18:20 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/30 18:38:29 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static t_token	*cat_prev(t_shell *data, t_token *cur, t_token *prev, int len)
{
	char	*new;

	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(NULL, data, data->tokens);
	ft_strlcpy(new, prev->content, len);
	ft_strlcat(new, cur->content, len);
	free(prev->content);
	prev->content = new;
	prev->next = cur->next;
	cur->next->previous = prev;
	free(cur->content);
	free(cur);
	return (prev);
}

static t_token	*cat_next(t_shell *data, t_token *current, t_token *next,
int len)
{
	t_token	*prev;
	char	*new;

	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(NULL, data, data->tokens);
	ft_strlcpy(new, current->content, len);
	ft_strlcat(new, next->content, len);
	free(next->content);
	next->content = new;
	next->previous = current->previous;
	current->previous->next = current->next;
	prev = current->previous;
	free(current->content);
	free(current);
	return (prev);
}

static t_token	*cat_both(t_shell *data, t_token *current, int len)
{
	t_token	*next;
	t_token	*prev;
	char	*new;

	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(NULL, data, data->tokens);
	prev = current->previous;
	next = current->next;
	ft_strlcpy(new, prev->content, len);
	ft_strlcat(new, current->content, len);
	ft_strlcat(new, next->content, len);
	free(prev->content);
	prev->content = new;
	prev->next = next->next;
	next->next->previous = prev;
	free(current->content);
	free(current);
	free(next->content);
	free(next);
	return (prev);
}

t_token	*cat_word(t_shell *data, t_token *current, t_token *prev, t_token *next)
{
	int		len;

	len = ft_strlen(current->content) + 1;
	if ((!prev || prev->type != WORD) && (!next || next->type != WORD))
	{
		current->type = WORD;
		return (current);
	}
	else if (prev && prev->type == WORD && next && next->type == WORD)
	{
		len += ft_strlen(prev->content) + ft_strlen(next->content);
		return (cat_both(data, current, len));
	}
	else if (prev && prev->type == WORD)
	{
		len += ft_strlen(prev->content);
		return (cat_prev(data, current, prev, len));
	}
	else if (next && next->type == WORD)
	{
		len += ft_strlen(next->content);
		return (cat_next(data, current, next, len));
	}
	return (current);
}
