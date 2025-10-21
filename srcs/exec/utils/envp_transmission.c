/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_transmission.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 13:44:58 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/08 14:16:38 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*copy_env_entry(t_ast *node, char **arr, int i);
int		char_arr_len(char **arr);

static int	contains_equal(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (ft_strncmp(str, "OLDPWD", 7) && str[i] == '=')
			return (1);
	}
	return (0);
}

static int	count_uninit_vars(char **envp)
{
	int	count;
	int	i;

	i = -1;
	count = 0;
	while (envp[++i])
	{
		if (!contains_equal(envp[i]))
			count++;
	}
	return (count);
}

static char	**remove_uninit_vars(t_ast *node, char **envp)
{
	char	**new;
	int		count;
	int		len;
	int		i;
	int		j;

	count = count_uninit_vars(envp);
	if (!count)
		return (NULL);
	len = char_arr_len(envp) - count + 1;
	new = (char **) ft_calloc(len, sizeof(char *));
	if (!new)
		malloc_error(node, node->shell, NULL);
	i = -1;
	j = -1;
	while (envp[++i])
	{
		if (contains_equal(envp[i]))
			new[++j] = copy_env_entry(node, envp, i);
	}
	new[++j] = NULL;
	return (new);
}

void	exec_minishell(t_ast *node, t_cmd cmd)
{
	char	**envp;
	int		i;

	envp = remove_uninit_vars(node, node->shell->envp);
	if (envp)
	{
		if (execve(cmd.path, cmd.args, envp) == -1)
			perror("execve");
		i = -1;
		while (envp[++i])
			free(envp[i]);
		free(envp);
	}
	else
	{
		if (execve(cmd.path, cmd.args, node->shell->envp) == -1)
			perror("execve");
	}
}
