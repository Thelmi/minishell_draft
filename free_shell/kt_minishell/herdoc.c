// #include "minishell_practice.h"

// static size_t	t_strlen(const char *s)
// {
// 	size_t	i;

// 	i = 0;
// 	while (s[i] != '\0')
// 		i++;
// 	return (i);
// }

// int main(int ac, char **argv)
// {
// 	char	*read;
// 	char	input_strings[1000];
// 	char	**input_splitted;
// 	int		i;
// 	int		j;
// 	int		z;

// 	(void)ac;

// 	input_splitted = NULL;
// 	i = 0;
// 	j = 0;
// 	while (argv[i])
// 	{
// 		z = 0;
// 		if (argv[i + 1] && argv[i][0] == '<' && argv[i][1] == '<' && argv[i][2] == '\0')
// 		{
// 			while (argv[i + 1][z])
// 			{
// 				input_strings[j] = argv[i + 1][z];
// 				j++;
// 				z++;
// 			}
// 		}
// 		input_strings[j++] = ' ';
// 		i++;
// 	}
// 	input_strings[j++] = '\0';
// 	input_splitted = ft_split(input_strings, ' ');
// 	i = 0;
// 	while (read)
// 	{
// 		write(1, "> ", 2);
// 		read = get_next_line(0);
// 		read[t_strlen(read) - 1] = '\0';
// 		if (t_strcmp(read, input_splitted[i]) == 0)
// 		{
// 			i++;
// 			if (input_splitted[i] == NULL)
// 			{
// 				if (read)
// 					free(read);
// 				break ;
// 			}
// 		}
// 		if (read)
// 			free(read);
// 	}
// 	//free splited
// }
#include "minishell.h"

static size_t t_strlen(const char *s)
{
	size_t i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

int main(int ac, char **argv)
{
	char *read = NULL;
	char input_strings[1000] = {0};  // Initialize to avoid garbage values
	char **input_splitted = NULL;
	int i = 0, j = 0, z = 0;

	(void)ac;

	// Build the string of heredoc delimiters
	while (argv[i])
	{
		if (argv[i + 1] && argv[i][0] == '<' && argv[i][1] == '<' && argv[i][2] == '\0')
		{
			z = 0;
			while (argv[i + 1][z])
			{
				input_strings[j++] = argv[i + 1][z++];
			}
			input_strings[j++] = ' ';  // Add space between delimiters
		}
		i++;
	}
	input_strings[j] = '\0';  // Null-terminate the string

	// Split the input delimiters into an array
	input_splitted = ft_split(input_strings, ' ');

	// Handle heredoc input
	i = 0;
	while (input_splitted[i])  // Iterate through all delimiters
	{
		while (1)  // Keep reading until the delimiter is found
		{
			read = readline("> ");
			if (!read)
				break;

			if (t_strlen(read) > 0 && t_strcmp(read, input_splitted[i]) == 0)
			{
				free(read);  // Free the input string
				break;
			}

			// Process heredoc input as needed (e.g., write to a temp file or pipe)

			free(read);  // Free the input string
		}
		i++;
	}

	// Free allocated memory for input_splitted
	if (input_splitted)
	{
		i = 0;
		while (input_splitted[i])
		{
			free(input_splitted[i]);
			i++;
		}
		free(input_splitted);
	}

	return 0;
}

