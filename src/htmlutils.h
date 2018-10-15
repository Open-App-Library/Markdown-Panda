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

int get_tag_id(char *tag) {
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
  return 0;
}

#endif
