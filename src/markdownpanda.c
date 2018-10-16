#include "markdownpanda.h"
#include "helper.h"
#include "htmlutils.h"

boolean ordered_list_mode = false;
int     ordered_list_index = 1;

typedef struct {
  char *prepend;
  char *append;
} AppendPrependData_t;

AppendPrependData_t getAppendPrepend(char *tag)
{
  AppendPrependData_t data = { "", "" };
  int tagID = get_tag_id( tag );

  if ( tagID > 0 && tagID <= 6 ) {
    char *heading_prefix = "";
    for (int i = 0; i < tagID; i++)
      heading_prefix = string_append(heading_prefix, "#");
    heading_prefix = string_append(heading_prefix, " ");
    data.prepend = heading_prefix;
  }
  else if ( tagID == TAG_BOLD ) {
    data.prepend = "**"; data.append = "** ";
  }
  else if ( tagID == TAG_ITALIC ) {
    data.prepend = "*"; data.append = "* ";
  }
  return data;
}

char *mdpanda_to_markdown(HtmlObject object)
{
  char               *markdown = "";
  myhtml_tree_t      *tree = object.tree;
  myhtml_tree_node_t *node = object.body;

  while (node) {
    myhtml_tag_id_t tag_id      = myhtml_node_tag_id(node);
    char	   *tag_name    = (char*) myhtml_tag_name_by_id(tree, tag_id, NULL);
    int html_tag_id = get_tag_id(tag_name);

    if (html_tag_id == TAG_OL)
      ordered_list_mode = true;

    // Process the node
    AppendPrependData_t appendPrependData = getAppendPrepend( tag_name );
    if (html_tag_id == TAG_LI) {
      if ( tag_is_parent(TAG_OL, tree, node) ) {
	char numberstr[4];
	sprintf(numberstr, "%i", ordered_list_index);
	appendPrependData.prepend = string_append(numberstr, ". ");
	appendPrependData.append = "";
	ordered_list_index++;
      } else {
	appendPrependData.prepend = "- ";
	appendPrependData.append = "";
      }
    }
    markdown = string_append(markdown, appendPrependData.prepend);
    if (tag_id == MyHTML_TAG__TEXT) {
      char *text = (char*) myhtml_node_text(node, NULL);
      if ( tag_is_parent(TAG_BLOCKQUOTE, tree, node) ) {
	text = trimWhitespace(text);
	text = string_prepend(text, "> ");
      }
      markdown = string_append(markdown, text);
    }

    // Append the children
    HtmlObject child = { object.myhtml_instance, tree, myhtml_node_child(node) };
    markdown = string_append(markdown, mdpanda_to_markdown(child)); 

    // Closing
    if (html_tag_id == TAG_OL) {
      ordered_list_mode = false;
      ordered_list_index = 1;
    }
    markdown = string_append(markdown, appendPrependData.append);

    if ( html_tag_id == TAG_LI ||
	 html_tag_id == TAG_UL ||
	 html_tag_id == TAG_OL)
      markdown = string_append(markdown, "\n");
    else if ( is_block_element(html_tag_id) )
      markdown = string_append(markdown, "\n\n");

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
  myhtml_tree_parse_flags_set(tree,
			      MyHTML_TREE_PARSE_FLAGS_SKIP_WHITESPACE_TOKEN |
			      MyHTML_TREE_PARSE_FLAGS_WITHOUT_DOCTYPE_IN_TREE
			      );

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
