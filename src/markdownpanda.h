#ifndef MARKDOWNPANDA_H
#define MARKDOWNPANDA_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
    #define MyCORE_FMT_Z "%Iu"
#else
    #define MyCORE_FMT_Z "%zu"
#endif
  
#include <myhtml/api.h>

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

#include <myhtml/api.h>

  char *mdpanda_to_markdown(HtmlObject object);

  HtmlObject load_html_from_string(char *string);
  HtmlObject load_html_from_file(char *filename);

  void destroy_html_object(HtmlObject object);
  
#ifdef __cplusplus
}
#endif

#endif
