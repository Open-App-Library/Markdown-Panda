#ifndef HTMLUTILS_H
#define HTMLUTILS_H

#include "helper.h"

#define TAG_OTHER	0
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

int get_tag_id(char *tag)
{
  if ( string_equals(tag, "h1") )
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
  else if ( string_equals(tag, "i") )
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
  return 0;
}

boolean is_block_element(int tag_id)
{
  boolean is_heading = tag_id > 0 && tag_id <= 6;
  if (is_heading                        || // If tag is a h1-h6
      tag_id == TAG_IMG			||
      tag_id == TAG_LI			||
      tag_id == TAG_HR			||
      tag_id == TAG_P			||
      tag_id == TAG_PRE			||
      tag_id == TAG_TABLE		||
      tag_id == TAG_BLOCKQUOTE)
    return true;
  return false;
}

#endif
