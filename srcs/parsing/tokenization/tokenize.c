/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 15:16:16 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 18:18:22 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	wordlen(const char *str);

t_token	*tokenize_double_quote(t_shell *shell, char **command, t_token **list,
t_token *token)
{
	token->type = DOUBLE_QUOTE;
	token->content = (char *) malloc(2 * sizeof(char));
	if (!token->content)
	{
		free(token);
		malloc_error(NULL, shell, list);
	}
	ft_strlcpy(token->content, *command, 2);
	*command += 1;
	return (token);
}

t_token	*tokenize_single_quote(t_shell *shell, char **command, t_token **list,
t_token *token)
{
	token->type = SINGLE_QUOTE;
	token->content = (char *) malloc(2 * sizeof(char));
	if (!token->content)
	{
		free(token);
		malloc_error(NULL, shell, list);
	}
	ft_strlcpy(token->content, *command, 2);
	*command += 1;
	return (token);
}

t_token	*tokenize_whitespace(t_shell *shell, char **command, t_token **list,
t_token *token)
{
	token->type = WHITESPACE;
	token->content = (char *) malloc(2 * sizeof(char));
	if (!token->content)
	{
		free(token);
		malloc_error(NULL, shell, list);
	}
	ft_strlcpy(token->content, *command, 2);
	*command += 1;
	return (token);
}

t_token	*tokenize_word(t_shell *shell, char **command, t_token **list,
t_token *token)
{
	int	len;

	token->type = WORD;
	len = wordlen(*command) + 1;
	token->content = (char *) malloc(len * sizeof(char));
	if (!token->content)
	{
		free(token);
		malloc_error(NULL, shell, list);
	}
	ft_strlcpy(token->content, *command, len);
	*command += --len;
	return (token);
}
