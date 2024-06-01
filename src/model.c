#include <stdlib.h>
#include <string.h>
#include "model.h"

char* process_data(const char *input) {
    const char *response_prefix = "Processed: ";
    char *response = (char *)malloc(strlen(response_prefix) + strlen(input) + 1);
    strcpy(response, response_prefix);
    strcat(response, input);
    return response;
}
