/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:58:22 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/11 05:51:40 by thelmy           ###   ########.fr       */
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

	last_exit_status = 0; 
    envir = storing_env(ev);
	exp = storing_export(ev);
    while(1)
    {
        command = readline("minishell$ ");
        if (command == NULL) {
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
    return (last_exit_status);
}
