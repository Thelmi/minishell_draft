/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krazikho <krazikho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:47:43 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/17 18:48:11 by krazikho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void panic(char *s)
{
  printf("%s\n", s);
  exit(1);
}

int fork1(void)
{
  int pid;

  pid = fork();
  if(pid == -1)
    panic("fork");
  return pid;
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (NULL);
}

void runcmd(t_main main, char **ev, t_env **envir, t_export **exp, int *last_exit_status) {
	int p[2];
	struct execcmd *ecmd;
	struct pipecmd *pcmd;
	struct redircmd *rcmd;
	int saved_stdin = dup(STDIN_FILENO);
	int saved_stdout = dup(STDOUT_FILENO);
	int status = 0;
	int pipe_fd;
  char *tmp;
  // t_heredoc *tmph = main.heredoc;
  // t_heredoc *tmp2;
  char *tmp3;
  char *tmp2;
  char *read = NULL;
  int i;
  static char *input;
  char *tmp_input;
  char *tmp_input2;
  // t_heredoc *input;

  struct cmd *cmd = main.cmd;
  if (cmd == NULL)
    return;
  if (!input)
  {
    input = NULL;
  }
    while (main.heredoc) {
        i = 0;
        while (main.heredoc->argv[i] && main.heredoc->argv[i] != ' ')
		{
            i++;
        }
        tmp2 = ft_substr(main.heredoc->argv, 0, i);
        if (tmp2 && tmp2[0] == '\"' && tmp2[ft_strlen(tmp2) - 1] == '\"')
        {
          tmp3 = ft_substr(main.heredoc->argv, 1, ft_strlen(tmp2) - 2);
          free (tmp2);
          tmp2 = tmp3; // my bro
        } 
        if (tmp2 && tmp2[0] == '\'' && tmp2[ft_strlen(tmp2) - 1] == '\'')
        {
          tmp3 = ft_substr(main.heredoc->argv, 1, ft_strlen(tmp2) - 2);
          free (tmp2);
          tmp2 = tmp3; // my bro
        } 
		 if (tmp2 == NULL) 
		 {
            perror("Error allocating memory for heredoc delimiter");
            *last_exit_status = 1; // General error
            return ;
      }
        while ((read = readline("> ")) != NULL) {
          if (num_strncmp(read, tmp2) == 0) {
              free(read);
              break;
          }
          if (main.heredoc->next == NULL)
          {
            tmp_input = ft_strjoin(read, "\n");
            if (!input)
              tmp_input2 = tmp_input;
            else
            {
              tmp_input2 = ft_strjoin(input, tmp_input);
              free(input);
            }
            if (input)
              free(tmp_input);
            input = tmp_input2;
          }
          free(read);
        }
        if (read == NULL)  //check if this is important
        {
          free(tmp2);
          break;
        }
        free(tmp2);
        main.heredoc = main.heredoc->next;
    }

	if (cmd->type == EXEC) 
	{
		ecmd = (struct execcmd*)cmd;
		if (ecmd->argv[0] == NULL)
		{
			*last_exit_status = 1;
			return ; 
		}
	if (is_builtin(ecmd->argv[0])==true) //free as a builtin, only the execcmd
	{
		execute_builtin(envir , ecmd->argv, last_exit_status, exp);
		return ;
	}
  if (ft_strcmp(ecmd->argv[0], "cat") && ecmd->argv[1] == NULL && input)
  {
    write (1, input, ft_strlen(input));
    if (input)
      free (input);
    input = NULL;
    return ;
  }
  if (input)
      free (input);
    input = NULL;
	if (fork() == 0)
	{
		remove_quotes(cmd);
		if (execve(ecmd->argv[0], ecmd->argv, (*envir)->ev) == -1)
			execve(find_path(ecmd->argv[0], (*envir)->ev), ecmd->argv, (*envir)->ev); //you should free properly, make sure strjoin is not leaking
		perror("execve failed"); // change it...
		*last_exit_status = 127;
    //free here if there is an error
		exit(127);
	}
	else 
	{
		wait(&status);
		*last_exit_status = WEXITSTATUS(status);
		return ;
	}
	} 
	else if (cmd->type == REDIR) 
	{
		rcmd = (struct redircmd*)cmd;
    if (rcmd->file && !((rcmd->file[0] == '\"' || rcmd->file[0] == '\'') && rcmd->file[1] == '\0'))
    {
      if (rcmd->file && rcmd->file[0] == '\"' && rcmd->file[ft_strlen(rcmd->file) - 1] == '\"')
        {
          tmp = ft_substr(rcmd->file, 1, ft_strlen(rcmd->file) - 2);
          // if ()
          // free (rcmd->file);
          rcmd->file = tmp; //we need to free the file name after using it
        } 
        if (rcmd->file && rcmd->file[0] == '\'' && rcmd->file[ft_strlen(rcmd->file) - 1] == '\'')
        {
          tmp = ft_substr(rcmd->file, 1, ft_strlen(rcmd->file) - 2);
          // if ()
          // free (rcmd->file);
          rcmd->file = tmp; //we need to free the file name after using it
        } 
      int fd = open(rcmd->file, rcmd->mode, 0644);
      free(tmp); //the same as free(rcmd->file);
      if (fd < 0) {
        perror("open failed");
        *last_exit_status = 1;
        return ;
      }
      if (dup2(fd, rcmd->fd) < 0) {
        perror("dup2 failed");
        close(fd);
        *last_exit_status = 1;
        return ;
      }
      close(fd);
    }
    main.cmd = rcmd->cmd;
		runcmd(main, ev, envir, exp, last_exit_status); // WARNING!!! make sure everything is free here, this is a recursive call WARNING!!!
		dup2(saved_stdout, 1);
		dup2(saved_stdin, 0);
		return;
	} 
	else if (cmd->type == PIPE) 
	{
		pcmd = (struct pipecmd*)cmd;
		if (pipe(p) < 0)
			panic("pipe failed");
		ecmd = (struct execcmd*)pcmd->left;
		if (ecmd->argv[0] == NULL)
		{
			printf("bash: syntax error near unexpected token `|\n");
			*last_exit_status = 2;
			return ;
		}
		if (fork1() == 0) 
		{
			close(p[0]);
			if (dup2(p[1], STDOUT_FILENO) < 0)
				panic("dup2 failed");
			close(p[1]);
            main.cmd = pcmd->left;
			runcmd(main, ev, envir, exp, last_exit_status);
			exit(*last_exit_status);
		}
		wait(&status);
		*last_exit_status = WEXITSTATUS(status);
		if (fork1() == 0) 
		{
			close(p[1]);
			if (dup2(p[0], STDIN_FILENO) < 0)
				panic("dup2 failed");
			close(p[0]);
			if (pcmd->right->type != PIPE)
			{
				ecmd = (struct execcmd*)pcmd->right;
				if (ecmd->argv[0] == NULL)
				{
				char *read = NULL;

				pipe_fd = dup(0);
				dup2(saved_stdin, 0);
				while (!read)
				{
					write (1, "> ", 2);
					read = get_next_line(0);
					if (read && read[0] != '\n')
					{
					dup2(pipe_fd, 0);
					break ;
					}
					free (read);
					read = NULL;
				}
                runcmd(parsecmd(read, last_exit_status), ev, envir, exp, last_exit_status); //WARNING!!! recursive call
			    exit(0);
				}
			}
            main.cmd = pcmd->right;
			runcmd(main, ev, envir, exp, last_exit_status); //WARNING!!! recursive call
			exit(*last_exit_status);
		}
		close(p[0]);
		close(p[1]);
		wait(&status);
	    *last_exit_status = WEXITSTATUS(status);
	}
	return ;
}

