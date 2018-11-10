/*
 * The entry-point file for unit testing.
 */

#include <criterion/criterion.h>
#include <stdio.h>
#include <markdownpanda.h>
#include "test-helpers.h"
#include "printdiff.h"
#include "plugins.h"
#include <sds.h>
#include <string_arrays.h>

char *format_types[] = {"basic-formatting",
			"code",
			"headings",
			"img",
			"link",
			"lists",
			"paragraph",
			"tables"};
int format_count = 8;


/*
 * HTML to Markdown:
 * This test will run through all of the format_types,
 * load the corresponding md/html files in the
 * test/{html,md} folder, and test to see if the
 * HTML->Markdown conversion string matches the
 * corresponding markdown file.
 */
Test(formatting, html_to_markdown) {
  for (int i = 0; i < format_count; i++) {
    char *type = format_types[i];
    text_t t = load(type);
    char *md = mdpanda_to_markdown( t.htmlObject );
    if ( ! string_equals( t.md, md) ) {
      cr_log_error("\n html_to_markdown '%s' FAILED \n\n", type);

		  char *f = "\n%s:\n<!--Start-->\n%s\n<!--End-->\n";
			printf(f, "Expected", t.md);
			printf(f, "Actual", md);
      /* printdiff("Expected", "mdpanda ", t.md, md); */
      cr_assert(0, "Formatting test failed.");
    }
    cr_assert(1);
  }
}


/*
 * Markdown to HTML
 * This test will run through all of the format_types,
 * load the corresponding md/html files in the
 * test/{html,md} folder, and test to see if the
 * Markdown->HTML conversion string matches the
 * corresponding HTML file.
 */
Test(formatting, markdown_to_html) {
  for (int i = 0; i < format_count; i++) {
    char *type = format_types[i];
    text_t t = load(type);
    char *html = mdpanda_to_html( t.md );
    boolean isSuccess = true;

    if ( !string_equals(t.html, html) ) {
      cr_log_error("\n markdown_to_html '%s' FAILED \n\n", type);
      printdiff("Expected", "CMark   ", t.html, html);
      isSuccess = false;
    }

    free(t.html);
    free(t.md);
    free(html);
    cr_assert(isSuccess);
  }
}


/*
 * Testing ensure_newline in helper.h
 */
Test(plugins, ensure_newlines) {
  char *expected = "Hello\n"; // Expected output after running ensure_newlines on each test string.

  size_t tests_length = 4;
  char *tests[ tests_length ];

  tests[0] = string_new("Hello\n\n");
  tests[1] = string_new("Hello\n\n\n");
  tests[2] = string_new("Hello\n\n\n\n\n\n\n");
  tests[3] = string_new("Hello");

  for (int i=0; i < tests_length; i++) {
    tests[i] = plugin_ensure_newline( tests[i] );
    if ( !string_equals(tests[i], expected)) {
      cr_log_error("tests[%i] failed", i);
      cr_assert(0);
    }
    free( tests[i] );
  }

  cr_assert(1);
}


Test(plugins, beautify_tables) {
	char *table_stack = \
		"# Here is a table\n\n"
		"| Name | Age | Description |\n"
		"| -- | -- | -- |\n"
		"| Joe | 19 | An average guy with a super power. |\n\n"
		"# Here is another table:\n\n"
		"| Name | Age |\n"
		"| ted | 22 |\n\n";

	char *expectedResult_stack = \
		"# Here is a table\n\n"
		"| Name | Age |            Description             |\n"
		"| ---- | --- | ---------------------------------- |\n"
		"| Joe  | 19  | An average guy with a super power. |\n\n"
		"# Here is another table:\n\n"
		"| Name | Age |\n"
		"| ted  | 22  |\n\n";

	char *expectedResult = malloc( strlen(expectedResult_stack)+1 );
	char *table = malloc( strlen(table_stack)+1 );
	strcpy(table, table_stack);
	strcpy(expectedResult, expectedResult_stack);

	table = plugin_beautify_tables( table );

	if ( !string_equals(table, expectedResult)) {
		cr_log_error("plugins/beautify_tables test failed. Does not match expected.");
		printf("[ Expected ]\n%s---\n", expectedResult);
		printf("[ Actual   ]\n%s---\n-", table);
		cr_assert(0);
	}

	free(expectedResult);
	free(table);

  cr_assert(1);
}


Test(helpers, stringarrays) {
	MDStringList *list = mdstringlist_init();

	mdstringlist_alter(list, 0, 0, "Name");
	mdstringlist_alter(list, 0, 1, "Age");
	mdstringlist_alter(list, 0, 2, "Score");
	mdstringlist_alter(list, 0, 3, "Comments");
	mdstringlist_alter(list, 1, 0, "Doug");
	mdstringlist_alter(list, 1, 1, "19");
	mdstringlist_alter(list, 1, 2, "199");
	mdstringlist_alter(list, 1, 3, "I like C.");

	// Example Loop
	/* for (int y=0; y<list->count; y++) { */
	/* 	StringList *col = list->lists[y]; */
	/* 	for (int x=0; x<col->count; x++) { */
	/* 		printf("| %s ", col->strings[x]); */
	/* 	} */
	/* 	printf("\n"); */
	/* } */

	mdstringlist_free(list);

	cr_assert(1);
}
