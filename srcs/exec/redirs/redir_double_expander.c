/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_double_expander.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 09:45:19 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 09:48:45 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*get_name(t_ast *node, const char *str);
int		contains_dol(const char *str);
int		get_name_len(const char *str);
int		rdr_handle_exit_status(t_ast *node, t_rdr *rdr, int index);
int		rdr_remove_arg(t_rdr *rdr, int i);
int		rdr_remove_var(t_ast *node, t_rdr *rdr, int index);

static int	rdr_double_handle_var(t_ast *node, t_rdr *rdr, char *entry, int index)
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
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(new, rdr->args[index], i + 1);
	ft_strlcat(new, entry, len);
	ft_strlcat(new, rdr->args[index] + i + name_len, len);
	free(rdr->args[index]);
	rdr->args[index] = new;
	return (0);
}

static int	rdr_double_expand_cat(t_ast *node, t_rdr *rdr, char **envp, int index)
{
	char	*name;
	int		i;
	int		j;

	name = get_name(node, rdr->args[index]);
	if (!name)
		return (rdr_handle_exit_status(node, rdr, index));
	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (name[j] && name[j] == envp[i][j])
			j++;
		if (envp[i][j] == '=' && !name[j])
		{
			free(name);
			return (rdr_double_handle_var(node, rdr, envp[i] + j + 1, index));
		}
	}
	if (ft_strlen(name) + 1 == ft_strlen(rdr->args[index]))
	{
		free(name);
		return (rdr_remove_arg(rdr, index));
	}
	free(name);
	return (rdr_remove_var(node, rdr, index));
}

void	rdr_double_expand(t_ast *node, t_rdr *rdr, char **envp, int index)
{
	while (contains_dol(rdr->args[index]))
	{
		if (rdr_double_expand_cat(node, rdr, envp, index))
			return ;
	}
}
