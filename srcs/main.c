/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:31:12 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/12 18:47:11 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	print_data(t_shell data)
{
	int	i = -1;

	while (data.envp[++i])
		printf("data.envp[%d]: %s\n", i, data.envp[i]);
	i = -1;
	while (data.paths[++i])
		printf("data.paths[%d]: %s\n", i, data.paths[i]);
}

static void	process_command(char *command, t_shell *data)
{
	t_ast	*ast;
	
	add_history(command);
	data->state = EXECUTING;
	data->cmd = command;
	setup_execution_signals(command, data);
	ast = parse(command, data);
	if (ast)
	{
//		printf("\n");
//		print_tree(ast);
		data->exit_status = exec_ast(ast);
		clean_ast(ast);
		data->root = NULL;
	}
	if (g_signal_received == SIGINT)
	{
		data->exit_status = 130;
		g_signal_received = 0;
	}
	else if (g_signal_received == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		data->exit_status = 131;
		g_signal_received = 0;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell	data;
	char	*command;
	char	cwd[256];
	int		should_exit;

	if (ac != 1)
		return (1);
	(void) av;
	data = init_shell_data(envp);
	should_exit = 0;
	while (!should_exit)
	{
		data.state = INTERACTIVE;
		setup_interactive_signals(&data);	// in loop ??
		get_trunc_cwd(cwd, &data);
		command = readline(cwd);
		if (!command)
		{
			should_exit = printf("exit\n");
			continue ;
		}
		else if (*command)
			process_command(command, &data);
		free(command);
		data.cmd = NULL;
	}
	return (clean_data(&data));
}
