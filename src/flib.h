#ifndef FLIB_H
#define FLIB_H

#include <stdbool.h>

#define MEMORY_SIZE 50

#define COLOR_RED     "\033[91m"
#define COLOR_YELLOW  "\033[93m"
#define COLOR_BLUE    "\033[94m"
#define COLOR_GRAY    "\033[90m"
#define COLOR_RESET   "\033[0m"

int string_to_integer(const char *data);
void ist_set(unsigned char *memory, int target, const char *data);
void ist_add(unsigned char *memory, int target, int data);
void ist_sub(unsigned char *memory, int target, int data);
bool ist_cmp(unsigned char *memory, int target, int data);
void ist_ext(unsigned char *memory, int target);

void print_error(const char *message, int line_number, const char *line_content);
void print_warning(const char *message, int line_number, const char *line_content, bool show_warnings);

#endif