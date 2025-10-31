/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 22:11:41 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/31 23:51:04 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	**init_args(t_ast *node, int count);
char	*filter_spaces(t_ast *node, char *entry);
char	*get_name(t_ast *node, const char *str);
void	rdr_expand_exit_status(t_ast *node, t_rdr *rdr, int index);
int		rdr_expand_in_double(t_ast *node, t_rdr *rdr, int i);
void	rdr_make_args(t_ast *node, t_rdr *rdr, char **args, int count);
int		contains_contig_spaces(const char *str);
int		contains_dol(const char *str);
int		get_name_len(const char *str);
int		is_exit_status(const char *str);
int		rdr_count_args(t_rdr *rdr);
int		rdr_skip_single(t_rdr *rdr, int i);

static void	handle_var(t_ast *node, t_rdr *rdr, char *entry, int index)
{
	char	*new;
	int		name_len;
	int		spaces;
	int		len;
	int		i;

	i = 0;
	spaces = 0;
	while (rdr->args[index][i] && rdr->args[index][i] != '$')
		i++;
	name_len = get_name_len(rdr->args[index] + i);
	if (contains_contig_spaces(entry) && ++spaces)
		entry = filter_spaces(node, entry);
	len = ft_strlen(rdr->args[index]) + ft_strlen(entry) - name_len + 1;
	new = (char *) ft_calloc(len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	//	Need to protect this malloc more efficiently
	ft_strlcpy(new, rdr->args[index], i + 1);
	ft_strlcat(new, entry, len);
	ft_strlcat(new, rdr->args[index] + i + name_len, len);
	if (spaces)
		free(entry);
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

static void	expand(t_ast *node, t_rdr *rdr)
{
	int	i;

	i = 0;
	while (rdr->args[i])
	{
		if (!ft_strncmp(rdr->args[i], "\"", 2))
			i = rdr_expand_in_double(node, rdr, ++i);
		else if (!ft_strncmp(rdr->args[i], "'", 2))
			i = rdr_skip_single(rdr, ++i);
		else if (contains_dol(rdr->args[i]))
			expand_arg(node, rdr, i++);
		else
			i++;
	}
}

void	rdr_expander(t_ast *node, t_rdr *rdr)
{
	char	**args;
	int		count;

	expand(node, rdr);
	count = rdr_count_args(rdr);
	args = init_args(node, count + 1);
	rdr_make_args(node, rdr, args, count);
}