// Parsing 
int gettoken(char **ps, char *es, char **q, char **eq) // add herdoc
{
    char *s;
    int ret;
	char whitespace[6];
	char symbols[4];

	strcpy(whitespace, " \t\r\n\v");
	strcpy(symbols, "<|>"); 
    s = *ps;
    // printf("xx%s\n", s);
    while (s < es && strchr(whitespace, *s))
        s++;
    if (q)
        *q = s;
    ret = *s;

    if (*s == 0) 
    {
        return ret;
    } 
     else if (*s == '|')
    {
        s++;
    }
    else if (*s == '<') 
    {
        s++;
        if (*s == '<') 
        {
            ret = 'h';
            s++;
        } 
	}
    else if (*s == '>') {
        s++;
        if (*s == '>') {
            ret = '+';
            s++;
        }
    } 
    else {
        ret = 'a';
        while(s < es && strchr(whitespace, *s))
          s++;
        // printf("x1%s\n", *ps);
        if (s[0] == '\"')
        {
          s++;
          while(s < es && s[0] != '\"')
            s++;
          s++;
          // printf("%s\n", s);
        }
        else if (s[0] == '\'')
        {
          s++;
          while(s < es && s[0] != '\'')
            s++;
          s++;
        }
        else
        // printf("x1%s\n", s);
        {
          while (s < es && s[0] != '\"' && s[0] != '\'' &&!strchr(whitespace, *s) && !strchr(symbols, *s))
            s++;
        }
        // printf("x2%s\n", s);
    }
    // printf("|%s|\n", s);
    if (eq)
        *eq = s;
    while (s < es && strchr(whitespace, *s))
        s++;
    *ps = s;
    return ret;
}

