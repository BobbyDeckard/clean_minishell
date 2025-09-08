/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safer_libft.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:21:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/08 12:14:50 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

char	*sf_strdup(const char *s, t_token **tokens, char **args, t_shell *data)
{
	size_t	len;
	size_t	i;
	char	*ptr;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	ptr = (char *) malloc((len + 1) * sizeof(char));
	if (!ptr)
	{
		free_str_array(args);
		malloc_error(NULL, data, tokens);
	}
	i = -1;
	while (++i < len)
		ptr[i] = s[i];
	ptr[i] = 0;
	return (ptr);
}
