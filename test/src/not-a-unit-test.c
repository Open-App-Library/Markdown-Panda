/*
 * This is not a unit test. :)
 * It's just a little 'sandbox'
 * for playing around with code.
 * It's easier to use GDB with than
 * criterion.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plugins.h>

int main() {
	char *table_stack =															\
		"# Here is a table\n\n"
		"| Name | Age | Description |\n"
		"| -- | -- | -- |\n"
		"| Joe | 19 | An average guy with a super power |\n\n"
		"# Here is another table:\n\n"
		"| Name | Age |\n"
		"| ted | 22 |\n\n";
	char *expectedResult_stack = \
		"# Here is a table\n\n"
		"| Name | Age |             Description            |\n"
		"| ---- | --- | ---------------------------------- |\n"
		"| Joe  | 19  | An average guy with a super power. |\n\n"
		"# Here is another table:\n\n"
		"| Name | Age |\n"
		"| ted  | 22  |\n\n";

	// Malloc new variables and copy the strings
	char *table = (char*) malloc(strlen(table_stack)+1);
	char *expectedResult = (char*) malloc(strlen(expectedResult_stack)+1);
	strcpy(table, table_stack);
	strcpy(expectedResult, expectedResult_stack);

	// Run Beautification code
	table = plugin_beautify_tables( table );

	printf("Expected:\n[TABLE]%s[/TABLE]\nActual:\n[TABLE]%s[/TABLE]", expectedResult, table);

	free(table);
	free(expectedResult);

	return 0;
}
