#ifndef PRINTDIFF_H
#define PRINTDIFF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>
#include <helper.h>

////////////////
// PUBLIC API //
////////////////
void printdiff(char *s1_label, char *s2_label, char *s1, char *s2);

////////////////////
// Implementation //
////////////////////

#define DIFF_FILE_PADDING 5 /* Amount of characters to show
			     * before and after a printdiff
			     * output */

// Terminal Colors
#define COLOR_NORMAL  "\x1B[0m"
#define COLOR_RED  "\x1B[31m"
#define COLOR_GREEN  "\x1B[32m"

char *string_new(char *str)
{
  char *new = malloc( strlen(str)+1 );
  strcpy(new, str);
  return new;
}

char *string_new_from_char(char c)
{ // Warning: This function will turn '\n' to ' '
  char *str = malloc(2);
  if ( c != '\n')
    str[0] = c;
  else
    str[0] = ' ';
  str[1] = '\0';
  return str;
}

char *string_new_from_size(int strsize)
{
  char *new = malloc( strsize+1 );
  return new;
}

char *string_cat(char *s1, char *s2)
{
  char *new = malloc( strlen(s1)+strlen(s2)+1);
  strcpy( new, s1 );
  strcat( new, s2 );
  free(s1);
  free(s2);
  return new;
}

char *string_new_with_color(char *color, char *str)
{
  char *c = string_new(color);
  char *n = string_new(COLOR_NORMAL);
  char *s = string_new(str);
  char *str1 = string_cat(c, s);
  char *str2 = string_cat(str1, n);
  return str2;
}

char *string_cat_with_color(char *color, char *str1, char *str2)
{
  char *str2_color = string_new_with_color(color, str2);
  free(str2);
  char *new = string_cat(str1, str2_color);
  return new;
}


char *string_new_from_range_of_string(char *str, int min, int max)
{
  int newStrSize =  max-min > 0 ? max-min : 0;
  char *newStr = string_new_from_size( newStrSize );
  int newStr_len = 0;
  for (int i = min; i <= max; i++) {
    if ( str[i] != '\n' && str[i] != '\r') {
      newStr[newStr_len] = str[i];
      newStr_len++;
    }
  }
  return newStr;
}

void printdiff(char *s1_label, char *s2_label, char *s1, char *s2)
{
  int s1_len = strlen(s1);
  int s2_len = strlen(s2);
  /*
   * keep it simple for now and only loop through
   * shortest string.
   */
  int len = s1_len < s2_len ? s1_len : s2_len;

  boolean inDifferentPart = False;
  char *diffBuffer1 = malloc(1); diffBuffer1[0] = '\0';
  char *diffBuffer2 = malloc(1); diffBuffer2[0] = '\0';

  cr_log_info("\tSummary");
  cr_log_info("\t=======");

  for (int i = 0; i < len; i++) {
    if ( s1[i] == s2[i]) {
      if ( inDifferentPart ) {
	// Padding right
	int max = i+1+DIFF_FILE_PADDING < len ? i+1+DIFF_FILE_PADDING : len-1;
	char *charPadding1 = string_new_from_range_of_string(s1, i+1, max);
	char *charPadding2 = string_new_from_range_of_string(s2, i+1, max);
	diffBuffer1 = string_cat_with_color(COLOR_GREEN, diffBuffer1, charPadding1);
	diffBuffer2 = string_cat_with_color(COLOR_GREEN, diffBuffer2, charPadding2);

	cr_log_info("\t[%s]: %s\n\t[%s]: %s\n\n", s1_label, diffBuffer1, s2_label, diffBuffer2);

	inDifferentPart = False;
	free(diffBuffer1); free(diffBuffer2);
	diffBuffer1 = string_new("");
        diffBuffer2 = string_new("");
      }
    }
    else if ( s1[i] != s2[i] ) {
      // Padding Left
      if (!inDifferentPart) {
	int min = i-1-DIFF_FILE_PADDING > 0 ? i-1-DIFF_FILE_PADDING : 0;
	char *charPadding1 = string_new_from_range_of_string(s1, min, i-1);
	char *charPadding2 = string_new_from_range_of_string(s2, min, i-1);

	diffBuffer1 = string_cat_with_color(COLOR_GREEN, diffBuffer1, charPadding1);
	diffBuffer2 = string_cat_with_color(COLOR_GREEN, diffBuffer2, charPadding2);
      }

      // Center text
      char *charStr1 = string_new_from_char(s1[i]);
      char *charStr2 = string_new_from_char(s2[i]);
      diffBuffer1 = string_cat_with_color(COLOR_RED, diffBuffer1, charStr1);
      diffBuffer2 = string_cat_with_color(COLOR_RED, diffBuffer2, charStr2);
      inDifferentPart=True;
    }
  }
  if (criterion_options.logging_threshold != CRITERION_IMPORTANT) {
    puts("Full Files");
    puts("==========");
    printf("%s\n%s", s1_label, s1);
    printf("\n%s\n%s\n", s2_label, s2);
  }
}

#endif
