#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "flib.h"

#define MINIMUM_LINE_LENGTH 11
#define MAXIMUM_LINE_LENGTH 150

// Compilation commands for optimzation:
// clang -O3 -mtune=native -funroll-loops -fomit-frame-pointer main.c flib.c -o fol
// strip --strip-all fol

// Skip whitespace characters in a string
static inline char* skip_whitespace(char *pointer) {
	while (*pointer == ' ' || *pointer == '\t') pointer++;
	return pointer;
}

// Check if two instructions match exactly
static inline int instructions_match(const char *first, const char *second) {
	return (first[0] == second[0]) && (first[1] == second[1]) && (first[2] == second[2]);
}

// Fast integer conversion for numeric strings
static inline int parse_integer(const char *text) {
	int result = 0;
	while (*text >= '0' && *text <= '9') {
		result = result * 10 + (*text - '0');
		text++;
	}
	return result;
}

// Convert escape sequences in data field
static inline void convert_escape_sequences(char *destination, const char *source, int max_length) {
	int j = 0;
	for (int i = 0; source[i] && j < max_length - 1; i++) {
		if (source[i] == '\\') {
			destination[j++] = '\n';
		} else if (source[i] == '-') {
			destination[j++] = '\x07';
		} else {
			destination[j++] = source[i];
		}
	}
	destination[j] = '\0';
}

// Check if a string contains only whitespace
static inline bool contains_only_whitespace(const char *text) {
	if (!text || !*text) return true;
	while (*text) {
		if (*text != ' ' && *text != '\t') return false;
		text++;
	}
	return true;
}

// Check if a string is numeric
static inline bool is_numeric(const char *text) {
	if (!text || !*text) return false;
	while (*text) {
		if (*text < '0' || *text > '9') return false;
		text++;
	}
	return true;
}

// Count the number of digits in a number
static inline int count_digits(int number) {
	if (number == 0) return 1;
	int count = 0;
	while (number > 0) {
		count++;
		number /= 10;
	}
	return count;
}

void print_usage(const char *program_name) {
	printf("Usage: %s [options] <file>\n", program_name);
	printf("\nOptions:\n");
	printf("  --no-warn, -W    Suppress warning messages\n");
	printf("  --help, -h       Show this help message\n");
}

