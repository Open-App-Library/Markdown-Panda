#include <myhtml/api.h>
#include "markdownpanda.h"
#include "helper.h"
#include "htmlutils.h"

#ifdef _WIN32
    #define MyCORE_FMT_Z "%Iu"
#else
    #define MyCORE_FMT_Z "%zu"
#endif

int     list_nesting       = 0;
boolean should_add_newline_to_child_list;

typedef struct {
  char *prepend;
  char *append;
} AppendPrependData_t;

typedef struct {
  int *colSizes; // Array
  int  colCount;
  boolean created;
} MarkdownTable;

MarkdownTable current_table;

AppendPrependData_t getAppendPrepend( char *tag, myhtml_tree_t *tree, myhtml_tree_node_t *node)
{
  AppendPrependData_t data = { "", "" };
  int tagID = get_tag_id( tag );
  myhtml_tag_id_t node_tagID = myhtml_node_tag_id(node);

  switch ( tagID ) {
  case TAG_BOLD:
    data.prepend = "**"; data.append = "**";
    break;
  case TAG_ITALIC:
    data.prepend = "*"; data.append = "*";
    break;
  case TAG_HR:
    data.prepend = "---"; data.append = "";
    break;
  case TAG_STRIKE:
    data.prepend = "~~"; data.append = "~~";
    break;
  case TAG_LINK:
    data.prepend = "[";
    data.append = "](";
    myhtml_tree_attr_t *href_attr = myhtml_attribute_by_key(node, "href", 4);
    if (href_attr) {
      const char *attr_char = myhtml_attribute_value(href_attr, NULL);
      if (attr_char)
	data.append = string_append(data.append, attr_char);
    }
    data.append = string_append(data.append, ")");
    break;
  case TAG_IMG:
    data.prepend = "![";
    data.append = "](";
    myhtml_tree_attr_t *src_attr = myhtml_attribute_by_key(node, "src", 3);
    myhtml_tree_attr_t *alt_attr = myhtml_attribute_by_key(node, "alt", 3);
    if ( src_attr ) {
      const char *src_char = myhtml_attribute_value(src_attr, NULL);
      if (src_char)
	data.append = string_append(data.append, src_char);
    }
    if ( alt_attr ) {
      const char *alt_char = myhtml_attribute_value(alt_attr, NULL);
      if (alt_char)
	data.prepend = string_append(data.prepend, alt_char);      
    }
    data.append = string_append(data.append, ")");
    break;
  case TAG_CODE:
    if ( !tag_is_parent(TAG_PRE, tree, node) ) {
      data.prepend = "`"; data.append = "`";
    } else {
      data.prepend = "";
      if ( myhtml_node_next(node) ) {
	data.append = "\n";
      } else {
	data.append = "";
      }
    }
    break;
  case TAG_PRE:
    data.prepend = "```\n"; data.append = "\n```";
    break;
  case TAG_TR:
    if ( node_is_id(TAG_TH, tree, myhtml_node_child(node) ) ) {
      data.append = "\n";
      for (int i = 0; i < current_table.colCount; i++) {
    	data.append = string_append(data.append, "|");
    	for (int s = 0; s < current_table.colSizes[i] + 2; s++) { // +2 for two spaces on both siide of table to give nice space
    	  data.append = string_append(data.append, "-");
    	}
      }
      data.append = string_append(data.append, "|\n");
    } else if ( myhtml_node_next(node) ) {
      data.append = "\n";
    }
    break;
  case TAG_TD:
    if ( !myhtml_node_next(node) ) {
      data.prepend = " | ";
      data.append  = " |";
    }
    else if ( myhtml_node_prev(node) )
      data.prepend = " | ";
    else
      data.prepend = "| ";
    break;
  case TAG_TH:
    if ( !myhtml_node_next(node) ) {
      data.prepend = " | ";
      data.append  = " |";
    }
    else if ( myhtml_node_prev(node) )
      data.prepend = " | ";
    else
      data.prepend = "| ";
    break;
  case TAG_LI:
      if ( node_parent_is_id(TAG_OL, tree, node) ) {
      	char numberstr[4];
	int index = 1;
	myhtml_tree_node_t *other_list_item = myhtml_node_prev(node);
	while (other_list_item) {
	  index++;
	  other_list_item = myhtml_node_prev( other_list_item );
	}
      	sprintf(numberstr, "%i", index);
      	data.prepend = string_append(numberstr, ". ");
      	data.append = "";
      } else {
      	data.prepend = "- ";
      	data.append = "";
      }
      if ( list_nesting > 1) {
	int indentation = 2;
	int curLineIndentation = list_nesting * indentation - 2;
	char spaces[curLineIndentation+2];
	spaces[0] = '\n';
	for (int i = 1; i < curLineIndentation+1; i++) {
	  spaces[i] = ' ';
	}
	spaces[curLineIndentation+1] = '\0';
	data.prepend = string_prepend(data.prepend, spaces);
	should_add_newline_to_child_list = false;
      }
      break;
  case TAG_TEXT: {
    char *text = (char*) myhtml_node_text(node, NULL);
    boolean blockquote_parent = tag_is_parent(TAG_BLOCKQUOTE, tree, node);
    boolean li_parent = tag_is_parent(TAG_LI, tree, node);
    boolean table_parent = tag_is_parent(TAG_TABLE, tree, node);
    if ( blockquote_parent ) {
      text = trimWhitespace(text);
      text = string_prepend(text, "> ");
    } else if ( li_parent || table_parent) {
      text = trimWhitespace(text);
    }
    if ( table_parent ) {
      int curColIndex = 0;
      myhtml_tree_node_t *prevCell = myhtml_node_prev( myhtml_node_parent(node) );
      if (curColIndex <= current_table.colCount) {
	while (prevCell) {
	  curColIndex++;
	  prevCell = myhtml_node_prev(prevCell);
	}
	int textLen = strlen(text); // 2 for the two spaces added on the side for paddingo
	int spacesToAdd = current_table.colSizes[curColIndex] - textLen;
	for (int s = 0; s < spacesToAdd; s++) {
	  data.append = string_append(data.append, " ");
	}
      } else {
	puts("ERROR! Counted a table column count greater than exists. Exiting...");
	exit( EXIT_FAILURE );
      }
    }
    data.prepend = "";
    data.prepend = string_append(data.prepend, text);
    break;   
  }
  default:
    if ( tagID > 0 && tagID <= 6 ) {
      char *heading_prefix = "";
      for (int i = 0; i < tagID; i++)
	heading_prefix = string_append(heading_prefix, "#");
      heading_prefix = string_append(heading_prefix, " ");
      data.prepend = heading_prefix;
    }
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

    myhtml_tag_id_t pid      = myhtml_node_tag_id(myhtml_node_parent(node));
    char	   *pname    = (char*) myhtml_tag_name_by_id(tree, pid, NULL);

    if (html_tag_id == TAG_OL || html_tag_id == TAG_UL) {
      list_nesting++;
      should_add_newline_to_child_list = true;
    }

    // Hacky fix for tfoot no-new-line behavior
    if (string_equals(tag_name, "tfoot")) {
      markdown = string_append(markdown, "\n");
    }

    if (html_tag_id == TAG_TABLE && !current_table.created ) {
      myhtml_tree_node_t *thead_or_tbody = myhtml_node_child(node);
      while (thead_or_tbody) {
	myhtml_tree_node_t *tr = myhtml_node_child(thead_or_tbody);
	while (tr) {
	  // Loop through 'th' or 'td's (cells)
	  myhtml_tree_node_t *cell = myhtml_node_child(tr);
	  int colIndex = 0;
	  while (cell) {
	    myhtml_tree_node_t *cellChild = myhtml_node_child(cell);
	    while (cellChild) {
	      myhtml_tag_id_t cellChildID = myhtml_node_tag_id(cellChild);
	      char *tag = (char*) myhtml_tag_name_by_id(tree, cellChildID, NULL);
	      if ( string_equals(tag, "-text") ) {
		char *text = (char*) myhtml_node_text(cellChild, NULL);
		if (text) {
		  if (current_table.colCount < colIndex+1) {
		    int *newColSizes = malloc(sizeof(int) * (colIndex+1));
		    for (int i = 0; i < colIndex+1; i++) // Zero-out values
		      newColSizes[i] = 0;
		    for (int i = 0; i < current_table.colCount; i++) {
		      newColSizes[i] = current_table.colSizes[i];
		    }
		    if (current_table.created) {
		      free(current_table.colSizes);
		    }
		    current_table.colSizes = newColSizes;
		    current_table.colCount = colIndex + 1;
		  }
		  int strLen = strlen(text);
		  if (strLen > current_table.colSizes[colIndex]) {
		    current_table.colSizes[colIndex] = strLen;
		  }
		}
	      }
	      cellChild = myhtml_node_child(cellChild);
	    }
	    colIndex++;
	    cell = myhtml_node_next(cell);
	  }
	  tr = myhtml_node_next(tr);
	}
	thead_or_tbody = myhtml_node_next(thead_or_tbody);
      }
      current_table.created = true;
    }

    // Process the node
    AppendPrependData_t appendPrependData = getAppendPrepend( tag_name, tree, node );
    markdown = string_append(markdown, appendPrependData.prepend);

    // Append the children
    HtmlObject child = { object.myhtml_instance, tree, myhtml_node_child(node) };
    markdown = string_append(markdown, mdpanda_to_markdown(child)); 

    // Closing
    markdown = string_append(markdown, appendPrependData.append);

    // Tables
    if (html_tag_id == TAG_TABLE) {
      free( current_table.colSizes );
      current_table.colCount = 0;
      current_table.created = false;
    }

    // Newline
    if ( html_tag_id == TAG_LI ||
	 html_tag_id == TAG_UL ||
	 html_tag_id == TAG_OL) {
      if ( !(html_tag_id == TAG_UL && list_nesting > 1) &&
	   !(html_tag_id == TAG_OL && list_nesting > 1) &&
	   !(html_tag_id == TAG_LI && list_nesting > 1))
	markdown = string_append(markdown, "\n");
    }
    else if ( is_block_element(html_tag_id) )
      markdown = string_append(markdown, "\n\n");

    if (html_tag_id == TAG_OL || html_tag_id == TAG_UL) {
      list_nesting--;
    }

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

  
  HtmlObject obj = {myhtml, tree, body};
  return obj;
}

HtmlObject load_html_from_file(char *filename)
{
  char *textOfFile = file_to_string(filename);
  HtmlObject obj = load_html_from_string( textOfFile );
  free(textOfFile);
  return obj;
}

void destroy_html_object(HtmlObject object)
{
  myhtml_tree_destroy(object.tree);
  myhtml_destroy(object.myhtml_instance);
}
