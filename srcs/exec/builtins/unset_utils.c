/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:23:29 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/20 13:54:30 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	free_char_array(char **arr);

//	The usage of free_char_array() supposes a ft_calloc'ed char array
char	*copy_env_entry(t_ast *node, char **arr, int i)
{
	char	*new;
	int		len;

	len = ft_strlen(node->shell->envp[i]) + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
	{
		free_char_array(arr);
		malloc_error(node, node->shell, NULL);
	}
	ft_strlcpy(new, node->shell->envp[i], len);
	return (new);
}
