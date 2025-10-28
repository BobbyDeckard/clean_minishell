/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_expander_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 20:29:00 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/27 19:49:19 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*get_name(t_ast *node, const char *str);
char	*filter_spaces(t_ast *node, char *entry, int *flag);
int		contains_contig_spaces(const char *str);
int		get_name_len(const char *str);
int		rdr_remove_arg(t_rdr *rdr, int i);
int		rdr_remove_var(t_ast *node, t_rdr *rdr, int index);

static int	rdr_handle_var(t_ast *node, t_rdr *rdr, char *entry, int index)
{
	char	*new;
	int		filtered_spaces;
	int		name_len;
	int		len;
	int		i;

	i = 0;
	filtered_spaces = 0;
	while (rdr->args[index][i] && rdr->args[index][i] != '$')
		i++;
	name_len = get_name_len(rdr->args[index] + i);
	if (contains_contig_spaces(entry))
		entry = filter_spaces(node, entry, &filtered_spaces);
	len = ft_strlen(rdr->args[index]) + ft_strlen(entry) - name_len + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(new, rdr->args[index], i + 1);
	ft_strlcat(new, entry, len);
	ft_strlcat(new, rdr->args[index] + i + name_len, len);
	if (filtered_spaces)
		free(entry);
	free(rdr->args[index]);
	rdr->args[index] = new;
	return (0);
}

int	rdr_handle_exit_status(t_ast *node, t_rdr *rdr, int index)
{
	char	*exit_status;
	char	*new;
	int		len;
	int		i;

	exit_status = ft_itoa(node->shell->exit_status);
	if (!exit_status)
		malloc_error(node, node->shell, NULL);
	i = 0;
	while (rdr->args[index][i] && rdr->args[index][i] != '$')
		i++;
	len = ft_strlen(rdr->args[index]) + ft_strlen(exit_status) - 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
	{
		free(exit_status);
		malloc_error(node, node->shell, NULL);
	}
	ft_strlcpy(new, rdr->args[index], i + 1);
	ft_strlcat(new, exit_status, len);
	ft_strlcat(new, rdr->args[index] + i + 2, len);
	free(exit_status);
	free(rdr->args[index]);
	rdr->args[index] = new;
	return (0);
}

static int	rdr_expand_cat_end(t_ast *node, t_rdr *rdr, char *name, int index)
{
	if (ft_strlen(name) + 1 == ft_strlen(rdr->args[index]))
	{
		printf("Found env var to be alone in arg[%d]\n", index);
		if (index == 0 && !rdr->args[1])
		{
			ft_putstr_fd("minishell: $", 2);
			ft_putstr_fd(name, 2);
			ft_putstr_fd(": ambiguous redirect\n", 2);
		}
		free(name);
		return (rdr_remove_arg(rdr, index));
	}
	free(name);
	return (rdr_remove_var(node, rdr, index));
}

int	rdr_expand_cat(t_ast *node, t_rdr *rdr, char **envp, int index)
{
	char	*name;
	int		i;
	int		j;

	printf("In rdr_expand_cat with arg[%d]: '%s'\n", index, rdr->args[index]);
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
			return (rdr_handle_var(node, rdr, envp[i] + j + 1, index));
		}
	}
	return (rdr_expand_cat_end(node, rdr, name, index));
}
