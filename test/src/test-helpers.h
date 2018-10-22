#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <markdownpanda.h>
#include <helper.h>

#define DIFF_FILE_PADDING 5 /* Amount of characters to show 
			     * before and after a printdiff
			     * output */
#define MD_LOCATION "../test/md/"
#define HTML_LOCATION "../test/html/"

// Terminal Colors
#define NORM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"

typedef struct {
  char *md;
  char *html;
  HtmlObject htmlObject;
} text_t;

text_t load(char *type)
{
  char *md_file = string_append(MD_LOCATION, string_append(type, ".md"));
  char *html_file = string_append(HTML_LOCATION, string_append(type, ".html"));
  char *md = load_markdown_from_file( md_file );
  char *html = file_to_string( html_file );
  HtmlObject htmlObject = load_html_from_string( html );
  text_t t = {md, html, htmlObject};
  return t;
}

char *charRangeToString_skipnewlines(char *str, int min, int max)
{
  char *newStr = malloc( sizeof(char) * (max - min) );
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
  char *diffBuffer1 = "";
  char *diffBuffer2 = "";

  puts("\tSummary");
  puts("\t=======");

  for (int i = 0; i < len; i++) {
    if ( s1[i] == s2[i]) {
      if ( inDifferentPart ) {
	int s1_label_len = strlen(s1_label);
	int s2_label_len = strlen(s2_label);
	printf("\t[%s]: %s\n\t[%s]: %s\n\n", s1_label, diffBuffer1, s2_label, diffBuffer2);
	diffBuffer1 = "";
	diffBuffer2 = "";
      }
      inDifferentPart = False;
    }
    else if ( s1[i] != s2[i] ) {
      if (!inDifferentPart) {
	int min = i-1-DIFF_FILE_PADDING > 0 ? i-1-DIFF_FILE_PADDING : 0;
	char *b1 = charRangeToString_skipnewlines(s1, min, i-1);
	char *b2 = charRangeToString_skipnewlines(s2, min, i-2);
	char *grnb1 = string_append(GRN, b1);
	char *grnb2 = string_append(GRN, b2);
	string_append_safe(diffBuffer1, grnb1);
	string_append_safe(diffBuffer2, grnb2);
	string_append_safe(diffBuffer1, NORM);
	string_append_safe(diffBuffer2, NORM);
	free(b1);
	free(b2);
	free(grnb1);
	free(grnb2);
      }
      char b1[2] = "\0"; b1[0] = s1[i];
      char b2[2] = "\0"; b2[0] = s2[i];
      diffBuffer1 = string_append(diffBuffer1, string_append(RED, b1));
      diffBuffer2 = string_append(diffBuffer2, string_append(RED, b1));
      diffBuffer1 = string_append(diffBuffer1, NORM);
      diffBuffer2 = string_append(diffBuffer2, NORM);
      inDifferentPart=True;
    }
  }
  puts("Full Files");
  puts("==========");
  printf("%s\n%s", s1_label, s1);
  printf("\n%s\n%s\n", s2_label, s2);
}

#endif

