/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:04:39 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/14 13:08:11 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h" 

int	ft_isalnum(int c)
{
	if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}

static int new_len(char *arg, t_env *envir, bool read_backslash){
    int len = 0;
    int i = 0; 
    char var_name[256];
    int var_len;
    while (arg[i]) { 
        if (arg[i] == '$' && (i == 0 || arg[i - 1] != '\\') && ft_isalnum(arg[i+1])){
            var_len = 0;
            i++;
            while (arg[i] && (ft_isalnum(arg[i]) || arg[i]=='_')) {
                var_name[var_len++] = arg[i++];
            }
            var_name[var_len] = '\0';
            if (getcopyenv(var_name, &envir)) {
                len += ft_strlen(getcopyenv(var_name, &envir));
            }
        } else if (arg[i]=='\\' && arg[i+1]!='$'){
            if(read_backslash){
                len++;
            }
            i++;
        }else{
            len++;
            i++;
        }
    }
    return len;
}

static char *no_quotes(char *arg, t_env *envir, bool read_backslash){
    int i;
    int j;
    int var_len;
    char *res;
    char var_name[256]; 
    i = 0;
    j = 0;
    res = malloc(sizeof(char)*(new_len(arg, envir, read_backslash) + 1));
    if (!res) {
        return NULL;
    }
    while (arg[i]) {
        if (arg[i] == '$' && ft_isalnum(arg[i+1])){
            var_len = 0;
            i++;
            while (arg[i] && (ft_isalnum(arg[i]) || arg[i]=='_')) {
                var_name[var_len++] = arg[i++];
            }
            var_name[var_len] = '\0';
            if (getcopyenv(var_name, &envir)) {
                ft_strcat(res, getcopyenv(var_name, &envir));
                j = ft_strlen(res); 
            }
        } else if (arg[i] == '\\' && ft_isalnum(arg[i+1])) {
            if (read_backslash) {
                res[j++] = arg[i++];
            }else{
                i++;
            }
        } else if (arg[i] == '\\' && !ft_isalnum(arg[i+1])) {
            i++;
            res[j++] = arg[i++];
        } else {
            res[j++] = arg[i++];
        }
    }
    res[j] = '\0'; 
    return res;
}


static char *double_quotes(char *arg, t_env *envir, bool read_backslash){
    char *res;
    int len;
    len=ft_strlen(arg);
    arg[len-1]='\0';
    arg=arg+1;
    res=no_quotes(arg, envir, read_backslash);
    return res;
}

static char *single_quotes(char *arg){
    int i;
    int len;
    char *res;
    i=0;
    len=ft_strlen(arg);
    res=malloc(sizeof(char)*(len+1-2));
    if (!res) 
        return NULL;
    while(i<len-2){
        res[i]=arg[i+1];
        i++;
    }
    res[i]='\0';
    return res;
}

void modify_args(char **args, t_env *envir){
    int i;
    char *tmp;
    bool read_backslash;
    read_backslash=false;
    i=1;
    while(args[i]){
        if(args[i][0]=='\'' && args[i][ft_strlen(args[i])-1]=='\''){
            tmp = single_quotes(args[i]);
        }else if(args[i][0]=='"' && args[i][ft_strlen(args[i])-1]=='"'){
            read_backslash=true;
            tmp = double_quotes(args[i], envir, read_backslash);
        }else {
            tmp = no_quotes(args[i], envir, read_backslash); 
        }
        // free(args[i]);
        args[i] = tmp;
        i++;
    }
}