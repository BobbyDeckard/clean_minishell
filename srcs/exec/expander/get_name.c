/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_name.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:03:03 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/30 17:03:46 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	get_name_len(const char *str)
{
	int	i;

	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

char	*get_name(t_ast *node, const char *str)
{
	char	*name;
	int		len;
	int		i;

	while (*str && *str != '$')
		str++;
	if (*(str + 1) == '?')
		return (NULL);
	len = get_name_len(str) + 1;
	name = (char *) malloc(len * sizeof(char));
	if (!name)
		malloc_error(node, node->shell, NULL);
	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		name[i - 1] = str[i];
		i++;
	}
	name[i - 1] = 0;
	return (name);
}
