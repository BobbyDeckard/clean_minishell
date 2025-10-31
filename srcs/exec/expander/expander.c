/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 12:12:02 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/31 15:38:29 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	**init_args(t_ast *node, int count);
char	*filter_spaces(t_ast *node, char *entry);
char	*get_name(t_ast *node, const char *str);
void	expand_exit_status(t_ast *node, t_cmd *cmd, int index);
void	free_char_array(char **arr);
void	make_args(t_ast *node, t_cmd *cmd, char **args, int count);
int		contains_contig_spaces(const char *str);
int		contains_dol(const char *str);
int		count_args(t_cmd *cmd);
int		expand_in_double(t_ast *node, t_cmd *cmd, int i);
int		get_name_len(const char *str);
int		is_exit_status(const char *str);
int		is_whitespace(const char *str);
int		skip_single(t_cmd *cmd, int i);
int		skip_whitespaces(t_cmd *cmd, int i);

static void	handle_var(t_ast *node, t_cmd *cmd, char *entry, int index)
{
	char	*new;
	int		name_len;
	int		spaces;
	int		len;
	int		i;

	i = 0;
	spaces = 0;
	while (cmd->args[index][i] && cmd->args[index][i] != '$')
		i++;
	name_len = get_name_len(cmd->args[index] + i);
	if (contains_contig_spaces(entry) && ++spaces)
		entry = filter_spaces(node, entry);
	len = ft_strlen(cmd->args[index]) + ft_strlen(entry) - name_len + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(new, cmd->args[index], i + 1);
	ft_strlcat(new, entry, len);
	ft_strlcat(new, cmd->args[index] + i + name_len, len);
	if (spaces)
		free(entry);
	free(cmd->args[index]);
	cmd->args[index] = new;
}

static void	expand_var(t_ast *node, t_cmd *cmd, char *name, int index)
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
			return (handle_var(node, cmd, envp[i] + j + 1, index));
		}
	}
	free(name);
	return (handle_var(node, cmd, "", index));
}

static void	expand_arg(t_ast *node, t_cmd *cmd, int i)
{
	char	*name;

	while (contains_dol(cmd->args[i]))
	{
		if (is_exit_status(cmd->args[i]))
			expand_exit_status(node, cmd, i);
		else
		{
			name = get_name(node, cmd->args[i]);
			expand_var(node, cmd, name, i);
		}
	}
}

static void	expand(t_ast *node, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (is_whitespace(cmd->args[i]))
			i = skip_whitespaces(cmd, i);
		else if (!ft_strncmp(cmd->args[i], "\"", 2))
			i = expand_in_double(node, cmd, ++i);
		else if (!ft_strncmp(cmd->args[i], "'", 2))
			i = skip_single(cmd, ++i);
		else if (contains_dol(cmd->args[i]))
			expand_arg(node, cmd, i++);
		else
			i++;
	}
}

void	expander(t_ast *node, t_cmd *cmd)
{
	char	**args;
	int		count;

//	printf("Args before expansion:\n");
//	int j = -1;
//	while (cmd->args[++j])
//		printf("arg[%d]: '%s'\n", j, cmd->args[j]);
	expand(node, cmd);
//	printf("\nArgs after expansion:\n");
//	j = -1;
//	while (cmd->args[++j])
//		printf("arg[%d]: '%s'\n", j, cmd->args[j]);
	count = count_args(cmd);
	args = init_args(node, count + 1);
	make_args(node, cmd, args, count);
//	printf("\nArgs after cat:\n");
//	j = -1;
//	while (cmd->args[++j])
//		printf("arg[%d]: '%s'\n", j, cmd->args[j]);
}
