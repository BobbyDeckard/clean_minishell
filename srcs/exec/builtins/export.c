/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:26:55 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/30 17:14:03 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	has_equal(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '=')
			return (1);
	}
	return (0);
}

static void	print_export(t_ast *node, char **abc)
{
	int	i;
	int	j;

	i = -1;
	while (abc[++i])
	{
		ft_putstr_fd("declare -x ", node->cmd.fd_out);
		j = 0;
		while (abc[i][j] && abc[i][j] != '=')
			ft_putchar_fd(abc[i][j++], node->cmd.fd_out);
		if (abc[i][j])
		{
			ft_putchar_fd(abc[i][j++], node->cmd.fd_out);
			ft_putchar_fd('"', node->cmd.fd_out);
			ft_putstr_fd(abc[i] + j, node->cmd.fd_out);
			ft_putchar_fd('"', node->cmd.fd_out);
		}
		ft_putchar_fd('\n', node->cmd.fd_out);
	}
}

static	void	order(char **abc)
{
	char	*ptr;
	int		i;
	int		j;

	i = -1;
	while (abc[++i])
	{
		j = i;
		while (abc[++j])
		{
			if (ft_strncmp(abc[i], abc[j], ft_strlen(abc[i]) + 1) > 0)
			{
				ptr = abc[i];
				abc[i] = abc[j];
				abc[j] = ptr;
			}
		}
	}
}

static int	export_print(t_ast *node, int size)
{
	char	**abc;
	int		i;

	abc = (char **) ft_calloc(size + 1, sizeof(char *));
	if (!abc)
		malloc_error(node, node->data, NULL);
	i = -1;
	while (node->data->envp[++i])
		abc[i] = copy_env_entry(node, abc, i);
	order(abc);
	print_export(node, abc);
	free_char_array(abc);
	close_all_redirs(node);
	return (set_exit_status(node, 0));
}

int	export_bltn(t_ast *node)
{
	int	status;
	int	size;

	status = 0;
	if (make_redirs(node))
		return (set_exit_status(node, 1));
	size = char_arr_len(node->data->envp);
	if (node->cmd.args[1] && size == -1)	// to check !
		return (create_env(node));
	else if (size == -1)
		return (set_exit_status(node, 0));
	else if (!node->cmd.args[1])
		return (export_print(node, size));
	status = handle_export_args(node, size);
	close_all_redirs(node);
	return (status);
}
