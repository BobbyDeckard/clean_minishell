/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expander.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 19:30:30 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/04 20:14:24 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static char	*get_name(t_ast *node, char *line)
{
	char	*name;
	int		len;
	int		i;

	while (*line && *line != '$')
		line++;
	if (*(line + 1) == '?')
		return (NULL);
	len = get_name_len(line) + 1;
	name = (char *) malloc(len * sizeof(char));
	if (!name)
	{
		free(line);
		malloc_error(node, node->data, NULL);
	}
	i = 0;
	while (line[++i] && (ft_isalnum(line[i]) || line[i] == '_'))
		name[i - 1] = line[i];
	name[i - 1] = 0;
	return (name);
}

static int	dol_pos(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	return (i);
}

static void	expand_heredoc_error(t_ast *node, char *line)
{
free(line);
	malloc_error(node, node->data, NULL);
}

static char	*expand_exit_status(t_ast *node, char *line)
{
	char	*exit_status;
	char	*new;
	int		len;
	int		i;

	exit_status = ft_itoa(node->data->exit_status);
	if (!exit_status)
		expand_heredoc_error(node, line);
	i = dol_pos(line);
	len = ft_strlen(line) - ft_strlen(exit_status) - 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
	{
		free(exit_status);
		expand_heredoc_error(node, line);
	}
	ft_strlcpy(new, line, i + 1);
	ft_strlcat(new, exit_status, len);
	ft_strlcat(new, exit_status + i + 2, len);
	free(exit_status);
	free(line);
	return (new);
}

static char	*expand_var(t_ast *node, char *entry, char *line)
{
	char	*new;
	int		name_len;
	int		len;
	int		i;

	i = dol_pos(line);
	name_len = get_name_len(line + i);
	len = ft_strlen(line) + ft_strlen(entry) - name_len + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		expand_heredoc_error(node, line);
	ft_strlcpy(new, line, i + 1);
	ft_strlcat(new, entry, len);
	ft_strlcat(new, line + i + name_len, len);
	free(line);
	return (new);
}

static char	*remove_var_from_line(t_ast *node, char *line)
{
	char	*new;
	int		name_len;
	int		len;
	int		i;

	i = dol_pos(line);
	name_len = get_name_len(line + i);
	len = ft_strlen(line) - name_len + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		expand_heredoc_error(node, line);
	ft_strlcpy(new, line, i + 1);
	ft_strlcat(new, line + i + name_len, len);
	free(line);
	return (new);
}

static char	*expand_cat(t_ast *node, char **envp, char *line)
{
	char	*name;
	int		i;
	int		j;

	name = get_name(node, line);
	if (!name)
		return (expand_exit_status(node, line));
	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (name[j] && name[j] == envp[i][j])
			j++;
		if (envp[i][j] == '=' && !name[j])
		{
			free(name);
			return (expand_var(node, envp[i] + j + 1, line));
		}
	}
	free(name);
	return (remove_var_from_line(node, line));
}

char	*expand_line(t_ast *node, char **envp, char *line)
{
	while (contains_dol(line))
		line = expand_cat(node, envp, line);
	return (line);
}
