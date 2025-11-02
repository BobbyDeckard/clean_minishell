/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 14:32:22 by imeulema          #+#    #+#             */
/*   Updated: 2025/11/02 15:02:04 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	free_char_array(char **arr);
void	free_char_array_size(char **arr, int i);
int		get_len(t_cmd *cmd, int i);
int		is_whitespace(const char *str);
int		skip_whitespaces(t_cmd *cmd, int i);

static int	cat_double(t_cmd *cmd, int i, char *arg, int len)
{
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "\"", 2))
		ft_strlcat(arg, cmd->args[i++], len);
	if (cmd->args[i])
		i++;
	return (i);
}

static int	cat_single(t_cmd *cmd, int i, char *arg, int len)
{
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "'", 2))
		ft_strlcat(arg, cmd->args[i++], len);
	if (cmd->args[i])
		i++;
	return (i);
}

static char	*cat_arg(t_cmd *cmd, int *index)
{
	char	*arg;
	int		len;
	int		i;

	i = *index;
	len = get_len(cmd, i) + 1;
	arg = (char *) ft_calloc(len, sizeof(char));
	if (!arg)
		return (NULL);
	while (cmd->args[i] && !is_whitespace(cmd->args[i]))
	{
		if (!ft_strncmp(cmd->args[i], "\"", 2))
			i = cat_double(cmd, ++i, arg, len);
		else if (!ft_strncmp(cmd->args[i], "'", 2))
			i = cat_single(cmd, ++i, arg, len);
		else
			ft_strlcat(arg, cmd->args[i++], len);
	}
	*index = i;
	return (arg);
}

void	make_args(t_ast *node, t_cmd *cmd, char **args, int count)
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
			args[j] = cat_arg(cmd, &i);
			if (!args[j])
			{
				free_char_array_size(args, j);
				malloc_error(node, node->shell, NULL);
			}
			j++;
		}
	}
	free_char_array(cmd->args);
	cmd->args = args;
}

char	**init_args(t_ast *node, int count)
{
	char	**args;

	args = (char **) malloc(count * sizeof(char *));
	if (!args)
		malloc_error(node, node->shell, NULL);
	while (--count >= 0)
		args[count] = NULL;
	return (args);
}
