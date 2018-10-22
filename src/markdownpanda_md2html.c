#include "markdownpanda.h"
#include <hoedown/html.h>
#include <string.h>
#include "unescape.h"

#define MARKDOWNPANDA_MARKDOWN_BUFFER_UNIT 1024
#define MARKDOWNPANDA_HTML_BUFFER_UNIT 64
#define MARKDOWNPANDA_MAX_NESTING 16

char *mdpanda_to_html(char *markdown_string)
{
  hoedown_renderer *r = hoedown_html_renderer_new(0, 0);
  unsigned int extensions = \
    HOEDOWN_EXT_TABLES         | \
    HOEDOWN_EXT_UNDERLINE      | \
    HOEDOWN_EXT_FENCED_CODE    | \
    HOEDOWN_EXT_STRIKETHROUGH;
  hoedown_document *d = hoedown_document_new(r, extensions, MARKDOWNPANDA_MAX_NESTING);

  hoedown_buffer *markdown = hoedown_buffer_new( MARKDOWNPANDA_MARKDOWN_BUFFER_UNIT );
  hoedown_buffer *html = hoedown_buffer_new( MARKDOWNPANDA_HTML_BUFFER_UNIT );

  hoedown_buffer_printf(markdown,"%s", markdown_string);
  hoedown_document_render(d, html, markdown->data, markdown->size);
  
  char *your_html = malloc(sizeof(char) * html->size + 1);
  strcpy(your_html, (char*) html->data);

  hoedown_buffer_free(html);
  hoedown_document_free(d);
  hoedown_html_renderer_free(r);

  char *your_html_unescaped = malloc( strlen(your_html) + 1);
  decode_html_entities_utf8(your_html_unescaped, your_html);
  free(your_html);

  return your_html_unescaped;
}
