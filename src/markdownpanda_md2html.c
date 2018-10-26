#include "markdownpanda.h"
#include <string.h>
#include "unescape.h"
#include <cmark.h>

char *mdpanda_to_html(char *markdown_string)
{
  return cmark_markdown_to_html(markdown_string, strlen(markdown_string),
				CMARK_OPT_SAFE | );
}

/* #define MARKDOWNPANDA_HTML_BUFFER_UNIT 64 */
/* #define MARKDOWNPANDA_MARKDOWN_BUFFER_UNIT 64 */
/* #define MARKDOWNPANDA_MAX_NESTING 16 */

/* hoedown_renderer *r  = NULL; */
/* hoedown_document *d  = NULL; */
/* hoedown_buffer *html = NULL; */
/* hoedown_buffer *markdown = NULL; */

/* void mdpanda_init_hoedown() */
/* { */
/*   unsigned int extensions = \ */
/*     HOEDOWN_EXT_TABLES         | \ */
/*     HOEDOWN_EXT_UNDERLINE      | \ */
/*     HOEDOWN_EXT_FENCED_CODE    | \ */
/*     HOEDOWN_EXT_STRIKETHROUGH; */
/*   r = hoedown_html_renderer_new(0, 0); */
/*   d = hoedown_document_new(r, extensions, MARKDOWNPANDA_MAX_NESTING); */
/*   html = hoedown_buffer_new( MARKDOWNPANDA_HTML_BUFFER_UNIT ); */
/*   markdown = hoedown_buffer_new( MARKDOWNPANDA_MARKDOWN_BUFFER_UNIT ); */
/* } */

/* void mdpanda_destroy_hoedown() */
/* { */
/*   hoedown_buffer_free(html); */
/*   hoedown_buffer_free(markdown); */
/*   hoedown_document_free(d); */
/*   hoedown_html_renderer_free(r); */
/*   html = NULL; */
/*   markdown = NULL; */
/*   d = NULL; */
/*   r = NULL; */
/* } */

/* char *mdpanda_to_html(char *markdown_string) */
/* { */

/*   hoedown_document_render(d, html, (uint8_t*)markdown_string, strlen(markdown_string)); */

/*   char *your_html = malloc(sizeof(char) * html->size + 1); */
/*   strcpy(your_html, (char*)html->data); */

/*   char *your_html_unescaped = malloc( strlen(your_html) + 1); */
/*   decode_html_entities_utf8(your_html_unescaped, your_html); */

/*   free(your_html); */

/*   hoedown_buffer_reset(markdown); */
/*   hoedown_buffer_reset(html); */

/*   return your_html_unescaped; */
/* } */
