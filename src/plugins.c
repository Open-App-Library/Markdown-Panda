#include "plugins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sds.h>
#include <string_arrays.h>
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

typedef struct {
	int  row_count;
	int  column_count;
	int* column_max_widths;
	MDStringList *cells;
} tableMetrics;

// Determines if a row in a row is a horizontal divider line
// ex. "| ---- | ---- |"
boolean tableRowIsDivider(sds rowStr, int startIndex) {
	char c = rowStr[startIndex];
	if (c == ' ') {
		return tableRowIsDivider(rowStr, startIndex+1);
	} else if (c == '-') {
		return True;
	} else {
		return False;
	}
}

tableMetrics getTableMetrics(sds tableStr)
{
	tableMetrics t;
	t.row_count = 0;
	t.column_count = 0;
	t.column_max_widths = malloc( 1*sizeof(int) );
	t.cells = mdstringlist_init();

	boolean editing_cell = False;
	sds currentCellText = sdsempty();
	int curColumn = -1;
	int curColumnWidth = 0;

	// Loop through table string
	for (int i = 0; i < sdslen(tableStr); i++) {
		char c = tableStr[i];

		// If editing a cell and you reach a newline or pipe character...
		// ...then set column in column_max_width to the current cell width
		//    if it is larger than the current value.
		if (editing_cell && (c == '\n' || c == '|')) {
			// Done with the cell. Process the results.
			int max_widths_len = sizeof(t.column_max_widths);

			if (max_widths_len < 0)
				puts("WARNING! max_widths_len >= sizeof(t.column_max_widths) -- This should never happen! :(");

			// Resize max col size array if needed
			if (sizeof(t.column_max_widths) < sizeof(int)*curColumn+1) {
				int *buff = realloc(t.column_max_widths, sizeof(int)*curColumn+1);
				if (buff) t.column_max_widths = buff;
				t.column_max_widths[curColumn] = 0;
			}

			// Adjust column count if needed
			if (curColumn+1 > t.column_count) {
				t.column_count = curColumn+1;
			}

			if (curColumnWidth > t.column_max_widths[curColumn]) {
				t.column_max_widths[curColumn] = curColumnWidth;
			}
		}

		// Process cell text
		if ((c == '\n' || c == '|') && sdslen(currentCellText)) {
			if (editing_cell) {
				int row=t.row_count,col=curColumn;
				mdstringlist_alter(t.cells, row, col, sdsdup(currentCellText));
			}
			sdsfree(currentCellText);
			currentCellText = sdsempty();
		}

		// If current char is a newline character or on the last char in the string...
		// ...Set editing_cell to false, increment row count, reset curColumn, reset curColumnWidth
		if (c == '\n' || i+1 >= sdslen(tableStr)) {
			editing_cell = False;
			t.row_count++;
			curColumn = -1;
			curColumnWidth = 0;
		}
		// If current char is a pipe character and the next char is not a newline...
		// ...Set editing_cell to true, incremenet curColumn, reset curColumnWidth.
		else if (c == '|' && i+1 < sdslen(tableStr) && tableStr[i+1] != '\n') {
			editing_cell = True;
			curColumn++;
			curColumnWidth = 0;
		}
		// Otherwise, if editing_cell...
		// ...increment the curColumnWidth.
		else if(editing_cell && c != '|' && c != '\n') {
			char char_to_string[] = {c};
			sds char_string = sdsnewlen(char_to_string, 1);
			currentCellText = sdscat(currentCellText, char_string);
			sdsfree(char_string);
			curColumnWidth++;
		}

	}
	return t;
}

void freeTableMetrics(tableMetrics t)
{
	free(t.column_max_widths);
	mdstringlist_free( t.cells );
}

sds process_table(sds tableStr)
{
	sds newstr = sdsnew("");

	tableMetrics t = getTableMetrics(tableStr);
	MDStringList *cells = t.cells;

	for (int y = 0; y < cells->count; y++) {

		StringList *col = cells->lists[y];
		for (int x = 0; x < col->count; x++) {
			sds text = sdsdup(col->strings[x]);
			int ts, ps=0, as=0; // count for total spaces, appendSpaces and prependSpaces
			sds appendSpaces = sdsempty();
			sds prependSpaces = sdsempty();

			boolean isDivider = False;
			if ( tableRowIsDivider(text, 0) )
				isDivider = True;

			if (!isDivider) {
				ts = t.column_max_widths[x] - sdslen(col->strings[x]);
				ps = ts/2;
				as = ts/2 + ts%2;

				for (int i = 0; i < as; i++) {
					appendSpaces = sdscat(appendSpaces, " ");
					if (i < ps)
						prependSpaces = sdscat(prependSpaces, " ");
				}
			} else {
				sdsfree(text);
				text = sdsempty();
				appendSpaces = sdscat(appendSpaces, " ");
				prependSpaces = sdscat(prependSpaces, " ");
				for (int i = 0; i < t.column_max_widths[x]-2; i++)
					text = sdscat(text, "-");
			}


			if (x == 0)
				newstr = sdscat(newstr, "|");
			newstr = sdscatfmt(newstr, "%s%s%s|", prependSpaces, text, appendSpaces);
			sdsfree(appendSpaces);
			sdsfree(prependSpaces);
			sdsfree(text);
		}

		if (y != cells->count-1)
			newstr = sdscat(newstr, "\n");
	}

	freeTableMetrics(t);
	/* sdsfree(tableStr); */
	return newstr;
}

void plugin_beautify_tables(char *str)
{
	int number_of_tables_processed = 0;

	sds newStr = sdsnew("");
	sds curTable = sdsnew("");

	boolean first_char_of_line = True;
	boolean editing_table = False;
	boolean was_editing_table = False;

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

		// If not editing table and was editing table or on last char and currently editing table
		// Process the completed table
		if ( (!editing_table && was_editing_table) || (i == strlen(str)-1 && editing_table) ) {
			sds processed_table = process_table(curTable);
			newStr = sdscat(newStr, processed_table);
			sdsfree(curTable);
			sdsfree(processed_table);
			curTable = sdsempty();
		}

		char cur_char[] = {c};
 		sds char_string = sdsnewlen(cur_char, 1);
		if (editing_table) {
			curTable = sdscat(curTable, char_string);
		} else {
			newStr = sdscat(newStr, char_string);
		}

		//// END: THE MAIN LOGIC

		// Save the current state of editing_table
		was_editing_table = editing_table;
	}
	if ( strlen(str) < strlen(newStr)) {
		str = (char*)realloc(str, strlen(newStr));
	}
	strcpy(str, newStr);
	free(newStr);
}
