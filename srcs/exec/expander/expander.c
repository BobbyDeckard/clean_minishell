/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 12:12:02 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/30 17:22:18 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	free_char_array_size(char **arr, int i);
int		contains_dol(const char *str);
int		count_args(t_cmd *cmd);
int		is_whitespace(const char *str);
int		skip_single(t_cmd *cmd, int i);
int		skip_whitespaces(t_cmd *cmd, int i);

static int	get_len(t_cmd *cmd, int i)
{
	int	len;

	len = 0;
	return (len);
}

static char	*make_arg(t_cmd *cmd, int *index)
{
	char	*arg;
	int		len;
	int		i;

	i = *index;
	len = get_len(cmd, i);
	*index = i;
}

static void	pop_new_args(t_ast *node, t_cmd *cmd, char **args, int count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd->args[i] && j < count)
	{
		if (is_whitespace(cmd->args[i]))
			i = skip_whitespaces(cmd, i);
		else
		{
			args[j] = make_arg(cmd, &i);
			if (!args[j])
			{
				free_char_array_size(args, j);
				malloc_error(node, node->shell, NULL);
			}
		}
	}
}

static char	**init_args(t_ast *node, int count)
{
	char	**args;

	args = (char **) malloc(count * sizeof(char *));
	if (!args)
		malloc_error(node, node->shell, NULL);
	while (--count >= 0)
		args[count] = NULL;
	return (args);
}

static int	expand_in_double(t_ast *node, t_cmd *cmd, int i)
{

}

static int	is_exit_status(const char *str)
{
	while (*str && *str != '$')
		str++;
	if (*(str + 1) && *(str + 1) == '?')
		return (1);
	return (0);
}

static void	handle_exit_status(t_ast *node, t_cmd *cmd, int index)
{
	char	*status;
	char	*new;
	int		len;
	int		i;

	status = ft_itoa(node->shell->exit_status);
	if (!status)
		malloc_error(node, node->shell, NULL);
	i = 0;
	while (cmd->args[index][i] && cmd->args[index][i] != '$')
		i++;
	len = ft_strlen(cmd->args[index]) + ft_strlen(exit_status) - 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
	{
		free(status);
		malloc_error(node, node->shell, NULL);
	}
	ft_strlcpy(new, cmd->args[index], i + 1);
	ft_strlcat(new, status, len);
	ft_strlcat(new, cmd->args[index] + i + 2, len);
	free(status);
	free(cmd->args[index]);
	cmd->args[index] = new;
}

static int	check_end(t_cmd *cmd, int i)
{
	int	j;

	j = 0;
	while (cmd->args[i][j] && cmd->args[i][j] != '$')
		j++;
	if (cmd->args[i][j + 1])
		return (0);
	return (1);
}

//	This doesn't take account for potential multiple $ in arg.
//	For example:
//		echo $USER$USR$
//	>	ivo$
static void	handle_end(t_ast *node, t_cmd *cmd, int index)
{
	if (!cmd->args[index + 1])
		return ;
}

static void	expand_arg(t_ast *node, t_cmd *cmd, int i)
{
	char	*name;

	while (contains_dol(cmd->args[i]))
	{
		if (is_exit_status(cmd->args[i]))
			handle_exit_status(node, cmd, i);
//		DO BETTER !!!!!!
//		else if (check_end(cmd, i))
//			handle_end(node, cmd, i);
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

	expand(node, cmd);
	count = count_args(cmd);
	args = init_args(node, count + 1);
	pop_new_args(node, cmd, args, count);
}