int peek(char **ps, char *es, char *toks)
{
  char *s;

  char whitespace[6];
  strcpy(whitespace, " \t\r\n\v");
  s = *ps;
  while(s < es && strchr(whitespace, *s))
    s++;
  *ps = s;
  return (*s && strchr(toks, *s));
}

struct cmd* parseredirs(struct cmd *cmd, char **ps, char *es, struct heredoc **heredoc, int *last_exit_status) // add herdoc, and struct
{
  int tok;
  char *q, *eq;

  while(peek(ps, es, "<>"))
  {
    tok = gettoken(ps, es, 0, 0);
    if(gettoken(ps, es, &q, &eq) != 'a')
    {
		write(2, "bash: syntax error near unexpected token\n", 41);
		*last_exit_status = 2; // Indicating a syntax error (like bash)
		return NULL;
    }

    if(tok == '<') {
      cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
    } else if(tok == '>') {
      cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
    } else if(tok == '+') {  // >>
      cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_APPEND, 1);
    } else if(tok == 'h') {  // <<
        // printf("HELLOOO\n");
        redircmd_h(q, eq, heredoc);
      // cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_APPEND, 1);
    }
  }
  return cmd;
}

struct cmd* parseexec(char **ps, char *es, struct heredoc **heredoc, int *last_exit_status)
{
  char *q, *eq;
  int tok, argc;
  struct execcmd *cmd;
  struct cmd *ret;
  
  ret = execcmd();
  if (!ret) {
        perror("Error creating exec command");
        *last_exit_status = 1;
        return NULL;
    }
  cmd = (struct execcmd*)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es, heredoc, last_exit_status); 
  while(!peek(ps, es, "|")){
    if((tok=gettoken(ps, es, &q, &eq)) == 0)
      break;
    // printf("x1%s\n", *ps);
    if (tok != 'a')
    {
		*last_exit_status = 2;
        write(2, "bash: syntax error near unexpected token\n", 41);
        return NULL;
    }
    // if (q == cmd->eargv[argc - 1])
    //   q++; //here taha
    cmd->argv[argc] = q;
    cmd->eargv[argc] = eq;
    // printf("%s\n", cmd->eargv[argc]);
    argc++;
    //if(argc >= MAXARGS)  // do we really need this
    //  panic("too many args");
    ret = parseredirs(ret, ps, es, heredoc, last_exit_status);
    // printf("x2%s\n", *ps);
  }
  cmd->argv[argc] = 0;
  cmd->eargv[argc] = 0;
  return ret;
}

struct cmd* parsepipe(char **ps, char *es, struct heredoc **heredoc, int *last_exit_status)
{
  struct cmd *cmd;
  
  cmd = parseexec(ps, es, heredoc, last_exit_status);
  if(peek(ps, es, "|")){
    gettoken(ps, es, 0, 0);
    cmd = pipecmd(cmd, parsepipe(ps, es, heredoc, last_exit_status));
  }
  return cmd;
}

struct cmd* nulterminate(struct cmd *cmd)
{
  int i;
  // struct backcmd *bcmd;
  struct execcmd *ecmd;
  // struct listcmd *lcmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;
  // int j = 0;

  if (cmd == 0)
    return 0;
  if (cmd->type == EXEC) {
    ecmd = (struct execcmd*)cmd;
    i = 0;
    // printf("|%s|\n", ecmd->argv[0]);
    // printf("{%s}\n", ecmd->eargv[0]);
    // printf("|%s|\n", ecmd->argv[1]);
    // printf("{%s}\n", ecmd->eargv[1]);
    // printf("|%s|\n", ecmd->argv[2]);
    // printf("{%s}\n", ecmd->eargv[2]);
    while (ecmd->argv[i]) {
      // printf("[%s]\n", ecmd->argv[i]);
      // printf("{%s}\n", ecmd->eargv[i]);
      // *ecmd->eargv[i] = 0;
      // j = 0;
      // while (ecmd->argv[i][j] != ecmd->eargv[i])
      ecmd->argv[i] = ft_substr(ecmd->argv[i], 0, ecmd->eargv[i] - ecmd->argv[i]);
      // printf("{%s}\n", ecmd->argv[i]);
      // printf("|%c|\n", ecmd->eargv[i][0]);
      i++;
      
    }
    // printf("|%s|\n", ecmd->argv[0]);
    // printf("{%s}\n", ecmd->eargv[0]);
    // printf("|%s|\n", ecmd->argv[1]);
    // printf("{%s}\n", ecmd->eargv[1]);
    // printf("|%s|\n", ecmd->argv[2]);
    // printf("{%s}\n", ecmd->eargv[2]);
    // i = 0;
    // while (ecmd->argv[i]) {
    //   printf("{%s}\n", ecmd->argv[i]);
    //   // *ecmd->eargv[i] = 0;
    //   // printf("{%s}\n", ecmd->eargv[i]);
    //   i++;
    // }
  }
  else if (cmd->type == REDIR) {
    rcmd = (struct redircmd*)cmd;
    nulterminate(rcmd->cmd);
    *rcmd->efile = 0;
    // printf("%s\n", rcmd->file);
    // int fd = open(rcmd->file, rcmd->mode, 0644);
    // write(fd,"9", 1);
    // close(fd);
  }
  else if (cmd->type == PIPE)
  {
    pcmd = (struct pipecmd*)cmd;
    nulterminate(pcmd->left);
    nulterminate(pcmd->right);
  }
  return cmd;
}

