#include "markdownpanda.h"
#include "helper.h"
#include "htmlutils.h"

void processText(char *text, char *pname)
{
  int tagID = get_tag_id(pname);
  if ( tagID == TAG_BOLD )
    text = string_wrap(text, "**");
}

char *buildMarkdownString(char *currentString, myhtml_tree_t *tree, myhtml_tree_node_t *node)
{
  myhtml_tag_id_t tag_id   = myhtml_node_tag_id(node);
  char           *tag_name = (char*) myhtml_tag_name_by_id(tree, tag_id, NULL);
  char           *text = (char*) myhtml_node_text(node, NULL);

  myhtml_tag_id_t     parent_tag_id   = myhtml_node_tag_id( myhtml_node_parent(node) );
  char               *parent_tag_name = (char*) myhtml_tag_name_by_id(tree, parent_tag_id, NULL);
  myhtml_tree_node_t *parentNode = myhtml_node_parent(node);

  while (parentNode) {
    myhtml_tag_id_t pID   = myhtml_node_tag_id(parentNode);
    char           *pNAME = (char*) myhtml_tag_name_by_id(tree, pID, NULL);
    if ( strcmp(pNAME, "body") == 0 )
      break;

    processText(text, pNAME);

    printf("%s, ", pNAME);
    parentNode = myhtml_node_parent(parentNode);
  }

  printf("%s\n", text);

  currentString = string_append(currentString, text);
  return currentString;
}

char *mdpanda_to_markdown(HtmlObject object)
{
  char               *markdown = "";
  myhtml_tree_t      *tree = object.tree;
  myhtml_tree_node_t *node = object.body;

  while (node) {
    myhtml_tag_id_t     tag_id          = myhtml_node_tag_id(node);
    char               *tag_name        = (char*) myhtml_tag_name_by_id(tree, tag_id, NULL);

    boolean skip_node = false;

    if ( !tag_name || strcmp( tag_name, "body" ) == 0 ) {
      skip_node = true;
    }
    else if (tag_id == MyHTML_TAG__TEXT) {
      char *text = (char*) myhtml_node_text(node, NULL);
      char  text_cpy[ strlen(text)+1 ];
      strcpy(text_cpy, text);
      removeWhitespace(text_cpy);
      if ( strcmp(text_cpy, "") == 0) // If string is not empty
	skip_node = true;
    } else {
      skip_node = true;
    }

    if ( !skip_node ) {
      buildMarkdownString( markdown, tree, node );
    }

    // print children
    HtmlObject child;
    child.myhtml_instance = object.myhtml_instance;
    child.tree = tree;
    child.body = myhtml_node_child(node);

    mdpanda_to_markdown(child);
    node = myhtml_node_next(node);
  }
  return markdown;
}

HtmlObject load_html_from_string(char *string)
{
  myhtml_t* myhtml = myhtml_create();
  myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

  // init tree
  myhtml_tree_t* tree = myhtml_tree_create();
  myhtml_tree_init(tree, myhtml);

  // parse html
  myhtml_parse(tree, MyENCODING_UTF_8, string, strlen(string));

  myhtml_tree_node_t *body = myhtml_tree_get_node_body(tree);

  HtmlObject obj;
  obj.myhtml_instance = myhtml;
  obj.tree = tree;
  obj.body = body;

  return obj;
}

HtmlObject load_html_from_file(char *filename)
{
  return load_html_from_string( file_to_string(filename) );
}

void destroy_html_object(HtmlObject object)
{
  myhtml_tree_destroy(object.tree);
  myhtml_destroy(object.myhtml_instance);
}
