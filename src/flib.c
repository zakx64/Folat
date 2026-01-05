#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "flib.h"

// Display an error message with context
void print_error(const char *message, int line_number, const char *line_content) {
	fprintf(stderr, "folat.%serror%s: %s\n", COLOR_RED, COLOR_RESET, message);
	if (line_number > 0 && line_content) {
		fprintf(stderr, "     %s%d%s | %s\n", COLOR_GRAY, line_number, COLOR_RESET, line_content);
	}
}

// Display a warning message with context
void print_warning(const char *message, int line_number, const char *line_content, bool show_warnings) {
	if (!show_warnings) return;
	
	fprintf(stderr, "folat.%swarn%s: %s\n", COLOR_YELLOW, COLOR_RESET, message);
	if (line_number > 0 && line_content) {
		fprintf(stderr, "     %s%d%s | %s\n", COLOR_GRAY, line_number, COLOR_RESET, line_content);
	}
}

// Convert a string to an integer safely
int string_to_integer(const char *data) {
	if (!data || !*data) return 0;
	
	// Skip leading whitespace
	while (*data == ' ' || *data == '\t') data++;
	
	if (!*data) return 0;
	
	int result = 0;
	int sign = 1;
	
	if (*data == '-') {
		sign = -1;
		data++;
	} else if (*data == '+') {
		data++;
	}
	
	// Convert digit by digit
	while (*data >= '0' && *data <= '9') {
		result = result * 10 + (*data - '0');
		data++;
	}
	
	if (*data && !isspace(*data)) {
		fprintf(stderr, "folat.%serror%s: expected an integer but got '%s'\n", COLOR_RED, COLOR_RESET, data);
		exit(1);
	}
	
	return result * sign;
}

// Set memory at target position with data
void ist_set(unsigned char *memory, int target, const char *data) {
	if (target >= 0 && target < MEMORY_SIZE) {
		int i;
		for (i = 0; i < 3 && data[i] != '\0'; i++) {
			if (target + i < MEMORY_SIZE) {
				// Convert pseudo-null (0x07/BEL) to actual null (0x00)
				if (data[i] == '\x07') {
					memory[target + i] = 0x00;
				} else {
					memory[target + i] = (unsigned char)data[i];
				}
			}
		}
		// Fill remaining positions with null if data is shorter than 3 chars
		while (i < 3 && target + i < MEMORY_SIZE) {
			memory[target + i] = 0x00;
			i++;
		}
	}
}

// Add value to memory at target position
void ist_add(unsigned char *memory, int target, int data) {
	if (target >= 0 && target < MEMORY_SIZE) {
		memory[target] = (unsigned char)((memory[target] + data) & 0xFF);
	}
}

// Subtract value from memory at target position
void ist_sub(unsigned char *memory, int target, int data) {
	if (target >= 0 && target < MEMORY_SIZE) {
		memory[target] = (unsigned char)((memory[target] - data) & 0xFF);
	}
}

// Compare memory at target position with data
bool ist_cmp(unsigned char *memory, int target, int data) {
	return (target >= 0 && target < MEMORY_SIZE) && (memory[target] == (unsigned char)(data & 0xFF));
}

// Exit instruction with optional memory dump
void ist_ext(unsigned char *memory, int target) {
	if (target == 1) {
		int last_position = -1;
		for (int i = MEMORY_SIZE - 1; i >= 0; i--) {
			if (memory[i] != 0) {
				last_position = i;
				break;
			}
		}
		
		if (last_position >= 0) {
			fwrite(memory, 1, last_position + 1, stdout);
		}
	}
	exit(0);
}