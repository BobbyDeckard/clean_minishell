/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:57:11 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/09 15:08:58 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*get_name(t_ast *node, const char *str);

static int	calc_exit_status_len(t_ast *node)
{
	char	*exit_status;
	int		len;

	exit_status = ft_itoa(node->shell->exit_status);
	if (!exit_status)
		malloc_error(node, node->shell, NULL);
	len = ft_strlen(exit_status) - 2;
	free(exit_status);
	return (len);
}

static int	calc_len(t_ast *node, char **envp, const char *arg)
{
	char	*name;
	int		len;
	int		i;
	int		j;

	name = get_name(node, arg);
	len = 0;
	if (!name)
		return (calc_exit_status_len(node));
	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (name[j] && name[j] == envp[i][j])
			j++;
		if (envp[i][j] == '=' && !name[j])
			break ;
	}
	if (envp[i])
		len = ft_strlen(envp[i]) - ft_strlen(name) - 1 - ft_strlen(name);
	free(name);
	return (len);
}

int	get_arg_len(t_ast *node, char **envp, const char *arg)
{
	int	in_double;
	int	in_single;
	int	len;
	int	i;

	i = -1;
	len = 0;
	in_double = 0;
	in_single = 0;
	while (arg[++i])
	{
		if (arg[i] == '"' && !in_single)
			in_double = !in_double;
		else if (arg[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (arg[i] == '$')
			len += calc_len(node, envp, arg + i);
		else
			len++;
	}
	return (len);
}

int	get_entry_index(const char *name, char **envp)
{
	int	i;
	int	j;

	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (name[j] && name[j] == envp[i][j])
			j++;
		if (envp[i][j] == '=' && !name[j])
			break ;
	}
	return (i);
}
