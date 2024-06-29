#include "minishell.h"

void reset_error_message(t_bool *is_saved, t_bool *is_printed, t_bool *err_here_doc, char **message)
{
    *is_saved = FALSE;
    *is_printed = FALSE;
    *err_here_doc = FALSE;
    free(*message);
    *message = NULL;
}

t_bool ft_print_error(char *message, char **line, t_error indicator)
{
    static t_bool   is_saved;
    static t_bool   is_printed;
    static t_bool err_here_doc;
    static char     *error_message;

    if (indicator == RESET)
        reset_error_message(&is_saved, &is_printed, &err_here_doc, &message);
    else if (indicator == RESET_HEREDOC)
    {
        err_here_doc = TRUE;
        is_saved = TRUE;
    }
    else if (indicator == SAVE && is_saved == FALSE)
    {
        if (line && *line)
            (*line) += ft_strlen(*line); 
        is_saved = TRUE;
        error_message = ft_strdup(message);
    }
    else if (indicator == RETRIEVE)
        return (is_saved);
    else if (indicator == PRINT && is_printed == FALSE && err_here_doc == FALSE)
    {
        if (error_message != NULL)
        { 
            is_printed = TRUE;
            write(STDERR_FILENO, error_message, ft_strlen(error_message));
        }
    }
    return TRUE;
}