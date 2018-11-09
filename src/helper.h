#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "markdownpanda.h"

typedef enum { False = 0, True = 1 } boolean;

static char *trimWhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  strtok(str, "\n");

  return str;
}

static void trimTrailingNewlines(char *str)
{
  for (int i = strlen(str)-1; i >= 0; i--) {
    if ( str[i] == '\n' ){
      str[i] = '\0';
    } else break;
  }
}

static boolean string_equals(const char *s1, const char *s2)
{
  if ( strcmp(s1, s2) == 0)
    return True;
  return False;
}

static char *string_append(const char *s1, const char *s2)
{
  // puts("WARNING: string_append is deprecated. Please use string_append_safe instead.");
  char * new_str;
  if((new_str = malloc(strlen(s1)+strlen(s2)+1)) != NULL){
    new_str[0] = '\0';   // ensures the memory is an empty string
    strcat(new_str,s1);
    strcat(new_str,s2);
  } else {
    fprintf(stderr,"malloc failed!\n");
    exit( EXIT_FAILURE );
  }
  return new_str;
}

static void string_append2(char *s1, const char *s2)
{
	/* printf("s1=\"%s\" s2=\"%s\" Resizing a %lu to %lu\n", s1,s2, strlen(s1)+1, strlen(s1)+strlen(s2)+1); */
  char *ptr = (char*) realloc(s1, strlen(s1)+strlen(s2)+1 );
  if ( ptr != NULL )
    strcat(s1, s2);
}

static void string_appendc(char *s, const char c) {
	string_append2(s, " ");
	s[ strlen(s)-1 ] = c;
}

static char *string_append_safe(char *s1, char *s2)
{
  char *new_str ;
  if((new_str = malloc(strlen(s1)+strlen(s2)+1)) != NULL){
    new_str[0] = '\0';   // ensures the memory is an empty string
    strcat(new_str,s1);
    strcat(new_str,s2);
    free(s1);
    free(s2);
  } else {
    fprintf(stderr,"malloc failed!\n");
    exit( EXIT_FAILURE );
  }
  return new_str;
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
  char *buffer = 0;
  long length;
  FILE *f = fopen (filename, "rb");

  if(f == NULL) {
    // Optionally print error message
    // fprintf(stderr, "Can't open file: %s\n", filename);
    return NULL;
  } else {
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    buffer = malloc(length+1);
    if (buffer) {
      fread (buffer, 1, length, f);
    }
    fclose (f);
  }

  buffer[length] = '\0';

  if (buffer) {
    return buffer;
  }
  return NULL;
}

#endif
