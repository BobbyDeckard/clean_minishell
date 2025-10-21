/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expander_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 15:26:24 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/09 15:26:48 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int		get_name_len(const char *str);

char	*get_name_bis(t_ast *node, char *line)
{
	char	*name;
	int		len;
	int		i;

	while (*line && *line != '$')
		line++;
	if (*(line + 1) == '?')
		return (NULL);
	len = get_name_len(line) + 1;
	name = (char *) malloc(len * sizeof(char));
	if (!name)
	{
		free(line);
		malloc_error(node, node->shell, NULL);
	}
	i = 0;
	while (line[++i] && (ft_isalnum(line[i]) || line[i] == '_'))
		name[i - 1] = line[i];
	name[i - 1] = 0;
	return (name);
}

int	dol_pos(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	return (i);
}

void	expand_heredoc_error(t_ast *node, char *line)
{
	free(line);
	malloc_error(node, node->shell, NULL);
}
