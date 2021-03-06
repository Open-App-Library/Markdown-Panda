#ifndef MARKDOWNPANDA_H
#define MARKDOWNPANDA_H

#ifdef __cplusplus
extern "C" {
#endif

#define TYPE_HTML 1
#define TYPE_MARKDOWN 2

#define HTML_FILE_NOT_FOUND_MESSAGE "<h1>ERROR: HTML file not found</h1>"
#define MARKDOWN_FILE_NOT_FOUND_MESSAGE "# ERROR: Markdown file not found"

typedef struct myhtml myhtml_t;
typedef struct myhtml_tree myhtml_tree_t;
typedef struct myhtml_tree_node myhtml_tree_node_t;

  typedef struct {
    myhtml_t *myhtml_instance;
    myhtml_tree_t *tree;
    myhtml_tree_node_t *body;
  } HtmlObject;

  typedef struct {
    char *attr;
    char *value;
  } HtmlAttribute;

  typedef struct {
    HtmlAttribute **attributes;
    int count;
  } HtmlAttributes;

  // First you need an HTMLObject.
  // Either load one from a string..or load one from a file.
  HtmlObject load_html_from_string(char *string);
  HtmlObject load_html_from_file(char *filename);
  char *load_markdown_from_file(char *filename);

  // With your HtmlObject,
  // you can now convert it to markdown like so.
  // HTML -> Markdown
  char *mdpanda_to_markdown(HtmlObject object);

  // Markdown -> HTML
  char *mdpanda_to_html(char *markdown_string); // Then you can convert

  // After you're all done, destroy your HtmlObject
  // to free the memory.
  // NOTE: HtmlObject is a struct. The struct itself
  // does not actually get freed. However, it has
  // members that are freed.
  void destroy_html_object(HtmlObject object);
  
#ifdef __cplusplus
}
#endif

#endif
