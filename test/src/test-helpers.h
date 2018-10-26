#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <markdownpanda.h>
#include <helper.h>

#define MD_LOCATION "../test/md/"
#define HTML_LOCATION "../test/html/"

typedef struct {
  char *md;
  char *html;
  HtmlObject htmlObject;
} text_t;

text_t load(char *type)
{
  char *md_file = string_append(MD_LOCATION, string_append(type, ".md"));
  char *html_file = string_append(HTML_LOCATION, string_append(type, ".html"));
  char *md = load_markdown_from_file( md_file );
  char *html = file_to_string( html_file );
  HtmlObject htmlObject = load_html_from_string( html );
  text_t t = {md, html, htmlObject};
  return t;
}

char *strip_ignored_chars (char *str, char *ignored_chars)
{
  int len = strlen(str) + 1;
  char *new = malloc( len );
  int counter = 0;
  for (int i=0; i < len; i++) {
    boolean can_add = True;
    for (int rule = 0; rule < strlen(ignored_chars)+1; rule++) {
      if ( str[i] == ignored_chars[rule] ) {
	can_add = False;
      }
    }
    if ( can_add ) {
      new[counter] = str[i];
      counter++;
    }
  }
  if ( counter+1 < len ) { // If string is less than needed. Reallocate to smaller.
    char *new_new = malloc(counter+1);
    for (int i=0; i < counter; i++)
      new_new[i] = new[i];
    free(new);
    new = new_new;
  }
  new[counter] = '\0';
  return new;
}

#endif

