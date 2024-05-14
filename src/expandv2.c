#include "minishell.h"


static char *find_env_value(const char *key, t_env *env_list) {
    while (env_list)
    {
        if (strcmp(key, env_list->key) == 0)
            return env_list->value;
        env_list = env_list->next;
    }
   return NULL;
}

void expand_variables(t_cmd *cmd, t_env *env_list, int *flag)
{
    // Helper function to find the value of an environment variable

    // Expand environment variables in the command
    if (cmd->cmd[0] == '$')
    {
        // Get the variable name (skip the '$' character)
        const char *var_name = cmd->cmd + 1;
        char *var_value = find_env_value(var_name, env_list);
        if (var_value != NULL)
        {
            free(cmd->cmd);
            cmd->cmd = strdup(var_value);
            if (cmd->cmd == NULL)
            {
                perror("strdup failed");
                exit(EXIT_FAILURE);
            }
        }
        else
            *flag = 0;
    }

    // Expand environment variables in command arguments
    // for (int i = 0; cmd->args[i] != NULL; i++) {
    //     if (cmd->args[i][0] == '$') {
    //         // Get the variable name (skip the '$' character)
    //         const char *var_name = cmd->args[i] + 1;
    //         char *var_value = find_env_value(var_name, env_list);
    //         if (var_value != NULL) {
    //             free(cmd->args[i]);
    //             cmd->args[i] = strdup(var_value);
    //             if (cmd->args[i] == NULL) {
    //                 perror("strdup failed");
    //                 exit(EXIT_FAILURE);
    //             }
    //         } else {
    //             fprintf(stderr, "Environment variable '%s' not found\n", var_name);
    //         }
    //     }
    // }
}





// Function to expand a single token if it starts with '$'
char *expand_token(const char *token, t_env *env_list) {
    if (token[0] == '$') {
        // Get the variable name (skip the '$' character)
        const char *var_name = token + 1;
        // Search for the variable in the environment
        while (env_list != NULL) {
            if (strcmp(var_name, env_list->key) == 0) {
                // Found a match, expand the variable
                return strdup(env_list->value);
            }
            env_list = env_list->next;
        }
        // Variable not found, print a warning
        fprintf(stderr, "Environment variable '%s' not found\n", var_name);
        return NULL; // Return original token if variable not found
    } else {
        // Not a variable, return original token
        return strdup(token);
    }
}

// Function to expand environment variables in an array of arguments
char **expand_args(char **args, t_env *env_list)
{
    // Count the number of arguments
    int count = 0;
    while (args[count] != NULL)
        count++;
    // Allocate memory for the expanded arguments array
    char **expanded_args = (char **)malloc((count + 1) * sizeof(char *));
    if (expanded_args == NULL)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    // Expand each argument and store in the expanded_args array
    for (int i = 0; i < count; i++)
        expanded_args[i] = expand_token(args[i], env_list);
    expanded_args[count] = NULL; // Null-terminate the array

    return expanded_args;
}
