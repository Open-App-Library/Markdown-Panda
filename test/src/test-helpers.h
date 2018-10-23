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

#endif

