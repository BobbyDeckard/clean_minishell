/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:52:15 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/11 22:06:28 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_token	*tokenize_env_var(t_shell *data, t_token **tokens, char **command, t_token *token)
{
	int	len;

	(*command)++;
	token->type = ENV_VAR;
	if (*command == '?')
		tokenize_exit_status(data, tokens, command, token);
	len = ft_wordlen(*command);
	token->content = (char *) malloc(++len * sizeof(char));
	if (!token->content)
	{
		free(token);
		malloc_error(NULL, data, tokens);
	}
	ft_strlcpy(token->content, *command, len);
	*command += --len;
	return (token);
}

t_token	*tokenize_space(char **command, t_token *token)
{
	int	len;

	len = 1;
	token->content = NULL;
	token->type = SPACE;
	*command += len;
	return (token);
}
