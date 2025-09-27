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

static void	filter_and_copy(t_ast *node, char **envp, char **new,
const char *remove)
{
	int	len;
	int	i;
	int	j;

	len = ft_strlen(remove);
	i = -1;
	j = 0;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], remove, len))
		{
			new[j] = copy_env_entry(node, new, i, j);
			j++;
		}
	}
	new[j] = NULL;
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
	filter_and_copy(node, node->data->envp, new, node->cmd.args[1]);
	free_char_array(node->data->envp);
	node->data->envp = new;
	return (set_exit_status(node, 0));
}
