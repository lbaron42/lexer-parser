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
	int i;

	i = 0;
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

void free_list(t_token *head)
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
		set_token(node, "Redirect_in", 4);
	else if (ft_strcmp("<", node->value) == 0)
		set_token(node, "Redirect_out", 5);
	else if (node->value[0] == '-')
		set_token(node, "Flag", 3);
	else if (ft_strcmp("<<", node->value) == 0)
		set_token(node, "Here_doc", 6);
	else if (ft_strcmp(">>", node->value) == 0)
		set_token(node, "Append_doc", 7);
	else
		set_token(node, "Argument", 1);
}

void	is_keyword(t_token *node)
{
	int		i;
	char	*keywords[] = {
			"cd", "echo", "env", "exit", "export", "pwd", "unset",
			"grep", "wc", "mkdir", "mv", "cp", "rm", "rmdir",
			"touch","find", "head", "tail", "diff", "find",
			"cat", "ls", NULL};

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

char	**build_command_array(t_token *head)
{
	int		i;
	t_token	*tmp;
	char	**command_array;

	// count the number of tokens in the list
	i = 0;
	tmp = head;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}

	// allocate memory for the command array
	command_array = (char **)malloc(sizeof(char *) * (i + 1)); // +1 for NULL terminator
	if (!command_array)
		return (NULL); // handle memory allocation failure

	// populate the command array with values from the list
	i = 0;
	tmp = head;
	while (tmp)
	{
		command_array[i] = tmp->value; // assign value
		tmp = tmp->next;
		i++;
	}
	command_array[i] = NULL; // NULL terminate the array

	return (command_array);
}

void	pipe_between_commands(char **cmd1, char **cmd2)
{
	int		pfd[2];
	pid_t	pid;

	pipe(pfd);
	pid = fork();
	if (pid == 0)
	{
		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		execve(cmd1[0], cmd1, NULL);  // replace with appropriate path
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		close(pfd[1]);
		dup2(pfd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
		execve(cmd2[0], cmd2, NULL);  // replace with appropriate path
	}
}

void	redirect_stdout_to_file(char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)
		return ;  // handle error
	if (dup2(fd, STDOUT_FILENO) == -1)
		return ;  // handle error
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
		return ;  // handle error
	if (dup2(fd, STDOUT_FILENO) == -1)
		return ;  // handle error
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

void parser(char **envp, t_token *head)
{
	char **path;
	path = find_path(envp);
	print_list(head);


	free_list(head);
	ft_free(path);
}

int main(int argc, char *argv[], char **envp)
{
	t_token *head = NULL;
	char *input;

	input = argv[1];
	head = create_token_list(input, head);
	parser(envp, head);

	return (0);
}

