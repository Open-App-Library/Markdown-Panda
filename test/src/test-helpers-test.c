#include <stdio.h>
#include <markdownpanda.h>
#include <string.h>
#include <helper.h>
#include "test-helpers.h"
#include "printdiff.h"

char *new_string(char *str)
{
  char *new = malloc( strlen(str) +1 );
  strcpy(new, str);
  return new;
}

int main()
{

  /* char *s1 = string_new("Hello world"); */
  /* char *s2 = string_new("byte"); */
  /* char *str = string_cat_with_color(COLOR_RED, s1, s2); */

  /* printf("%s\n", str); */

  /* free(str); */
  
  char *type = "code";
  text_t t = load(type);
  char *html = mdpanda_to_html( t.md );

  if ( ! string_equals( t.html, html ) ) {
    printf("\n markdown_to_html '%s' failed \n\n", type);
    printdiff("expected html", "hoedown html ", t.html, html);
  }
}

