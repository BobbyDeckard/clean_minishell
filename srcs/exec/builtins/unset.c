/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:28:23 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/11 15:37:18 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*copy_env_entry(t_ast *node, char **arr, int i, int j)
{
	char	*new;
	int		len;

	len = ft_strlen(node->data->envp[i]) + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
	{
		free_char_array_size(arr, j);
		malloc_error(node, node->data, NULL);
	}
	ft_strlcpy(new, node->data->envp[i], len);
	return (new);
}

int	unset(t_ast *node)
{
	char	**new;
	int		len;
	int		i;
	int		j;

	i = char_arr_len(node->data->envp);
	if (i == -1)
		return (set_exit_status(node, 1));
	new = (char **) malloc(i * sizeof(char *));
	if (!new)
		malloc_error(node, node->data, NULL);
	i = -1;
	j = 0;
	while (node->data->envp[++i])
	{
		len = ft_strlen(node->cmd.args[1]);
		if (ft_strncmp(node->data->envp[i], node->cmd.args[1], len))
		{
			new[j] = copy_env_entry(node, new, i, j);
			j++;
		}
	}
	new[j] = NULL;
	free_char_array(node->data->envp);
	node->data->envp = new;
	return (set_exit_status(node, 0));
}
