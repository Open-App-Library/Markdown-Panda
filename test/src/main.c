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
    boolean isSuccess = true;

    if ( !string_equals(t.html, html) ) {
      printf("\n markdown_to_html '%s' FAILED \n\n", type);
      printdiff("Expected", "CMark   ", t.html, html);
      isSuccess = false;
    }

    free(t.html);
    free(t.md);
    free(html);

    cr_assert(isSuccess);
  }
}

/* Test(formatting, markdown_to_html) { */
/*   mdpanda_init_hoedown(); */

/*   char *h1 = mdpanda_to_html( "hello world\n\nthis is a love" ); */
/*   char *h2 = mdpanda_to_html( "I like stuff" ); */
/*   char *h3 = mdpanda_to_html( "apple\n\ntetsdfsdfsdfdsfd\n\ntestse" ); */

/*   char *format = "\n<html>\n%s\n</html>\n\n"; */
/*   printf(format, h1); */
/*   printf(format, h2); */
/*   printf(format, h3); */

/*   mdpanda_destroy_hoedown(); */
/*   cr_assert(1); */
/* } */
