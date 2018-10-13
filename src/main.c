#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <myhtml/api.h>

struct res_html {
    char  *html;
    size_t size;
};

void RemoveSpaces(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
}

char* concat(const char *s1, const char *s2)
{
  char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

void print_tree(myhtml_tree_t* tree, myhtml_tree_node_t *node)
{
  while (node) {
    myhtml_tag_id_t     tag_id          = myhtml_node_tag_id(node);
    char               *tag_name        = (char*) myhtml_tag_name_by_id(tree, tag_id, NULL);
    myhtml_tree_node_t *parent          = myhtml_node_parent(node);
    myhtml_tag_id_t     parent_tag_id   = myhtml_node_tag_id(parent);
    char               *parent_tag_name = (char*) myhtml_tag_name_by_id(tree, parent_tag_id, NULL);

    if (tag_id == MyHTML_TAG__TEXT) {
      char *text = (char*) myhtml_node_text(node, NULL);
      char text_nowhitespace[strlen(text)];
      strcpy(text_nowhitespace, text);
      RemoveSpaces((char*) text_nowhitespace);
      if (strncmp(text_nowhitespace, "", strlen(text)))
	printf("%s\n", text);
    } else {
      printf("<%s>", tag_name);
    }

    // print children
    print_tree(tree, myhtml_node_child(node));
    node = myhtml_node_next(node);
    }
}

int main()
{
  char *html =
    "<p>Welcome<p>test</p</p>"
    "<ul>"
    "  <li>My Item #1</li>"
    "  <li>My Item #2</li>"
    "</ul>"
    "<div>"
    "Welcome to my world"
    "</div>";
  
  // basic init
  myhtml_t* myhtml = myhtml_create();
  myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

  // init tree
  myhtml_tree_t* tree = myhtml_tree_create();
  myhtml_tree_init(tree, myhtml);

  // parse html
  myhtml_parse(tree, MyENCODING_UTF_8, html, strlen(html));

  // print tree
  myhtml_tree_node_t *node = myhtml_tree_get_node_body(tree);
  print_tree(tree, myhtml_node_child(node));

  // release resources
  myhtml_tree_destroy(tree);
  myhtml_destroy(myhtml);

  return 0;
}


#ifdef __cplusplus
}
#endif
