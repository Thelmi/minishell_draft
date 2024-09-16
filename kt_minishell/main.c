/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:58:22 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/15 22:40:09 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **ev){
    (void)ac;
    (void)av;
    char *command;
    t_env *envir;
	t_export *exp;
    int last_exit_status;
    configure_terminal_behavior();
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
	last_exit_status = 0; 
    envir = storing_env(ev);
	exp = storing_export(ev);
    while(1)
    {
        command = readline("minishell$ ");
        if (command == NULL) {
		write(1, "exit\n", 5);
		break;
        }
        if (*command) {
            add_history(command);
            //envir = execute_command(command, &envir, &last_exit_status, ev);
			runcmd(parsecmd(command, &last_exit_status), ev, &envir, &exp, &last_exit_status);
        }
        free(command);
    }
    free_env(envir);
    free_export(exp);
    return (last_exit_status);
}
