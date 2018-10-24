/*
 * The entry-point file for unit testing.
 */

#include <criterion/criterion.h>
#include <stdio.h>
#include <markdownpanda.h>
#include "test-helpers.h"
#include "printdiff.h"

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
/* Test(formatting, html_to_markdown) { */
/*   for (int i = 0; i < format_count; i++) { */
/*     char *type = format_types[i]; */
/*     text_t t = load(type); */
/*     char *md = mdpanda_to_markdown( t.htmlObject ); */
/*     if ( ! string_equals( t.md, md) ) { */
/*       printf("\n html_to_markdown '%s' FAILED \n\n", type); */
/*       cr_assert(0, "Formatting test failed."); */
/*     } */
/*     cr_assert(1); */
/*   } */
/* } */

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

    if ( ! string_equals( t.html, html ) ) {
      printf("\n markdown_to_html '%s' FAILED \n\n", type);
      printdiff("Expected HTML", "Hoedown HTML ", t.html, html);
      cr_assert(0, "Formatting test failed.");
    }
    cr_assert(1);
  }
}
