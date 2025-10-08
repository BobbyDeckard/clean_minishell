/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 20:30:44 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/08 15:04:55 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	get_name_len(const char *str)
{
	int	i;

	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

int	handle_exit_status(t_ast *node, t_cmd *cmd, int index)
{
	char	*exit_status;
	char	*new;
	int		len;
	int		i;

	exit_status = ft_itoa(node->data->exit_status);
	if (!exit_status)
		malloc_error(node, node->data, NULL);
	i = 0;
	while (cmd->args[index][i] && cmd->args[index][i] != '$')
		i++;
	len = ft_strlen(cmd->args[index]) + ft_strlen(exit_status) - 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
	{
		free(exit_status);
		malloc_error(node, node->data, NULL);
	}
	ft_strlcpy(new, cmd->args[index], i + 1);
	ft_strlcat(new, exit_status, len);
	ft_strlcat(new, cmd->args[index] + i + 2, len);
	free(exit_status);
	free(cmd->args[index]);
	cmd->args[index] = new;
	return (0);
}

static int	contains_contig_spaces(const char *str)
{
	int	space;
	int	i;

	i = -1;
	space = 0;
	while (str[++i])
	{
		if (str[i] == ' ' && !space)
			space = !space;
		else if (str[i] == ' ')
			return (1);
		else if (space)
			space = !space;
	}
	return (0);
}

static int	get_last_non_space(const char *str)
{
	int	last_space;
	int	last_nonsp;
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == ' ')
			last_space = i;
		else
			last_nonsp = i;
	}
	if (last_space > last_nonsp)
		return (last_nonsp);
	return (-1);
}

static int	get_shortened_len(const char *str, int count, int space)
{
	int	last_nonsp;
	int	i;

	i = -1;
	last_nonsp = get_last_non_space(str);
	while (str[++i])
	{
		if (str[i] == ' ' && i > last_nonsp)
			break ;
		else if (str[i] == ' ' && !space)
		{
			space = !space;
			count++;
		}
		else if (str[i] != ' ' && space)
		{
			space = !space;
			count++;
		}
		else if (str[i] != ' ')
			count++;
	}
	return (count);
}

static void	make_new_entry(char *str, char *old, int len)
{
	int	space;
	int	i;
	int	j;
	
	i = -1;
	j = -1;
	while (old[++i] && ++j < len)
	{
		if (old[i] == ' ' && !space)
		{
			space = !space;
			str[j] = old[i];
		}
		else if (old[i] == ' ')
			i++;
		else
		{
			if (space)
				space = !space;
			str[j] = old[i];
		}
	}
}

static char	*filter_spaces(t_ast *node, char *entry)
{
	char	*new;
	int		len;

	len = get_shortened_len(entry, 0, 0) + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->data, NULL);
	make_new_entry(new, entry, len);
	return (new);
}

int	handle_var(t_ast *node, t_cmd *cmd, char *entry, int index)
{
	char	*new;
	int		name_len;
	int		len;
	int		i;

	i = 0;
	while (cmd->args[index][i] && cmd->args[index][i] != '$')
		i++;
	name_len = get_name_len(cmd->args[index] + i);
	if (cmd->exp[index] == 1 && contains_contig_spaces(entry))
		entry = filter_spaces(node, entry);
	len = ft_strlen(cmd->args[index]) + ft_strlen(entry) - name_len + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->data, NULL);
	ft_strlcpy(new, cmd->args[index], i + 1);
	ft_strlcat(new, entry, len);
	ft_strlcat(new, cmd->args[index] + i + name_len, len);
	free(cmd->args[index]);
	cmd->args[index] = new;
	return (0);
}

static int	remove_arg(t_cmd *cmd, int index)
{
	int	i;

	free(cmd->args[index]);
	i = index;
	while (i < cmd->arg_count)
	{
		cmd->args[i] = cmd->args[i + 1];
		if (i + 1 < cmd->arg_count)
		{
			cmd->exp[i] = cmd->exp[i + 1];
			cmd->cat[i] = cmd->cat[i + 1];
		}
		i++;
	}
	cmd->arg_count--;
	printf("wtf\n");
	return (1);
}

int	remove_var(t_ast *node, t_cmd *cmd, int index)
{
	char	*new;
	int		name_len;
	int		len;
	int		i;

	i = 0;
	while (cmd->args[index][i] && cmd->args[index][i] != '$')
		i++;
	name_len = get_name_len(cmd->args[index] + i);
	if (cmd->exp[index] == 1 && i == 0 && (int) ft_strlen(cmd->args[index]) == name_len)
		return (remove_arg(cmd, index));
	len = ft_strlen(cmd->args[index]) - name_len + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->data, NULL);
	ft_strlcpy(new, cmd->args[index], i + 1);
	ft_strlcat(new, cmd->args[index] + i + name_len, len);
	free(cmd->args[index]);
	cmd->args[index] = new;
	return (0);
}
