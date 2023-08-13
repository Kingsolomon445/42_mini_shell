#include "include/minishell.h"

static int memory_allocation(char **res, const char *start, int letter_count, int word_index)
{
    res[word_index] = (char *)malloc((letter_count + 1) * sizeof(char));
    if (!res[word_index])
	{
        return 0;
    }
    strlcpy(res[word_index], start, letter_count + 1);
    return 1;
}

int is_quote(char c) {
    return c == '"' || c == '\'';
}

char **ft_split_input(const char *s, char c, int cmd_no)
{
	const char *str;
	int	word_index;
	int	letter_count;
	const char *start;
	int	in_quotes;
	char **res;

    if (!s)
        return NULL;
    str = s;
    word_index = 0;
    res = (char **)malloc((cmd_no + 1) * sizeof(char *));
    if (!res)
        return NULL;
    while (*str)
	{
       	letter_count = 0;
        start = str;
        in_quotes = 0;
        while (*str && (*str != c || in_quotes))
		{
            if (is_quote(*str))
                in_quotes = !in_quotes;
            letter_count++;
            str++;
        }

        if (letter_count > 0)
		{
            if (!memory_allocation(res, start, letter_count, word_index))
			{
                return NULL;
            }
            word_index++;
        }

        if (*str)
            str++;
    }
    res[word_index] = NULL;
    return res;
}


void split_and_validate(const char *input) {
    int in_quotes = 0;
    int pipe_count = 0;
    int valid = 1;
    
    for (int i = 0; input[i]; i++) {
        if (is_quote(input[i]))
            in_quotes = !in_quotes;
        else if (input[i] == '|' && !in_quotes) {
            pipe_count++;
            if (i == 0 || input[i + 1] == '\0' || input[i - 1] == '|') {
                valid = 0;
                break;
            }
        }
    }

    if (in_quotes || input[strlen(input) - 1] == '|')
        valid = 0;

    if (pipe_count == 0 || !valid) {
        printf("Syntax error with pipes.\n");
        return;
    }

    // Splitting logic
    printf("Splitting input:\n");
    char **command = ft_split_input(input, '|', pipe_count+1);
    int i = 0;
    while (command[i]) {
        printf("Command: %s\n", command[i]);
        i++;
    }
}

int main() {
    const char *input = "command1 arg1 \"||\" command2 | command3 arg3";
    split_and_validate(input);
    return 0;
}
