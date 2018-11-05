#ifndef HTMLUTILS_H
#define HTMLUTILS_H

#include <myhtml/api.h>
#include "helper.h"

#define TAG_OTHER      -1
#define TAG_TEXT	0
#define TAG_H1		1
#define TAG_H2		2
#define TAG_H3		3
#define TAG_H4		4
#define TAG_H5		5
#define TAG_H6		6
#define TAG_LINK	7
#define TAG_BOLD	8
#define TAG_ITALIC	9
#define TAG_IMG  	10
#define TAG_LI          11
#define TAG_HR          12
#define TAG_P           13
#define TAG_PRE         14
#define TAG_TABLE       15
#define TAG_BLOCKQUOTE  16
#define TAG_BR          17
#define TAG_OL          18
#define TAG_UL          19
#define TAG_CODE        20
#define TAG_STRIKE      21
#define TAG_TR		22
#define TAG_TH		23
#define TAG_TD		24

int get_tag_id(char *tag)
{
  if      ( string_equals(tag, "-text") )
    return TAG_TEXT;
  else if ( string_equals(tag, "h1") )
    return TAG_H1;
  else if ( string_equals(tag, "h2") )
    return TAG_H2;
  else if ( string_equals(tag, "h3") )
    return TAG_H3;
  else if ( string_equals(tag, "h4") )
    return TAG_H4;
  else if ( string_equals(tag, "h5") )
    return TAG_H5;
  else if ( string_equals(tag, "h6") )
    return TAG_H6;
  else if ( string_equals(tag, "a") )
    return TAG_LINK;
  else if ( string_equals(tag, "strong") || string_equals(tag, "b"))
    return TAG_BOLD;
  else if ( string_equals(tag, "i") || string_equals(tag, "em") )
    return TAG_ITALIC;
  else if ( string_equals(tag, "img") )
    return TAG_IMG;
  else if ( string_equals(tag, "li") )
    return TAG_LI;
  else if ( string_equals(tag, "hr") )
    return TAG_HR;
  else if ( string_equals(tag, "p") )
    return TAG_P;
  else if ( string_equals(tag, "pre") )
    return TAG_PRE;
  else if ( string_equals(tag, "table") )
    return TAG_TABLE;
  else if ( string_equals(tag, "blockquote") )
    return TAG_BLOCKQUOTE;
  else if ( string_equals(tag, "br") )
    return TAG_BR;
  else if ( string_equals(tag, "ol") )
    return TAG_OL;
  else if ( string_equals(tag, "ul") )
    return TAG_UL;
  else if ( string_equals(tag, "code") )
    return TAG_CODE;
  else if ( string_equals(tag, "strike") || string_equals(tag, "del") )
    return TAG_STRIKE;
  else if ( string_equals(tag, "tr") )
    return TAG_TR;
  else if ( string_equals(tag, "th") )
    return TAG_TH;
  else if ( string_equals(tag, "td") )
    return TAG_TD;
  //printf("[WARNING] Could not find tag '%s'\n", tag);
  return -1;
}

boolean node_is_id(int target_html_tag_id, myhtml_tree_t *tree, myhtml_tree_node_t *node)
{
  myhtml_tag_id_t tag_id = myhtml_node_tag_id( node );
  char	   *tag_name     = (char*) myhtml_tag_name_by_id(tree, tag_id, NULL);
  int       html_tag_id  = get_tag_id(tag_name);
  if ( html_tag_id == target_html_tag_id )
    return true;
  return false;
}

boolean node_parent_is_id(int target_html_tag_id, myhtml_tree_t *tree, myhtml_tree_node_t *node)
{
  return node_is_id( target_html_tag_id, tree, myhtml_node_parent(node) );
}

boolean tag_is_parent(int target_html_tag_id, myhtml_tree_t *tree, myhtml_tree_node_t *node)
{
  myhtml_tree_node_t *parent = myhtml_node_parent(node);
  while (parent) {
    myhtml_tag_id_t tag_id      = myhtml_node_tag_id(parent);
    char	   *tag_name    = (char*) myhtml_tag_name_by_id(tree, tag_id, NULL);
    int             html_tag_id = get_tag_id(tag_name);

    if (html_tag_id == target_html_tag_id) {
      return true;
    }
    parent = myhtml_node_parent(parent);
  }
  return false;
}

boolean is_block_element(int tag_id)
{
  boolean is_heading = tag_id > 0 && tag_id <= 6;
  if (is_heading                || // If tag is a h1-h6
      tag_id == TAG_IMG		||
      tag_id == TAG_HR		||
      tag_id == TAG_P		||
      tag_id == TAG_PRE		||
      tag_id == TAG_TABLE	||
      tag_id == TAG_BLOCKQUOTE  ||
      tag_id == TAG_BR		||
      tag_id == TAG_OL		||
      tag_id == TAG_UL)            // Note that LI is not included even though it is a block element. It is not needed here.
    return true;
  return false;
}

boolean is_child_containing_element(int tag_id)
{
	if (tag_id == TAG_HR	||
			tag_id == TAG_BR	||
			tag_id == TAG_IMG ||
			tag_id == TAG_TEXT)
		return false;
	return true;
}

#endif
