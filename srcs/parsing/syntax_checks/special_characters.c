/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_characters.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 10:18:20 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/08 10:44:00 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_token	*cat_word(t_shell *data, t_token *previous, t_token *current)
{
	char	*new;
	int		len;

	if (!previous || previous->type != WORD)
	{
		current->type = WORD;
		return (current);
	}
	len = ft_strlen(previous->content) + 2;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(NULL, data, data->tokens);
	ft_strlcat(new, previous->content, len);
	ft_strlcat(new, current->content, len);
	free(previous->content);
	previous->content = new;
	current->next->previous = previous;
	previous->next = current->next;
	free(current->content);
	free(current);
	return (previous);
}
