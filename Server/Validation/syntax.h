//
// Created by ejbm0110 on 11/12/24.
//

#ifndef SYNTAX_H
#define SYNTAX_H

char **split_string(const char *str, char delimiter, int *count);
void free_split_string(char **substrings, int count);
int validate_instruction(char* buffer);

#endif //SYNTAX_H
