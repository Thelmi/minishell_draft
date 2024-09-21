
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

/* Function that will look for the path line inside the environment, will
 split and test each command path and then return the right one. */
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
		{
			free_arr(paths);
			return (path);
		}
		free(path);
		i++;
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (NULL);
}

void runcmd(t_main main, char **ev, t_env **envir, t_export **exp, int *last_exit_status)
{
	
	int p[2];
	struct pipecmd *pcmd;
	int pipe_fd;
  struct execcmd *ecmd;
	struct redircmd *rcmd;
	int saved_stdin = dup(STDIN_FILENO); //close all fds
	int saved_stdout = dup(STDOUT_FILENO);
  char *tmp;
	int status = 0;
  char *tmp3;
  char *tmp2;
  char *read = NULL;
  int i;
  char *tmp_input;
  char *tmp_input2;
  static char *input;
  t_heredoc *tmp_h;

  struct cmd *cmd = main.cmd;
  // struct cmd *tmp_main;
  if (cmd == NULL)
    return;
  // (void)ev;
  // (void)envir;
  // (void)exp;
  
  // free(main.command);
  if (!input)
  {
    input = NULL;
  }
  while (main.heredoc)
  {
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
    // printf("first tmp2 |%s|\n", tmp2);
    if (tmp2 == NULL) 
    {
          perror("Error allocating memory for heredoc delimiter");
          *last_exit_status = 1; // General error
          return ;
    }
    // while ((read = readline("> ")) != NULL) {

    while (1)
    {
      write (1, "> ", 2);
      read = get_next_line(0);
      tmp_input = ft_substr(read, 0, ft_strlen(read) - 1);
      free (read);
      read = tmp_input;
      if (read == NULL)
        break ;
      // printf("read |%s|\n", read);
      // printf("tmp2 |%s|\n", tmp2);
      if (num_strncmp(read, tmp2) == 0) {
          // printf("YES\n");
          free(read);
          read = NULL;
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
          free(tmp_input);
        }
        input = tmp_input2;
      }
      free(read);
    }
    // if (read == NULL)  //check if this is important
    // {
    //   free(tmp2);
    //   break;
    // }
    free(tmp2);
    tmp_h = main.heredoc;
    main.heredoc = main.heredoc->next;
    free(tmp_h);
    tmp_h = NULL;
  }
  // freecmd(main.cmd, 0);
  freeheredoc(main.heredoc); //this was after freeing heredoc


	if (cmd->type == EXEC) 
	{
		ecmd = (struct execcmd*)cmd;
		if (ecmd->argv[0] == NULL)
		{
			*last_exit_status = 1;
      if (input)
        free(input);
      input = NULL;
			return ; 
		}
    if (is_builtin(ecmd->argv[0])==true) //free as a builtin, only the execcmd
    {
      // printf("%s\n", ecmd->argv[0]);
      // printf("%s\n", ecmd->argv[1]);
      if (input)
      {
        free (input);
      }
      input = NULL;
      execute_builtin(envir , ecmd->argv, ecmd->echar, last_exit_status, exp);
      // free(ecmd->argv[1]);
      // freecmd(main.cmd, 0);
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
    {
      free (input);
    }
    input = NULL;
    if (fork() == 0)
    {
      // write(1, main.command, ft_strlen(main.command));
      // free(main.command);
      remove_quotes(cmd);
      // printf("%s\n",ecmd->argv[0]);
      // if (execve(ecmd->argv[0], ecmd->argv, ev) == -1)
      //   execve(find_path(ecmd->argv[0], ev), ecmd->argv, ev); //you should free properly, make sure strjoin is not leaking
      if (execve(ecmd->argv[0], ecmd->argv, (*envir)->ev) == -1)
			execve(find_path(ecmd->argv[0], (*envir)->ev), ecmd->argv, (*envir)->ev);
      
      *last_exit_status = 127;
      // free(main.command);
      freecmd(main.cmd, 0);
      free_double_pointer((*envir)->ev);
      free_env(*envir);
      free_export(*exp);
      perror("execve failed"); // change it...
      // freeheredoc(main.heredoc);
      //free here if there is an error
      exit(127);
    }
    else 
    {
      wait(&status);
      *last_exit_status = WEXITSTATUS(status);

      
      // freecmd(main.cmd, 0);
      // freeheredoc(main.heredoc); //freeing here - reached this point
      return ;
    }
	} 
	else if (cmd->type == REDIR) 
	{
		rcmd = (struct redircmd*)cmd;
    tmp = NULL;
    if (input)
    {
      free (input);
    }
    input = NULL;
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
        // printf("hi\n");
      if (tmp)
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
    if (input)
    {
      free (input);
    }
    input = NULL;
		if (pipe(p) < 0)
			panic("pipe failed");
		ecmd = (struct execcmd*)pcmd->left;
		if (ecmd->argv[0] == NULL)
		{
			printf("bash: syntax error near unexpected token `|\n");
			*last_exit_status = 2;
			return ;
		}
    // free(main.command);
		if (fork1() == 0) 
		{
      // printf("YES\n");
			close(p[0]);
			if (dup2(p[1], STDOUT_FILENO) < 0)
				panic("dup2 failed");
			close(p[1]);
      // tmp_main = main.cmd;
      main.cmd = pcmd->left; //save the address correctly before modifying it for the next call. otherwise, you'll lose the pipe node and left/right orders and addressses
			runcmd(main, ev, envir, exp, last_exit_status);
      free(main.command);
      free_double_pointer((*envir)->ev);
      free_env(*envir);
      free_export(*exp);
      freecmd(main.main_cmd, 0);
			exit(*last_exit_status);
      // printf("NO\n");
		}
		wait(&status);
    // return ;
    // exit(0); //I am here
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
            freecmd(main.cmd, 0);
            free(main.command);
            t_main x = parsecmd(read, last_exit_status);
            runcmd(x, ev, envir, exp, last_exit_status); //WARNING!!! recursive call
            if (x.cmd)
                freecmd(x.cmd, 0);
            free(read);
            read = NULL;
            free_double_pointer((*envir)->ev);
            free_env(*envir);
            free_export(*exp);
            exit(0);
				}
			}
      // tmp_main = main.cmd;
      main.cmd = pcmd->right;
			runcmd(main, ev, envir, exp, last_exit_status); //WARNING!!! recursive call
      // free(tmp_main);
      free(main.command);
      free_double_pointer((*envir)->ev);
      free_env(*envir);
      free_export(*exp);
      freecmd(main.main_cmd, 0);
			exit(*last_exit_status);
		}
		close(p[0]);
		close(p[1]);
		wait(&status);
	  *last_exit_status = WEXITSTATUS(status);
	}
 


  // freecmd(main.cmd, 0);
  // freeheredoc(main.heredoc);
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
// printf("yxYES\n");
  while(peek(ps, es, "<>"))
  {
    // printf("xYES\n"); //m
    tok = gettoken(ps, es, 0, 0);
    if(gettoken(ps, es, &q, &eq) != 'a')
    {
		write(2, "bash: syntax error near unexpected token\n", 41);
		*last_exit_status = 2;
    // print_tree(cmd);
		freecmd(cmd, 1); // Indicating a syntax error (like bash)
    cmd = NULL;
		return (NULL);
    }
    // if (tok == '<' && (q + 1) && (*(q + 1) != ' '))
    // printf ("|%c|\n", *(q - 1));
    if(tok == '<') {
      // if (((q - 1) && (*(q - 1) != ' ') || (q + 1)(*(q + 1) != ' ')))
      // {

      // }
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
    // printf("xHoooooh\n");

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
// (void)ps;
// (void)es;
// (void)heredoc;
// (void)tok;
// (void)eq;
// (void)argc;
// (void)cmd;
// (void)q;
  argc = 0;
  ret = parseredirs(ret, ps, es, heredoc, last_exit_status); 
  if (!ret)
    return (NULL);
  while(ret && !peek(ps, es, "|")){ // I added ret
    if((tok=gettoken(ps, es, &q, &eq)) == 0)
      break;
    // printf("x1%s\n", *ps);
    // printf("x1cYES\n");
    if (tok != 'a')
    {
		*last_exit_status = 2;
        write(2, "bash: syntax error near unexpected token\n", 41);
        // cmd->argv[argc] = 0;
        // cmd->eargv[argc] = 0;
        freecmd(ret, 1);
        // printf("Hoooooh\n");
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
    // if (ret)
    //   printf("Hoooooh\n");

    // printf("x2%s\n", *ps);
  }
  if (ret && cmd && cmd->argv[argc]) //&& cmd->argv
    cmd->argv[argc] = 0;
  if(ret && cmd && cmd->eargv[argc]) //&& cmd->eargv 
    cmd->eargv[argc] = 0;
  return ret;
}

struct cmd* parsepipe(char **ps, char *es, struct heredoc **heredoc, int *last_exit_status)
{
  struct cmd *cmd;
  
  cmd = parseexec(ps, es, heredoc, last_exit_status);
  if(cmd && peek(ps, es, "|")){ //I added cmd
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
  // char *tmp;
  // char *tmp2;
  // char *tmp3;
  // int j;

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
      // // while (ecmd->argv[i][j] != ecmd->eargv[i])
    //   free(ecmd->argv[i]);
      ecmd->argv[i] = ft_substr(ecmd->argv[i], 0, ecmd->eargv[i] - ecmd->argv[i]);
      ecmd->echar[i] = ecmd->eargv[i][0];
      // printf("|%c|\n", ecmd->echar[i]);
      // if ((ecmd->argv[i] == '\"' || ecmd->argv[i] == '\'') && ecmd->argv[i + 1] && ecmd->argv[i + 1] != ' ')
      // {

      // }
      // printf("{%c}\n", ecmd->argv[i][ecmd->eargv[i] - ecmd->argv[i] - 1]);
      // printf("{%c}\n", ecmd->argv[i][ecmd->eargv[i] - ecmd->argv[i]]);
      // printf("oOOo i= %d\n", i);
//trying to fix echo spaces
// if (i > 0 && ecmd->argv[i + 1] && (ecmd->argv[i][ecmd->eargv[i] - ecmd->argv[i] - 1] == '\'' || ecmd->argv[i][ecmd->eargv[i] - ecmd->argv[i] - 1] == '\"')
//   && ecmd->argv[i][ecmd->eargv[i] - ecmd->argv[i]] && ecmd->argv[i][ecmd->eargv[i] - ecmd->argv[i]] != ' ')
// {
//   // printf("oOOo i= %d\n", i);
//   tmp2 = ft_substr(ecmd->argv[i] + 1, 0, ft_strlen(ecmd->argv[i]) - 2);
//   tmp3 = ft_substr(ecmd->argv[i + 1], 0, ecmd->eargv[i + 1] - ecmd->argv[i + 1]);
//   printf("%s\n", tmp2);
//   tmp = ft_strjoin(tmp2, tmp3);
//   free(tmp2);
//   free(ecmd->argv[i]);
//   free(tmp3);
//   ecmd->argv[i] = tmp;
//   j = i + 1;
//   while (ecmd->argv[j + 1])
//   {
//     printf("begin %s\n", ecmd->argv[j]);
//     printf("end %s\n", ecmd->eargv[j]);
//     ecmd->argv[j] = ecmd->argv[j + 1];
//     ecmd->eargv[j] = ecmd->eargv[j + 1];
//     j++;
//   }
//   printf("%s\n", ecmd->argv[i - 1]);
//   i--;

// }

      if (ecmd->argv[i] == NULL) 
      {
        perror("ft_substr allocation failed");
        return NULL;
      }
      // printf("{%s}\n", ecmd->argv[i]);
      // printf("|%c|\n", ecmd->eargv[i][0]);
      i++;
      // printf("333\n");
      
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

int check_quotes(char *str)
{
  int i = 0;
  int double_counter = 0;
  int single_counter = 0;

  while (str[i])
  {
    if (str[i] == '\"')
      double_counter++;
    if (str[i] == '\'')
      single_counter++;
    i++;
  }
  if (double_counter%2 != 0)
    return (1); //wrong input
  if (single_counter%2 != 0)
    return (1); //wrong input
  return (0); //
}
// struct cmd* parsecmd(char *s, int *last_exit_status)
t_main parsecmd(char *s, int *last_exit_status)
{
    char *es;
    struct cmd *cmd;
    t_main main; 
    main.heredoc = NULL;
    cmd = NULL;

    if (check_quotes(s)) //handling """ to avoid sigfault/invalid read
    {
      main.cmd = NULL;
      return (main);
    }
    es = s + strlen(s);
    main.command = s;
    cmd = parsepipe(&s, es, &(main.heredoc), last_exit_status); //free tree, heredoc //free here if there is an error, otherwise, free in runcmd
    peek(&s, es, "");
    if (cmd && s != es) {
        write(2, "minishell: syntax error: unexpected token ", 42);
		write(2, s, strlen(s));
    	*last_exit_status = 2;
    	freecmd(cmd, 1);
    	freeheredoc(main.heredoc);
		return main; //double check this
    }
    // free(cmd);
    // if (cmd == NULL)
    //   printf("WOoooooowwwwW\n");
    // if (main.heredoc == NULL)
      // printf("WOoooooowwwwW\n");
    nulterminate(cmd); //free argv in the tree properly. Free argv first and then the nodes in the tree
    main.cmd = cmd;
    main.main_cmd = cmd;
    // main.command = s;
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
