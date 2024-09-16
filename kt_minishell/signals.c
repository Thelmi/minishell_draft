
#include "minishell.h"

void configure_terminal_behavior() {
    struct termios term;

    tcgetattr(STDIN_FILENO, &term);         // Get terminal attributes
    term.c_lflag &= ~ECHOCTL;               // Disable ^C and ^\ echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &term); // Set new terminal attributes
}

void sigint_handler(int sig) {
    (void)sig; 
    write(1, "\n", 1);  
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay(); 
}

void sigquit_handler(int sig) {
    (void)sig;
    write(1, "", 1);
}
