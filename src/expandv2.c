// #include "minishell.h"

// void expand_variables(t_cmd *cmd, t_env *env_list) {
//     // Helper function to find the value of an environment variable
//     char *find_env_value(const char *key, t_env *env_list) {
//         while (env_list) {
//             if (strcmp(key, env_list->key) == 0) {
//                 return env_list->value;
//             }
//             env_list = env_list->next;
//         }
//         return NULL;
//     }

//     // Expand environment variables in the command
//     if (cmd->cmd[0] == '$') {
//         // Get the variable name (skip the '$' character)
//         const char *var_name = cmd->cmd + 1;
//         char *var_value = find_env_value(var_name, env_list);
//         if (var_value != NULL) {
//             free(cmd->cmd);
//             cmd->cmd = strdup(var_value);
//             if (cmd->cmd == NULL) {
//                 perror("strdup failed");
//                 exit(EXIT_FAILURE);
//             }
//         } else {
//             fprintf(stderr, "Environment variable '%s' not found\n", var_name);
//         }
//     }

//     // Expand environment variables in command arguments
//     for (int i = 0; cmd->args[i] != NULL; i++) {
//         if (cmd->args[i][0] == '$') {
//             // Get the variable name (skip the '$' character)
//             const char *var_name = cmd->args[i] + 1;
//             char *var_value = find_env_value(var_name, env_list);
//             if (var_value != NULL) {
//                 free(cmd->args[i]);
//                 cmd->args[i] = strdup(var_value);
//                 if (cmd->args[i] == NULL) {
//                     perror("strdup failed");
//                     exit(EXIT_FAILURE);
//                 }
//             } else {
//                 fprintf(stderr, "Environment variable '%s' not found\n", var_name);
//             }
//         }
//     }
// }