#include <myhtml/api.h>
#include "markdownpanda.h"
#include "helper.h"
#include "htmlutils.h"
#include "plugins.h"

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

int current_table = 0;
boolean current_table_created = False;
boolean current_table_created_hor_line = False;

char *string_append_tag( char *str, char *append )
{
	char *newStr;
	int len = strlen(str);

	if (len) {
		newStr = strdup(str);
		char last_char_of_str = str[strlen(newStr)-1];
		char first_char_of_append = append[0];
		if ( last_char_of_str     == '*' && first_char_of_append == '*')
			newStr = string_append(newStr, " ");

		newStr = string_append(newStr, append);
		free(str);
	} else {
		newStr = strdup(append);
	}

	return newStr;
}

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
			if (current_table_created_hor_line)
				break;
      data.append = "\n";
      for (int i = 0; i < current_table; i++) {
				data.append = string_append(data.append, "|--");
      }
      data.append = string_append(data.append, "|\n");
			current_table_created_hor_line = True;
    } else if ( myhtml_node_next(node) ) {
      data.append = "\n";
    }
    break;
	case TAG_THEAD:
		if (current_table_created_hor_line)
			break;
		data.append = "\n";
		for (int i = 0; i < current_table; i++) {
			data.append = string_append(data.append, "|--");
		}
		data.append = string_append(data.append, "|\n");
		current_table_created_hor_line = True;
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
    boolean code_parent = tag_is_parent(TAG_CODE, tree, node);
    boolean bold_parent = tag_is_parent(TAG_BOLD, tree, node);
    boolean italic_parent = tag_is_parent(TAG_ITALIC, tree, node);
		boolean mustFreeText = False;

    if ( blockquote_parent ) {
      text = trimWhitespace(text);
      text = string_prepend(text, "> ");
    } else if ( li_parent || table_parent) {
      text = trimWhitespace(text);
    }

    if ( table_parent ) {
      int curColIndex = 0;
      myhtml_tree_node_t *prevCell = myhtml_node_prev( myhtml_node_parent(node) );
      if (curColIndex <= current_table) {
				while (prevCell) {
					curColIndex++;
					prevCell = myhtml_node_prev(prevCell);
				}
				int textLen = strlen(text); // 2 for the two spaces added on the side for paddingo
      } else {
				puts("ERROR! Counted a table column count greater than exists. Exiting...");
				exit( EXIT_FAILURE );
      }
    }

    if ( code_parent ) {
      trimTrailingNewlines(text);
    }

		// Make sure no spaces at beginning or end of text
		// Function ID: REMOVE_TRAILING_FROM_TAGS
		if ( bold_parent || italic_parent ) {
			// Check beginning
			int origStrLen = strlen(text);
			int start=0, end=origStrLen;
			int spacesToPrepend=0, spacesToAppend=0;
			// Find the first non-space
			for ( int i = 0; i < origStrLen+1; i++ ) {
				if (text[i] == ' ') start = i+1;
				else break;
			}
			// Find the first non-space, starting from the end (right-to-left)
			for ( int i = origStrLen-1; i > 0; i-- ) {
				if (text[i] == ' ') end = i;
				else break;
			}
			// Create new string
			if ( start > 0 || end < origStrLen ) {
				char *new = malloc( end-start+2 );
				for (int i = start; i < end; i++) {
					if (i-start < 0)
						puts("[WARNING] markdownpanda.c i-start is less than zero. This should never happen!\n(function ID=REMOVE_TRAILING_FROM_TAGS)");
					new[i-start] = text[i];
					mustFreeText = True;
				}
				new[end] = '\0';
				text = new;
				spacesToPrepend = start;
				spacesToAppend = origStrLen - end;
			}
		}
    data.prepend = "";
    data.prepend = string_append(data.prepend, text);
		if (mustFreeText)
			free(text);
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

char *mdpanda_to_markdown_recursive(HtmlObject object)
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
    int parent_html_tag_id = get_tag_id(pname);

		// If empty tag, don't bother.
		if ( is_child_containing_element(html_tag_id) && !myhtml_node_child(node) ) {
			node = myhtml_node_next(node);
			continue;
		}

    if (html_tag_id == TAG_OL || html_tag_id == TAG_UL) {
      list_nesting++;
      should_add_newline_to_child_list = true;
    }

    // Hacky fix for tfoot no-new-line behavior
    if (string_equals(tag_name, "tfoot")) {
      markdown = string_append(markdown, "\n");
    }

		// TODO: This is way over-nested. Fix it.
    if (html_tag_id == TAG_TABLE && !current_table_created ) {
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
									if (current_table < colIndex+1) {
										current_table = colIndex + 1;
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
      current_table_created = true;
			current_table_created_hor_line = False;
    }

    // Process the node
    AppendPrependData_t appendPrependData = getAppendPrepend( tag_name, tree, node );
		//		markdown = add_space_if_needed( markdown );
    markdown = string_append_tag(markdown, appendPrependData.prepend);

    // Append the children
    HtmlObject child = { object.myhtml_instance, tree, myhtml_node_child(node) };
    markdown = string_append(markdown, mdpanda_to_markdown_recursive(child));

    // Closing
    markdown = string_append(markdown, appendPrependData.append);

    // Tables
    if (html_tag_id == TAG_TABLE) {
      current_table = 0;
      current_table_created = false;
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
    else if ( is_block_element(html_tag_id) && !is_block_element(parent_html_tag_id) && !is_table_cell(parent_html_tag_id) )
      markdown = string_append(markdown, "\n\n");

		/* // Special treatment for THEADs */
		/* if (parent_html_tag_id == TAG_THEAD) { */
    /*   markdown = string_append(markdown, "\n"); */
		/* } */

    if (html_tag_id == TAG_OL || html_tag_id == TAG_UL) {
      list_nesting--;
    }

    node = myhtml_node_next(node);
  }

  return markdown;
}

char *mdpanda_to_markdown(HtmlObject object)
{
  char *markdown = mdpanda_to_markdown_recursive(object);

  // Process output
  markdown = plugin_ensure_newline(markdown);
	markdown = plugin_beautify_tables(markdown);

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
  HtmlObject obj;
  if (textOfFile)
    obj = load_html_from_string( textOfFile );
  else
    obj = load_html_from_string( HTML_FILE_NOT_FOUND_MESSAGE );
  free(textOfFile);
  return obj;
}

char *load_markdown_from_file(char *filename)
{
  char *textOfFile = file_to_string(filename);
  if (textOfFile)
    return textOfFile;
  return MARKDOWN_FILE_NOT_FOUND_MESSAGE;
}

void destroy_html_object(HtmlObject object)
{
  myhtml_tree_destroy(object.tree);
  myhtml_destroy(object.myhtml_instance);
}
