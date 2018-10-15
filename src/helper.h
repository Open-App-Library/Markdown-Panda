#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markdownpanda.h"

typedef enum { False, True } boolean;

static void removeWhitespace(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if( *i != ' ' && *i != '\n' && *i != '\t')
      i++;
  }
  *i = 0;
}

static boolean string_equals(const char *s1, const char *s2)
{
  if ( strcmp(s1, s2) == 0)
    return true;
  return false;
}

static char* string_append(const char *s1, const char *s2)
{
  char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

static char* string_prepend(const char *s1, const char *s2)
{
  return string_append(s2, s1);
}

static char* string_wrap2(const char *s1, const char *start_wrap, const char *end_wrap)
{
  return string_append( string_prepend(s1, start_wrap), end_wrap);
}

static char* string_wrap(const char *s1, const char *wrap)
{
  return string_wrap2(s1, wrap, wrap);
}

static char *file_to_string(char *filename) {
  char * buffer = 0;
  long length;
  FILE * f = fopen (filename, "rb");

  if (f) {
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    buffer = malloc (length);
    if (buffer)
      {
	fread (buffer, 1, length, f);
      }
    fclose (f);
  }

  if (buffer) {
    return buffer;
  }
  return NULL;
}

#endif