int main(int argc, char *argv[]) {
	unsigned char memory[MEMORY_SIZE] = {0};
	bool flag = false;
	long point_position = -1;
	bool show_warnings = true;
	char *filename = NULL;
	
	// Parse command line arguments
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--no-warn") == 0 || strcmp(argv[i], "-W") == 0) {
			show_warnings = false;
		} else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
			print_usage(argv[0]);
			return 0;
		} else if (argv[i][0] == '-') {
			fprintf(stderr, "folat.%serror%s: unknown option '%s'\n", COLOR_RED, COLOR_RESET, argv[i]);
			fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
			return 1;
		} else {
			filename = argv[i];
		}
	}
	
	if (!filename) {
		fprintf(stderr, "folat.%serror%s: no input file specified\n", COLOR_RED, COLOR_RESET);
		print_usage(argv[0]);
		return 1;
	}
	
	FILE *file = fopen(filename, "rb");
	if (!file) {
		fprintf(stderr, "folat.%serror%s: file '%s' not found\n", COLOR_RED, COLOR_RESET, filename);
		return 1;
	}
	
	char line[MAXIMUM_LINE_LENGTH + 2];
	char original_line[MAXIMUM_LINE_LENGTH + 2];
	int line_number = 0;
	
	while (fgets(line, sizeof(line), file)) {
		line_number++;
		strcpy(original_line, line);
		
		int length = strlen(line);
		
		// Remove trailing newline characters
		if (length > 0 && (line[length-1] == '\n' || line[length-1] == '\r')) {
			line[--length] = '\0';
			if (length > 0 && line[length-1] == '\r') line[--length] = '\0';
		}
		
		// Skip empty lines
		if (length == 0) continue;
		
		// Validate line length
		if (length < MINIMUM_LINE_LENGTH) {
			char message[256];
			snprintf(message, sizeof(message), "line too short (got %d chars, expected at least %d)", length, MINIMUM_LINE_LENGTH);
			print_error(message, line_number, original_line);
			fclose(file);
			return 1;
		}
		
		if (length > MAXIMUM_LINE_LENGTH) {
			char message[256];
			snprintf(message, sizeof(message), "line too long (got %d chars, maximum is %d)", length, MAXIMUM_LINE_LENGTH);
			print_error(message, line_number, original_line);
			fclose(file);
			return 1;
		}
		
		char *pointer = skip_whitespace(line);
		
		// Skip comment lines
		if (*pointer == ';') continue;
		
		long current_position = ftell(file);
		char *instruction = pointer;
		
		// Find the end of the instruction
		char *space_after_instruction = instruction;
		while (*space_after_instruction && *space_after_instruction != ' ' && *space_after_instruction != '\t') 
			space_after_instruction++;
		if (*space_after_instruction) *space_after_instruction++ = '\0';
		
		// Validate instruction length
		if (strlen(instruction) != 3) {
			char message[256];
			snprintf(message, sizeof(message), "instruction must be exactly 3 characters (got '%s')", instruction);
			print_error(message, line_number, original_line);
			fclose(file);
			return 1;
		}
		
		// Handle point markers
		if (instructions_match(instruction, "---")) {
			point_position = current_position;
			continue;
		}
		
		// Check if instruction requires arguments
		bool requires_arguments = !instructions_match(instruction, "gpt") && !instructions_match(instruction, "gpf");
		
		char *target_string = "000";
		char *data_string = "";
		int target = 0;
		
		if (requires_arguments) {
			// Skip whitespace before target field
			space_after_instruction = skip_whitespace(space_after_instruction);
			target_string = space_after_instruction;
			
			// Find end of target field
			char *space_after_target = target_string;
			while (*space_after_target && *space_after_target != ' ' && *space_after_target != '\t') 
				space_after_target++;
			if (*space_after_target) *space_after_target++ = '\0';
			
			// Validate target field length
			if (strlen(target_string) != 3) {
				char message[256];
				snprintf(message, sizeof(message), "target field must be exactly 3 characters (got '%s')", target_string);
				print_error(message, line_number, original_line);
				fclose(file);
				return 1;
			}
			
			// Skip whitespace before data field
			space_after_target = skip_whitespace(space_after_target);
			data_string = space_after_target;
			
			bool target_is_numeric = is_numeric(target_string);
			
			if (!target_is_numeric) {
				char message[256];
				snprintf(message, sizeof(message), "target field '%s' is not numeric, will be converted to 0", target_string);
				print_warning(message, line_number, original_line, show_warnings);
			}
			
			// Convert target to integer
			target = parse_integer(target_string);
			
			if (target >= MEMORY_SIZE) {
				char message[256];
				snprintf(message, sizeof(message), "target %d exceeds memory size (%d), operation will be ignored", target, MEMORY_SIZE);
				print_warning(message, line_number, original_line, show_warnings);
			}
			
			if (target_is_numeric && count_digits(target) < 3 && target_string[0] != '0') {
				char message[256];
				snprintf(message, sizeof(message), "target should have leading zeros (use '%03d' instead of '%s')", target, target_string);
				print_warning(message, line_number, original_line, show_warnings);
			}
		}
		
		// Process escape sequences in data field
		char processed_data[256];
		convert_escape_sequences(processed_data, data_string, sizeof(processed_data));
		
		// Execute instructions
		if (instructions_match(instruction, "add")) {
			if (strlen(processed_data) == 0 || contains_only_whitespace(processed_data)) {
				print_warning("data field is empty, will add 0", line_number, original_line, show_warnings);
			}
			
			int data_value = parse_integer(processed_data);
			
			if (target >= MEMORY_SIZE) {
				char message[256];
				snprintf(message, sizeof(message), "cannot add to memory position %d (valid range: 0-%d)", target, MEMORY_SIZE - 1);
				print_error(message, line_number, original_line);
				fclose(file);
				return 1;
			}
			
			ist_add(memory, target, data_value);
		} 
		else if (instructions_match(instruction, "sub")) {
			if (strlen(processed_data) == 0 || contains_only_whitespace(processed_data)) {
				print_warning("data field is empty, will subtract 0", line_number, original_line, show_warnings);
			}
			
			int data_value = parse_integer(processed_data);
			
			if (target >= MEMORY_SIZE) {
				char message[256];
				snprintf(message, sizeof(message), "cannot subtract from memory position %d (valid range: 0-%d)", target, MEMORY_SIZE - 1);
				print_error(message, line_number, original_line);
				fclose(file);
				return 1;
			}
			
			ist_sub(memory, target, data_value);
		} 
		else if (instructions_match(instruction, "set")) {
			if (strlen(processed_data) == 0) {
				print_warning("data field is empty, setting null bytes", line_number, original_line, show_warnings);
			}
			
			if (target >= MEMORY_SIZE) {
				char message[256];
				snprintf(message, sizeof(message), "cannot set memory position %d (valid range: 0-%d)", target, MEMORY_SIZE - 1);
				print_error(message, line_number, original_line);
				fclose(file);
				return 1;
			}
			
			if (target > MEMORY_SIZE - 3) {
				char message[256];
				snprintf(message, sizeof(message), "setting at position %d may overflow memory boundary (size: %d)", target, MEMORY_SIZE);
				print_warning(message, line_number, original_line, show_warnings);
			}
			
			ist_set(memory, target, processed_data);
		} 
		else if (instructions_match(instruction, "cmp")) {
			if (target >= MEMORY_SIZE) {
				char message[256];
				snprintf(message, sizeof(message), "cannot compare memory position %d (valid range: 0-%d)", target, MEMORY_SIZE - 1);
				print_error(message, line_number, original_line);
				fclose(file);
				return 1;
			}
			
			// Handle both ASCII characters and numeric values
			int data_value;
			if (strlen(processed_data) > 0 && !is_numeric(processed_data)) {
				// Use first character as ASCII value
				data_value = (unsigned char)processed_data[0];
			} else {
				// Parse as integer
				data_value = parse_integer(processed_data);
			}
			
			flag = ist_cmp(memory, target, data_value);
		} 
		else if (instructions_match(instruction, "out")) {
			if (target >= MEMORY_SIZE) {
				char message[256];
				snprintf(message, sizeof(message), "cannot output memory position %d (valid range: 0-%d)", target, MEMORY_SIZE - 1);
				print_error(message, line_number, original_line);
				fclose(file);
				return 1;
			}
			
			if (strlen(data_string) > 0 && !contains_only_whitespace(data_string)) {
				print_warning("'out' instruction ignores data field", line_number, original_line, show_warnings);
			}
			
			putchar(memory[target]);
			fflush(stdout);
		} 
		else if (instructions_match(instruction, "ext")) {
			if (strlen(data_string) > 0 && !contains_only_whitespace(data_string)) {
				print_warning("'ext' instruction ignores data field", line_number, original_line, show_warnings);
			}
			
			ist_ext(memory, target);
		} 
		else if (instructions_match(instruction, "gpt")) {
			if (flag) {
				if (point_position == -1) {
					print_error("no point recorded, cannot jump", line_number, original_line);
					fclose(file);
					return 1;
				}
				fseek(file, point_position, SEEK_SET);
			}
		} 
		else if (instructions_match(instruction, "gpf")) {
			if (!flag) {
				if (point_position == -1) {
					print_error("no point recorded, cannot jump", line_number, original_line);
					fclose(file);
					return 1;
				}
				fseek(file, point_position, SEEK_SET);
			}
		} 
		else {
			char message[256];
			snprintf(message, sizeof(message), "unknown instruction '%s'", instruction);
			print_error(message, line_number, original_line);
			fclose(file);
			return 1;
		}
	}
	
	fclose(file);
	return 0;
}