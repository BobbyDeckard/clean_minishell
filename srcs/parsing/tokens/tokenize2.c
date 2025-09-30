/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 09:59:57 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/30 18:45:45 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static t_token	*set_as_unknown(t_shell *data, t_token **tl, char **command,
t_token *token)
{
	token->type = UNKNOWN;
	token->content = (char *) malloc(2 * sizeof(char));
	if (!token->content)
	{
		free(token);
		malloc_error(NULL, data, tl);
	}
	ft_strlcpy(token->content, *command, 2);
	(*command)++;
	return (token);
}

static t_token	*set_as_env_var(t_shell *data, t_token **tl, char **command,
t_token *token)
{
	char	*str;
	int		i;

	token->type = ENV_VAR;
	(*command)++;
	str = *command;
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	token->content = ft_substr(str, 0, i);
	if (!token->content)
		tokenization_error(data, tl, token);
	*command += i;
	return (token);
}

t_token	*tokenize_special_character(t_shell *data, t_token **tl, char **command,
t_token *token)
{
	if (**command != '$')
		return (set_as_unknown(data, tl, command, token));
	else if ((*command)[1] == '?')
	{
		token->type = EXIT_STATUS;
		token->content = ft_strdup("?");
		if (!token->content)
			tokenization_error(data, tl, token);
		*command += 2;
	}
	else if (ft_isalpha((*command)[1]) || (*command)[1] == '_')
		return (set_as_env_var(data, tl, command, token));
	else
	{
		token->type = WORD;
		token->content = ft_strdup("$");
		if (!token->content)
			tokenization_error(data, tl, token);
		(*command)++;
	}
	return (token);
}

static void	define_bonus_operator(char **command, t_token *token, int *len)
{
	if (**command == '&' && (*command)[1] == '&')
		token->type = AND;
	else if (**command == '|' && (*command)[1] == '|')
		token->type = OR;
	else
		token->type = UNKNOWN;
	*len = 2;
}

t_token	*tokenize_operator(t_shell *data, t_token **tl, char **command,
t_token *token)
{
	int	len;
	int	i;

	len = 1;
	if (**command == '|')
		token->type = PIPE;
	else if (**command == ';')
		token->type = SEMICOLON;
	if (is_logical_operator((*command)[1]))
		define_bonus_operator(command, token, &len);
	token->content = (char *) malloc((len + 1) * sizeof(char));
	if (!token->content)
		tokenization_error(data, tl, token);
	i = -1;
	while (is_logical_operator((*command)[++i]) && i < len)
		token->content[i] = (*command)[i];
	token->content[i] = 0;
	*command += len;
	return (token);
}
