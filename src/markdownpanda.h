#ifndef MARKDOWNPANDA_H
#define MARKDOWNPANDA_H

#ifdef __cplusplus
extern "C" {
#endif

#define HTML_FILE_NOT_FOUND_MESSAGE "<h1>ERROR: HTML File not found</h1>"

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

  // With your HtmlObject,
  // you can now convert it to markdown like so.
  char *mdpanda_to_markdown(HtmlObject object);

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
