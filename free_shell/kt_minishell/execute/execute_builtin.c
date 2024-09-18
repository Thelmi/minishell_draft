/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:55:09 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/14 17:32:18 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int count_args(char **args) 
{
    int count;
    
    count = 0;
    while (args[count] != NULL)
        count++;
    return (count);
}

t_env *execute_builtin(t_env **envir, char **args, int *last_exit_status, t_export **exp){
	int count;
	(void)last_exit_status;
	if (!args || !args[0]) 
        return (*envir);

	// printf("%s\n", args[0]);
    // printf("%s\n", args[1]);
    // printf("%s\n", args[2]);
	count = count_args(args);
	 modify_args(args, *envir, last_exit_status);

	 
	if (ft_strcmp("echo", args[0])==true){
		echo(args, *envir);
		 *last_exit_status = 0; // assuming it succeed
    }
    else if(ft_strcmp("cd", args[0])==true)
    {
		cd(args, envir, last_exit_status);
		 *last_exit_status = 0; // assuming it succeed
    }
    else if(ft_strcmp("pwd", args[0])==true)
    {
    	 pwd(last_exit_status);
		 *last_exit_status = 0; // assuming it succeed
    }
    else if(ft_strcmp("export", args[0])==true)
    {
        if (args[1] != NULL)
			export_with_args(envir, exp , count, args, last_exit_status);
		else 
            export_no_arg(*exp, last_exit_status);
		 *last_exit_status = 0; // assuming it succeed
    }
    else if(ft_strcmp("unset", args[0])==true)
    {
        if (args[1] != NULL)
           unset(envir, count, args, last_exit_status);
		 *last_exit_status = 0; // assuming it succeed
	}
  	else if(ft_strcmp("env", args[0])==true){
    	env(*envir, last_exit_status);
    	 *last_exit_status = 0; // assuming it succeed
  	}
  	else if(ft_strcmp("exit", args[0])==true){
    	builtin_exit(args, last_exit_status);
  	}
    return (*envir);
}
