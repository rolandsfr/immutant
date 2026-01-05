#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Helpers
int line_starts_with(const char* line, const char* keyword)
{
	while (isspace(*line))
		line++;
	size_t len = strlen(keyword);
	return strncmp(line, keyword, len) == 0 &&
		   (isspace(line[len]) || line[len] == '(' || line[len] == '{');
}

int count_char(const char* line, char c)
{
	int count = 0;
	while (*line) {
		if (*line == c)
			count++;
		line++;
	}
	return count;
}

char* read_line(const char* prompt)
{
	static char buffer[256];
	printf("%s", prompt);
	if (!fgets(buffer, sizeof(buffer), stdin))
		return NULL;
	buffer[strcspn(buffer, "\n")] = '\0';
	return buffer;
}

// Main function
size_t buffer_block(char* first_line, char* out_buffer, size_t buffer_size)
{
	size_t len = strlen(first_line);
	if (len >= buffer_size)
		len = buffer_size - 1;
	strncpy(out_buffer, first_line, len);
	out_buffer[len] = '\0';

	// get current brace count
	int brace_count = count_char(first_line, '{') - count_char(first_line, '}');
	int braces_ever_matched = 0;
	int awaiting_else = 0;

	// Only buffer further if the line contains a block
	if (brace_count > 0 && line_starts_with(first_line, "if")) {
		awaiting_else = 1;
	}

	// Handle while blocks (no else needed)
	int is_while_block =
		line_starts_with(first_line, "while") && brace_count > 0;

	if (brace_count == 0) {
		return len;
	}

	while (brace_count > 0 || awaiting_else) {
		char* line = read_line("... ");
		if (!line)
			break;

		// trim
		char* p = line;
		while (*p && isspace(*p))
			p++;
		int only_whitespace = (*p == '\0');

		// If we just closed an if-block, allow else
		if (brace_count == 0 && awaiting_else &&
			line_starts_with(line, "else")) {

			// append
			out_buffer[len++] = '\n';
			strcpy(out_buffer + len, line);
			len += strlen(line);

			// now update brace count ONCE
			brace_count += count_char(line, '{') - count_char(line, '}');

			awaiting_else = 0;
			continue;
		}

		// stop if balanced and no else coming
		if (brace_count == 0 && only_whitespace) {
			awaiting_else = 0;
			break;
		}

		// append line
		out_buffer[len++] = '\n';
		strcpy(out_buffer + len, line);
		len += strlen(line);

		// update brace count
		brace_count += count_char(line, '{') - count_char(line, '}');
	}

	return len;
}
