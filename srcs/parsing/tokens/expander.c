/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 13:33:03 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/11 22:07:51 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static char	*expand_var(t_token **tl, t_shell *data, char *entry, int i)
{
	char	*content;
	int		len;

	len = ft_strlen(entry + ++i) + 1;
	content = (char *) malloc(len * sizeof(char));
	if (!content)
		malloc_error(NULL, data, tl);
	ft_strlcat(content, entry + i, len);
	return (content);
}

static char	*expand(t_token **tl, t_shell *data, char *name)
{
	char	**envp;
	int		i;
	int		j;

	envp = data->envp;
	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (name[j] && name[j] == envp[i][j])
			j++;
		if (envp[i][j] == '=')
		{
			free(name);
			return (expand_var(tl, data, envp[i], j));
		}
	}
	free(name);
	return (NULL);
	// ATTENTION, si une variable à étendre n'existe pas, bash se comporte comme si ce mot n'avait pas été donné
	// exemple :
	// bash > echo hello $USSER how are you		[input]
	// bash > hello how are you					[output]
	// bash > echo $USSER						[input]
	// bash > 									[output]
	// bash > echo								[input]
	// bash >									[output]
	// --> pour les deux derniers inputs, démonstration qu'echo se comporte comme si aucun argument lui avait été donné
}

static int	word_contains_var(char *content)
{
	int	in_single_quotes;
	int	i;

	if (!content)
		return (0);
	in_single_quotes = 0;
	i = -1;
	while (content[++i])
	{
		if (content[i] == '\'' && !in_single_quotes)
			in_single_quotes = 1;
		else if (content[i] == '\'' && in_single_quotes)
			in_single_quotes = 0;
		else if (content[i] == '$' && !in_single_quotes)
		{
			if (content[i + 1] == '?' || ft_isalpha(content[i + 1]))
				return (1);
		}
	}
	return (0);
}

static int	needs_expansion(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == ENV_VAR || token->type == EXIT_STATUS)
		return (1);
	if (token->type == WORD && word_contains_var(token->content))	// maybe need to remove ? seems like it could wrongly detect an env var where there is none
		return (1);
	return (0);
}

void	expander(t_token **token_list, t_shell *data)
{
	t_token	*current;

	if (!token_list || !*token_list)
		return ;
	current = *token_list;
	while (current)
	{
		if (needs_expansion(current))
			current->content = expand(token_list, data, current->content);
		current = current->next;
	}
}
