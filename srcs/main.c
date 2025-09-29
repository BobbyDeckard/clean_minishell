/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:31:12 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/29 13:33:11 by imeulema         ###   ########.fr       */
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

static t_shell process_command(char *command, t_shell data)
{
	t_ast	*ast;
	
	add_history(command);
	data.state = EXECUTING;
	data.cmd = command;
	setup_execution_signals(command, &data);
	ast = parse(command, &data);
	if (ast)
	{
//		printf("\n");
//		print_tree(ast);
		data.exit_status = exec_ast(ast);
		clean_ast(ast);
		data.root = NULL;
	}
	if (g_signal_received == SIGINT)
	{
		data.exit_status = 130;
		g_signal_received = 0;
	}
	else if (g_signal_received == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		data.exit_status = 131;
		g_signal_received = 0;
	}
	return (data);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	data;
	char	*command;
	char	cwd[256];
	int		should_exit;

	int	n;
	int	i = 0;
	if (ac == 2)
		n = ft_atoi(av[1]);
	else if (ac != 1)
		return (1);
	(void) av;
	data = init_shell_data(envp);
//	init_minipath(&data, envp);
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
			data = process_command(command, data);
		free(command);
		data.cmd = NULL;
		if (n)
		{
			if (++i == n)
				should_exit = printf("Iteration limit met (%d)\n", n);
		}
	}
	return (clean_data(&data));
}
