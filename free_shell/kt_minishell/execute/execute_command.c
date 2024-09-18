/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:27:04 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/10 21:03:16 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//static bool is_executable(char *command){
//    (void)command;
//    return false;
//}

bool is_builtin(char *command){
    if(ft_strcmp("echo", command)==true||ft_strcmp("cd", command)==true || ft_strcmp("pwd", command)==true) {
        return true;
    }else if(ft_strcmp("export", command)==true || ft_strcmp("unset", command)==true || ft_strcmp("env", command)==true){
        return true;
    }else if(ft_strcmp("exit", command)==true){
        return true;
    }else{
        return false;
    }
}

//t_env *execute_command(char *command, t_env **envir, int *last_exit_status, char** ev)
//{
//    // char **args;

//    // args = ft_split(command, ' '); // just for testing later we can parse better (temporary)
//    // if (!args || !args[0])
//    //     return envir;
//    // int fd[2];
//    // pipe(fd);
//    // (void)fd;
//    // if (fork1() == 0)
//    // {
//    //     close (fd[0]);
//        // printf("first %d\n",  fd[1]);
//    //    print_tree(parsecmd(command));
//        runcmd(parsecmd(command), ev, envir);
//        // fd parameter as well... was there
//        //we might need to exit when it is a built in, to stop the child process
//    // }

//    // wait(NULL);
//    // close (fd[1]); //pipe communication
//    // printf("%s\n", get_next_line(fd[0]));
//    // char buff[1000];
//    // int count = read(fd[0], buff, 100);
//    // buff[count] = '\0';
//    // // printf("%s\n", buff);
//    // close (fd[0]);
    
//    (void)last_exit_status;
//    (void)is_builtin; // YOU CAN VOID FUNCTIONS YAAY
//    (void)is_executable;





//    // modify_args(args, envir);
//    // if (is_builtin(args[0])==true)
//    // {
//    //     envir = execute_builtin(envir , args, last_exit_status);
//	// }
//    // else if(is_executable(args[0])==true){
//    //     // execute_external(command);
//    //     *last_exit_status = 0; // assuming command executed succesfully
//    // }else{
//    //     printf("bash:command not found: %s\n", args[0]); // modifying this based on the bash syntax
//    //     *last_exit_status = 127; // status command not found
//    // }
//    // free_arr(args);
//    return (*envir);
//}
