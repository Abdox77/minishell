#include "minishell.h"

static void	handle_single_quotes(const char **str, int *in_single_quotes)
{
	*in_single_quotes = !(*in_single_quotes);
	(*str)++;
}

static void	handle_double_quotes(const char **str, int *in_double_quotes)
{
	*in_double_quotes = !(*in_double_quotes);
	(*str)++;
}

void	dollar_sign2(char **str, char **res_ptr, t_env *env, int *in_s_quotes)
{
	(*str)++;
	expand_variable((const char **)str, res_ptr, env, in_s_quotes);
}

static void	expand_char(char **result_ptr, const char **str)
{
	**result_ptr = **str;
	(*result_ptr)++;
	(*str)++;
}

char	*expand_string(const char *str, t_env *env)
{
	size_t	result_size;
	char	*result;
	char	*result_ptr;
	int		in_single_quotes;
	int		in_double_quotes;

	in_single_quotes = 0;
	in_double_quotes = 0;
	result_size = get_expanded_length(str, env);
	result = malloc_with_error(result_size);
	result_ptr = result;
	while (*str)
	{
		if (*str == '\'' && !in_double_quotes)
			handle_single_quotes(&str, &in_single_quotes);
		else if (*str == '"' && !in_single_quotes)
			handle_double_quotes(&str, &in_double_quotes);
		else if (*str == '$' && !in_single_quotes)
			dollar_sign2((char **)&str, &result_ptr, env, &in_single_quotes);
		else
			expand_char(&result_ptr, &str);
	}
	*result_ptr = '\0';
	return (result);
}
