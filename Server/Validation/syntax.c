#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax.h"

char* valid_figures[] = {"sphere", "cube", "cylinder", "cone", "pyramid", "prism"};

const char* errorMessages[] = {
    "\033[1;31mInvalid figure. Options are:\n\nsphere\ncube\ncylinder\ncone\npyramid\nprism\033[0m\n",
    "\033[1;31mNo params were specified\033[0m\n",
    "\033[1;31mParam does not start with '-'\033[0m\n",
    "\033[1;31mNo specified value\033[0m\n",
    "takes 1 parameter\033[0m\n",
    "takes 2 parameters\033[0m\n",
    "takes 3 parameters\033[0m\n",
    "\033[1;31mParam value should be between 0.1 and 6.0\033[0m\n",
    "\033[1;31mParam value should be a float type value\033[0m\n",
    "\033[1;31msphere only admits\n\nradius\n\n\033[0m\n",
    "\033[1;31mcube only admits\n\nside\n\n\033[0m\n",
    "\033[1;31mpyramid only admits\n\nheight\n\n\033[0m\n",
    "only admits\n\nradius\nheight\n\n\033[0m\n",
    "\033[1;31mPrism only admits\n\nradius\nlength\nsides\n\n\033[0m\n"
};

// Function to split a string by a given delimiter
char **split_string(const char *str, char delimiter, int *count) {
    char **result = NULL;
    int substr_count = 0;
    const char *start = str;
    const char *end;

    while ((end = strchr(start, delimiter)) != NULL) {
        // Calculate the length of the substring
        int len = end - start;

        // Allocate memory for the substring and copy it
        result = realloc(result, sizeof(char *) * (substr_count + 1));
        result[substr_count] = malloc(len + 1);
        strncpy(result[substr_count], start, len);
        result[substr_count][len] = '\0';

        substr_count++;
        start = end + 1;
    }

    // Handle the last substring (or the entire string if no delimiter is found)
    result = realloc(result, sizeof(char *) * (substr_count + 1));
    result[substr_count] = malloc(strlen(start) + 1);
    strcpy(result[substr_count], start);
    substr_count++;

    *count = substr_count;
    return result;
}

// Function to free the array of substrings
void free_split_string(char **substrings, int count) {
    for (int i = 0; i < count; i++) {
        free(substrings[i]);
    }
    free(substrings);
}

int validate_param_value(char* cmd, char* buff_tok) {
    char* key = strtok(buff_tok, "=");
    char* value = strtok(NULL, "=");

    if (value == NULL) {
        printf("%s", errorMessages[3]);
        return 0;
    }

    double converted_value;
    if ((converted_value = atof(value))) {
        if (converted_value < 0.1 || converted_value > 6.0) {
            printf("%s", errorMessages[7]);
            return 0;
        }
    } else {
        printf("%s", errorMessages[8]);
        return 0;
    }

    // Sphere can only receive radius
    if (strcmp(cmd, "sphere") == 0 && strcmp(key, "-radius") != 0) {
        printf("%s", errorMessages[9]);
        return 0;
    }
    if (strcmp(cmd, "cube") == 0 && strcmp(key, "-side") != 0) {
        printf("%s", errorMessages[10]);
        return 0;
    }
    if (strcmp(cmd, "pyramid") == 0 && strcmp(key, "-height") != 0) {
        printf("%s", errorMessages[11]);
        return 0;
    }
    if ((strcmp(cmd, "cylinder") == 0 || strcmp(cmd, "cone") == 0) && (strcmp(key, "-radius") != 0 && strcmp(key, "-height") != 0)) {
        printf("\033[1;31m%s %s", cmd, errorMessages[12]);
        return 0;
    }
    if (strcmp(cmd, "prism") == 0 && (strcmp(key, "-radius") != 0 && strcmp(key, "-length") != 0 && strcmp(key, "-sides") != 0)) {
        printf("%s", errorMessages[13]);
        return 0;
    }
    return 1;
}

int validate_params(char* buffer) {
    int count;
    char **parameters = split_string(buffer, ' ', &count);

    int validFlag = 0;
    for (int i = 0; i < 6; i++) {
        if (strncmp(parameters[0], valid_figures[i], strlen(valid_figures[i])) == 0) {
            validFlag = 1;
            break;
        }
    }
    if (!validFlag) {
        printf("%s", errorMessages[0]);
        return 0;
    }

    // Verify if no parameters were given
    if (parameters[1] == NULL) {
        printf("%s", errorMessages[1]);
        return 0;
    }

    for (int i = 1; i < count; i++) {
        // Verify if param starts with "-"
        if (strncmp(parameters[i], "-", 1) != 0) {
            printf("%s", errorMessages[2]);
            return 0;
        }

        if (!validate_param_value(parameters[0], parameters[i])) {
            return 0;
        }
    }

    // Verify number of params greater than 1 for sphere, cube and pyramid
    if ((strcmp(parameters[0], "sphere") == 0 || strcmp(parameters[0], "cube") == 0 || strcmp(parameters[0], "pyramid") == 0) && count - 1 != 1) {
        printf("\033[1;31m%s %s", parameters[0], errorMessages[4]);
        return 0;
    }

    // Verify numbers of params for cylinder and cone
    if ((strcmp(parameters[0], "cylinder") == 0 || strcmp(parameters[0], "cone") == 0) && count - 1 != 2) {
        printf("\033[1;31m%s %s", parameters[0], errorMessages[5]);
        return 0;
    }

    // Verify number of params for prism
    if (strcmp(parameters[0], "prism") == 0 && count - 1 != 3) {
        printf("\033[1;31m%s %s", parameters[0], errorMessages[6]);
        return 0;
    }

    free_split_string(parameters, count);

    return 1;
}

int validate_instruction(char* buffer) {
    int count, validFlag = 0;
    char **instructions = split_string(buffer, '&', &count);

    for (int i = 0; i < count; i++) {
      if (validate_params(instructions[i])) validFlag = 1;
      else validFlag = 0;
    }

    free_split_string(instructions, count);

    return validFlag;
}