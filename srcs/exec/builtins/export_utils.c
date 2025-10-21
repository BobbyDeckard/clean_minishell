/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:16:44 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/20 21:19:31 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	update_paths(t_ast *node, t_shell *shell, const char *arg);
int		assign_var(t_ast *node, int size, int arg);
int		char_arr_len(char **arr);
int		create_var(t_ast *node, int size, int arg);
int		has_equal(const char *str);

/*
static int	name_is_invalid(char *arg)
{
	ft_putstr_fd("minishell: '", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (0);
}
*/
/*
static int	valid_name(char *arg)
{
	int	alpha;
	int	i;

	i = 0;
	alpha = 0;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalpha(arg[i]) && arg[i] != '_' && !alpha)
			return (name_is_invalid(arg));
		else if (ft_isalpha(arg[i]) && !alpha)
			alpha = 1;
		else if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (name_is_invalid(arg));
		i++;
	}
	if (!i)
		return (name_is_invalid(arg));
	return (1);
}
*/

int	handle_export_args(t_ast *node, int size)
{
	int	status;
	int	i;

	i = 0;
	status = 0;
	while (node->cmd.args[++i])
	{

//		if (node->cmd.exp[i] != 2 && !valid_name(node->cmd.args[i]))
//		{
//			status = 1;
//			continue ;
//		}
		if (!ft_strncmp(node->cmd.args[i], "PATH=", 5))
			update_paths(node, node->shell, node->cmd.args[i] + 5);
		if (has_equal(node->cmd.args[i]) && assign_var(node, size, i))
			status = 1;
		else if (!has_equal(node->cmd.args[i]) && create_var(node, size, i))
			status = 1;
		size = char_arr_len(node->shell->envp);
	}
	return (status);
}

void	create_env_error(t_ast *node, char **new, int i)
{
	while (--i >= 0)
		free(new[i]);
	free(new);
	malloc_error(node, node->shell, NULL);
}

int	var_exists(char *name, char **envp)
{
	int	i;
	int	j;

	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (name[j] && name[j] == envp[i][j])
			j++;
		if (!name[j] && (!envp[i][j] || envp[i][j] == '='))
			return (1);
	}
	return (0);
}
