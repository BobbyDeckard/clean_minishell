/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:18:34 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/09 11:59:49 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	calc_exit_status_len(t_ast *node)
{
	char	*exit_status;
	int		len;

	exit_status = ft_itoa(node->data->exit_status);
	if (!exit_status)
		malloc_error(node, node->data, NULL);
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

static int	get_arg_len(t_ast *node, char **envp, const char *arg)
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

static char	*get_entry(char **envp, const char *name)
{
	int		i;
	int		j;

	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (name[j] && name[j] == envp[i][j])
			j++;
		if (envp[i][j] == '=' && !name[j])
			return (envp[i] + j + 1);
	}
	return (NULL);
}

static int	exp_var(t_ast *node, char *new, const char *arg, int *i, int len)
{
	char	*name;
	char	*entry;

	name = get_name(node, arg + *i);
	*i += ft_strlen(name);
	entry = get_entry(node->data->envp, name);
	ft_strlcat(new, entry, len);
	free(name);
	return (ft_strlen(entry));
}

static void make_new_arg(t_ast *node, const char *arg, char *new, int len)
{
	int	in_double;
	int	in_single;
	int	i;
	int	j;

	i = -1;
	j = -1;
	in_double = 0;
	in_single = 0;
	*new = 0;
	while (arg[++i])
	{
		if (arg[i] == '"' && !in_single)
			in_double = !in_double;
		else if (arg[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (arg[i] == '$' && !in_single)
			j += exp_var(node, new, arg, &i, len);
		else
			new[++j] = arg[i];
	}
}

static void	expand_arg(t_ast *node, const char *arg, int index)
{
	char	*new;
	int		len;

	len = get_arg_len(node, node->data->envp, arg) + 1;
	new = (char *) ft_calloc(len, sizeof(char));
	if (!new)
		malloc_error(node, node->data, NULL);
	make_new_arg(node, arg, new, len);
	free(node->cmd.args[index]);
	node->cmd.args[index] = new;
}

void	export_expander(t_ast *node)
{
	int	i;

	i = 0;
	while (node->cmd.args[++i])
		expand_arg(node, node->cmd.args[i], i);
}
