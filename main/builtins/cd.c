/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:00:29 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/11 03:56:40 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void cd(char **args, t_env **envir, int *last_exit_status){
    char *path;
    (void)envir;
    char cwd[1024];
    if(!args[1]){
        path=getenv("HOME");;
        if(!path){
             perror("cd: $HOME not set");
            *last_exit_status = 1;
            return;
        }
    }else if(args[1][0]=='-'){
        if(args[1][1]=='\0'){
            if(getcopyenv("OLDPWD", envir)!=NULL){
                path=getcopyenv("OLDPWD", envir);
            }else{
                perror("bash: cd: OLDPWD not set");
            	*last_exit_status = 1;
                return;
            }
        }
    }else{
            path=args[1];
    } 
    if(chdir(path)!=0){
        perror("cd");
        *last_exit_status = 1;
    }else{
        update_env_for_cd(envir, "OLDPWD", getcopyenv("PWD", envir));
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
            update_env_for_cd(envir, "PWD", cwd);
        } else {
            perror("cd: Error getting current working directory");
            *last_exit_status = 1;
        }        
    }
}
