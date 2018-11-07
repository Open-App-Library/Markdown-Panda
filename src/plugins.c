#include "plugins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

void plugin_ensure_newline(char *str)
{
  for (int i = strlen(str)-1; i > 0; i--) {
    if ( str[i] != '\n' ) {
      if ( i+2 >= strlen(str)-1 )
      	if ( !realloc(str, i+2) )
      	  puts("Error running realloc in markdownpanda.c in mdpanda_to_markdown CASE 1");
      str[i+1] = '\n';
      str[i+2] = '\0';
      break;
    }
  }
}

void plugin_beautify_tables(char *str)
{
	int number_of_tables_processed = 0;

	char *newStr = malloc(1); newStr[0] = '\0'; // This will be the processed string

	boolean first_char_of_line = True;
	boolean editing_table = False;
	boolean was_editing_table = False;
	char *curTable = malloc(1); curTable[0] = '\0';
	for (int i = 0; i < strlen(str); i++) {
		char c = str[i];

		// Determine if first char of a line
		if ( i == 0 || str[i-1] == '\n')
			first_char_of_line = True;
		else
			first_char_of_line = False;

		// If editing_table AND current char is a newline AND (is last char in string or next char is a newline)
		if ( editing_table &&
				 c == '\n'     &&
				 (i+1>=strlen(str) || str[i+1] == '\n') ) {
			editing_table = False;
			number_of_tables_processed++;
		}

		// If current char is a pipe character and on the first char of line and not currently editing table
		if (c == '|' && first_char_of_line)
			editing_table = True;

		//// START: THE MAIN LOGIC
		// Process the current char

		if (editing_table) {
			if (c == '\n')
				curTable = string_append2(curTable, "\\N\n");
			else
				curTable = string_appendc(curTable, c);
		} else {
			if (c == '\n')
				newStr = string_append2(newStr, "\\n\n");
			else
				newStr = string_appendc(newStr, c);
		}

		// If not editing table and was editing table or on last char and currently editing table
		// Process the completed table
		if ( (!editing_table && was_editing_table) || (i == strlen(str)-1 && editing_table) ) {
			newStr = string_append2(newStr, curTable);
			free(curTable);
			curTable = malloc(1); curTable[0] = '\0';
		}
		//// END: THE MAIN LOGIC

		// Save the current state of editing_table
		was_editing_table = editing_table;
	}
	printf("New string\n%s", newStr);
}