struct cmd* remove_quotes(struct cmd *cmd)
{
  int i;
  // struct backcmd *bcmd;
  struct execcmd *ecmd;
  // struct listcmd *lcmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;
  char *tmp;

  if (cmd == 0)
    return 0;
  if (cmd->type == EXEC) {
    ecmd = (struct execcmd*)cmd;
    i = 0;
    while (ecmd->argv[i]) {
      if (ecmd->argv[i][0] == '\"' && ecmd->argv[i][ft_strlen(ecmd->argv[i]) - 1] == '\"')
      {
		tmp = ecmd->argv[i];
		ecmd->argv[i] = ft_substr(tmp, 1, ft_strlen(tmp) - 2);
		// free(tmp);
      }
      if (ecmd->argv[i][0] == '\'' && ecmd->argv[i][ft_strlen(ecmd->argv[i]) - 1] == '\'')
      {
		tmp = ecmd->argv[i];
		ecmd->argv[i] = ft_substr(tmp, 1, ft_strlen(tmp) - 2);
		// free(tmp);
      }
      i++;
    }
  }
  else if (cmd->type == REDIR) {
    rcmd = (struct redircmd*)cmd;
    remove_quotes(rcmd->cmd);
  }
  else if (cmd->type == PIPE) {
    pcmd = (struct pipecmd*)cmd;
    remove_quotes(pcmd->left);
    remove_quotes(pcmd->right);
  }
  return cmd;
}

// struct cmd* parsecmd(char *s, int *last_exit_status)
t_main parsecmd(char *s, int *last_exit_status)
{
    char *es;
    struct cmd *cmd;
    t_main main; 
    main.heredoc = NULL;
    cmd = NULL;

    es = s + strlen(s);
    cmd = parsepipe(&s, es, &(main.heredoc), last_exit_status); //free tree, heredoc //free here if there is an error, otherwise, free in runcmd
    peek(&s, es, "");
    if (s != es) {
        write(2, "minishell: syntax error: unexpected token ", 42);
		write(2, s, strlen(s));
    	*last_exit_status = 2;
		return main; //double check this
    }
    nulterminate(cmd); //free argv in the tree properly. Free argv first and then the nodes in the tree
    main.cmd = cmd;
    return (main);
}


// constructers
struct cmd* pipecmd(struct cmd *left, struct cmd *right)
{
  struct pipecmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  // if (left)
  //   cmd->left_available = 1;
  // else
  //   cmd->left_available = 0;
  // if (right)
  //   cmd->right_available = 1;
  // else
  //   cmd->right_available = 0;
  cmd->type = PIPE;
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

struct cmd* redircmd(struct cmd *subcmd, char *file, char *efile, int mode, int fd)
{
  struct redircmd *cmd;
  struct cmd *tmp;
  struct redircmd *tmp2;

  // printf("%s\n", file);
  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = REDIR;
  tmp = subcmd;
  if (subcmd && subcmd->type == REDIR)
  {
    tmp2 = (struct redircmd *)subcmd;
    while (tmp->type == REDIR)
    {
      tmp2 = (struct redircmd *)tmp;
      tmp = ((struct redircmd *)tmp)->cmd;
    }
    tmp2->cmd = (struct cmd*)cmd;
  }
  cmd->cmd = tmp;
  cmd->file = file;
  cmd->efile = efile;
  cmd->mode = mode;
  cmd->fd = fd;
  if (subcmd && subcmd->type == REDIR)
  {
    return (subcmd);
  }
  return (struct cmd*)cmd;
}

void redircmd_h(char *argv, char *eargv, struct heredoc **heredoc)
{
  struct heredoc *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = HEREDOC;
  cmd->argv = argv;
  cmd->eargv = eargv;
  cmd->next = NULL;

// printf("%s|\n", argv);
  if (*heredoc == NULL)
  {
    // create a node, new head
    *heredoc = cmd;
  }
  else
  {
    // add to the linked list
    struct heredoc *tmp = *heredoc;
    while (tmp->next)
		tmp = tmp->next;
	tmp->next = cmd;
  }
}

struct cmd* execcmd(void)
{
  struct execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = EXEC;
  return (struct cmd*)cmd;
}
