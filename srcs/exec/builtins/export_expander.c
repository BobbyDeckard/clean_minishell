/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:18:34 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/10 16:19:23 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*get_name(t_ast *node, const char *str);
int		get_arg_len(t_ast *node, char **envp, const char *arg);

char	*get_entry(char **envp, const char *name)
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

static int	exp_var(t_ast *node, char *new, const char *arg, int len_and_i[2])
{
	char	*name;
	char	*entry;

	name = get_name(node, arg + len_and_i[1]);
	len_and_i[1] += ft_strlen(name);
	entry = get_entry(node->shell->envp, name);
	ft_strlcat(new, entry, len_and_i[0]);
	free(name);
	return (ft_strlen(entry));
}

static void	make_new_arg(t_ast *node, const char *arg, char *new, int len)
{
	int	in_double;
	int	in_single;
	int	len_and_i[2];
	int	j;

	j = -1;
	in_double = 0;
	in_single = 0;
	*new = 0;
	len_and_i[0] = len;
	len_and_i[1] = -1;
	while (arg[++(len_and_i[1])])
	{
		if (arg[len_and_i[1]] == '"' && !in_single)
			in_double = !in_double;
		else if (arg[len_and_i[1]] == '\'' && !in_double)
			in_single = !in_single;
		else if (arg[len_and_i[1]] == '$' && !in_single)
			j += exp_var(node, new, arg, len_and_i);
		else
			new[++j] = arg[len_and_i[1]];
	}
}

static void	expand_arg(t_ast *node, const char *arg, int index)
{
	char	*new;
	int		len;

	len = get_arg_len(node, node->shell->envp, arg) + 1;
	new = (char *) ft_calloc(len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
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
