/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_expand_double.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 22:53:48 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/31 23:03:42 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*get_name(t_ast *node, const char *str);
void	rdr_expand_exit_status(t_ast *node, t_rdr *rdr, int index);
int		contains_dol(const char *str);
int		get_name_len(const char *str);
int		is_exit_status(const char *str);

static void	handle_var(t_ast *node, t_rdr *rdr, char *entry, int index)
{
	char	*new;
	int		name_len;
	int		len;
	int		i;

	i = 0;
	while (rdr->args[index][i] && rdr->args[index][i] != '$')
		i++;
	name_len = get_name_len(rdr->args[index] + i);
	len = ft_strlen(rdr->args[index]) + ft_strlen(entry) - name_len + 1;
	new = (char *) ft_calloc(len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(new, rdr->args[index], i + 1);
	ft_strlcat(new, entry, len);
	ft_strlcat(new, rdr->args[index] + i + name_len, len);
	free(rdr->args[index]);
	rdr->args[index] = new;
}

static void	expand_var(t_ast *node, t_rdr *rdr, char *name, int index)
{
	char	**envp;
	int		i;
	int		j;

	i = -1;
	envp = node->shell->envp;
	while (envp[++i])
	{
		j = 0;
		while (name[j] && name[j] == envp[i][j])
			j++;
		if (envp[i][j] == '=' && !name[j])
		{
			free(name);
			return (handle_var(node, rdr, envp[i] + j + 1, index));
		}
	}
	free(name);
	return (handle_var(node, rdr, "", index));
}

static void	expand_arg(t_ast *node, t_rdr *rdr, int i)
{
	char	*name;

	while (contains_dol(rdr->args[i]))
	{
		if (is_exit_status(rdr->args[i]))
			rdr_expand_exit_status(node, rdr, i);
		else
		{
			name = get_name(node, rdr->args[i]);
			expand_var(node, rdr, name, i);
		}
	}
}

int	rdr_expand_in_double(t_ast *node, t_rdr *rdr, int i)
{
	while (rdr->args[i] && ft_strncmp(rdr->args[i], "\"", 2))
	{
		if (contains_dol(rdr->args[i]))
			expand_arg(node, rdr, i);
		i++;
	}
	if (rdr->args[i])
		i++;
	return (i);
}
