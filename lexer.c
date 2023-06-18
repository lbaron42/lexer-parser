/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   'lexer.c'                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaron  <lbaron@student.42berlin.de>       :+:  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023-06-13 14:46:41 by lbaron            :+:    #+#             */
/*   Updated: 2023-06-13 14:46:41 by lbaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

#define BUFFER_SIZE 1024

int	ft_strcmp(const char *s1,const char *s2)
{
	int	i;
	int	ret;

	i = 0;
	while(s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
			i++;
	ret = s1[i] - s2[i];
	return (ret);
}

char	*ft_strncpy(char *dest, const char *src, int n)
{
	int i;

	i = 0;
	while(i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return(dest);
}

int		ft_word_count(const char *str)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
			i++;
		if (str[i])
			count++;
		while (str[i] && !(str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
			i++;
	}
	return (count);
}

void	ft_copy_words(char **out, char *str)
{
	int i;
	int j;
	int k;

	i = 0;
	k = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
			i++;
		j = i;
		while (str[i] && !(str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
			i++;
		if (i > j)
		{
			out[k] = (char*)malloc(((i - j) + 1) * sizeof(char));
			out[k] = ft_strncpy(out[k], &str[j], i - j);
			k++;
		}
	}
	out[k] = NULL;
}

char	**ft_split(char *str)
{
	char	**out;
	int		word_count;

	word_count = ft_word_count(str);
	out = (char**)malloc((word_count + 1) * sizeof(char*));
	if (!out)
		return (NULL);
	ft_copy_words(out, str);
	return (out);
}

void ft_free(char **split)
{
	if (split == NULL)
	{
		return;
	}

	int i = 0;
	while(split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

typedef struct s_token{
	int token_code;
	int index;
	char *token;
	char *value;
	struct s_token *next;
}t_token;

void print_list(t_token *head)
{
	t_token *temp;

	temp = head;
	while (temp)
	{
		printf("Token: %s\n", temp->token);
		printf("Value: %s\n", temp->value);
		printf("Token code: %d\n", temp->token_code);
		printf("Index: %d\n", temp->index);
		printf("\n");
		temp = temp->next;
	}
}

void free_t_token(t_token *head)
{
	t_token *temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->token);
		free(temp->value);
		free(temp);
	}
}

void	set_token(t_token *node, char *token, int code)
{
	node->token = strdup(token);
	node->token_code = code;
}

void	check_symbols(t_token *node)
{
	if (ft_strcmp("|", node->value) == 0)
		set_token(node, "Pipe", 2);
	else if (ft_strcmp(">", node->value) == 0)
		set_token(node, "Redirect_in", 2);
	else if (ft_strcmp("<", node->value) == 0)
		set_token(node, "Redirect_out", 2);
	else if (node->value[0] == '-')
		set_token(node, "Flag", 3);
	else if (ft_strcmp("<<", node->value) == 0)
		set_token(node, "Here_doc", 2);
	else if (ft_strcmp(">>", node->value) == 0)
		set_token(node, "Append_doc", 2);
	else
		set_token(node, "Argument", 1);
}

void	is_keyword(t_token *node)
{
	int		i;
	char	*keywords[] = {
			"grep", "wc", "mkdir", "mv", "cp", "rm", "rmdir",
			"touch", "whoami", "find", "head", "tail", "diff", "find",
			"cat", "ls", "date", "touch", NULL};
    char	*built_in[] = {
            "cd", "echo", "env", "exit", "export", "pwd", "unset",NULL};

	i = 0;
	while (keywords[i])
	{
		if (ft_strcmp(keywords[i], node->value) == 0)
		{
			set_token(node, "Command", 0);
			return ;
		}
		i++;
	}
    i = 0;
    while (built_in[i])
    {
        if (ft_strcmp(built_in[i], node->value) == 0)
        {
            set_token(node, "Built_in", 0);
            return ;
        }
        i++;
    }
	check_symbols(node);
}

t_token	*create_token_list(char *input, t_token *head)
{
	int		i;
	t_token	*current;
	char	**split;

	split = ft_split(input);
	i = 0;
	while (split[i])
	{
		t_token	*new_node = (t_token*)malloc(sizeof(t_token));
		new_node->index = i;
		new_node->value = split[i];
		new_node->next = NULL;
		is_keyword(new_node);
		if (!head)
			current = head = new_node;
		else
		{
			current->next = new_node;
			current = current->next;
		}
		i++;
	}
	free(split);
	return (head);
}

typedef struct s_command {
	char **command;
	char  *separator;
	char *arguments;
	struct s_command *next;
} t_command;

void free_t_command(t_command *head)
{
	t_command *temp;

	while (head)
	{
		temp = head;
		head = head->next;
		ft_free(temp->command);
		free(temp);
	}
}

void	redirect_stdout_to_file(char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)
		return ;
	if (dup2(fd, STDOUT_FILENO) == -1)
		return ;
	close(fd);
}

void	redirect_stdin_from_file(char *file_name)
{
	int	fd;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return ;  // handle error
	if (dup2(fd, STDIN_FILENO) == -1)
		return ;  // handle error
	close(fd);
}

void	append_stdout_to_file(char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
	if (fd == -1)
		return ;
	if (dup2(fd, STDOUT_FILENO) == -1)
		return ;
	close(fd);
}

void	read_stdin_from_string(char *str)
{
	int	pfd[2];

	pipe(pfd);
	write(pfd[1], str, strlen(str));
	close(pfd[1]);
	dup2(pfd[0], STDIN_FILENO);
	close(pfd[0]);
}

void add_command_node(t_command **current, t_token **tmp, char **path)
{
	int i;

	i = 0;
	(*current)->command = (char **)malloc(sizeof(char *));
	(*current)->command[i] = valid_cmd(path, (*tmp)->value);
	*tmp = (*tmp)->next;

	while (*tmp && (*tmp)->token_code != 2) {
		i++;
		(*current)->command = realloc((*current)->command, (i+2) * sizeof(char *));
		(*current)->command[i] = strdup((*tmp)->value);
		*tmp = (*tmp)->next;
	}
	if(i == 0){
		(*current)->command = realloc((*current)->command, (i+2) * sizeof(char *));
	}

	(*current)->command[i+1] = NULL;
	(*current)->separator = NULL;
	(*current)->arguments = NULL;
}

void add_separator_node(t_command **current, t_token *tmp)
{
	(*current)->separator = tmp->value;
	(*current)->command = NULL;
	(*current)->arguments = NULL;
}

void add_argument_node(t_command **current, t_token *tmp)
{
	(*current)->arguments = tmp->value;
	(*current)->command = NULL;
	(*current)->separator = NULL;
}

t_command *build_command_list(t_token *head, char **path)
{
	t_token *tmp;
	t_command *list_head;
	t_command *current;


	tmp = head;
	list_head = NULL;
	current = NULL;
	while (tmp)
	{
		t_command *new_node = (t_command*)malloc(sizeof(t_command));
		new_node->next = NULL;

		if (tmp->token_code == 0)
		{
			add_command_node(&new_node, &tmp, path);
		}
		else if (tmp->token_code == 2)
		{
			add_separator_node(&new_node, tmp);
			tmp = tmp->next;
		}
		else
		{
			add_argument_node(&new_node, tmp);
			tmp = tmp->next;
		}
		if (!list_head) {
			list_head = new_node;
			current = list_head;
		} else {
			current->next = new_node;
			current = current->next;
		}
	}
	return list_head;
}

void print_command_list(t_command *head)
{
	t_command *temp;

	temp = head;
	while (temp)
	{
		if(temp->command) {
			printf("Command: ");
			for(int i = 0; temp->command[i]; i++) {
				printf("%s ", temp->command[i]);
			}
			printf("\n");
		}

		if(temp->separator) {
			printf("Separator: %s\n", temp->separator);
		}

		if(temp->arguments) {
			printf("Arguments: %s\n", temp->arguments);
		}

		printf("\n");
		temp = temp->next;
	}
}

void execute_command(char **cmd, char **envp, int in_fd, int out_fd) {
	pid_t pid;

	pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) {
		if (in_fd != STDIN_FILENO) {
			if (dup2(in_fd, STDIN_FILENO) == -1) {
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			close(in_fd);
		}
		if (out_fd != STDOUT_FILENO) {
			if (dup2(out_fd, STDOUT_FILENO) == -1) {
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			close(out_fd);
		}

		if (execve(cmd[0], cmd, envp) == -1) {
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else {
		waitpid(pid, NULL, 0);
	}
}

void handle_redirection(t_command *current)
{
	if (current->next && current->next->separator)
	{
		if (strcmp(current->next->separator, ">") == 0)
		{
			if(current->next->next && current->next->next->arguments)
			{
				redirect_stdout_to_file(current->next->next->arguments);
			}
		}
		else if (strcmp(current->next->separator, "<") == 0)
		{
			if(current->next->next && current->next->next->arguments)
			{
				redirect_stdin_from_file(current->next->next->arguments);
			}
		}
		else if (strcmp(current->next->separator, ">>") == 0)
		{
			if(current->next->next && current->next->next->arguments)
			{
				append_stdout_to_file(current->next->next->arguments);
			}
		}
		else if (strcmp(current->next->separator, "<<") == 0)
		{
			if(current->next->next && current->next->next->arguments)
			{
				read_stdin_from_string(current->next->next->arguments);
			}
		}
	}
}

void tree(t_command *commands, char **envp)
{
	t_command *current = commands;
	int pipefd[2], in_fd = STDIN_FILENO;

	while (current != NULL)
	{
		if (current->command)
		{
			int out_fd = STDOUT_FILENO;
			if (current->next && current->next->separator) {
				if (strcmp(current->next->separator, "|") == 0) {
					if (pipe(pipefd) == -1) {
						perror("pipe");
						exit(EXIT_FAILURE);
					}
					out_fd = pipefd[1];
				}
				else {
					handle_redirection(current);  // handle other redirections
				}
			}

			execute_command(current->command, envp, in_fd, out_fd);
			if (out_fd != STDOUT_FILENO) {
				close(out_fd);
				in_fd = pipefd[0];
			}
		}
		current = current->next;
	}
}


void parser(char **envp, t_token *head)
{
	t_command *commands;
	char **path;

	path = find_path(envp);
	commands = build_command_list(head, path);

	tree(commands, envp);

	//print_command_list(commands);
	ft_free(path);
	free_t_token(head);
	free_t_command(commands);
}

int	main(int argc, char *argv[], char **envp)
{
    t_token	*head = NULL;
    char	*input;

    if(argc != 2)
    {
        printf("this process must have only one parameter\n");
        return (1);
    }
	input = argv[1];
	head = create_token_list(input, head);
	parser(envp, head);
	return (0);
}
