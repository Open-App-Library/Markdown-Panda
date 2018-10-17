#include <stdio.h>
#include "helper.h"
#include "markdownpanda.h"
#include <time.h>

boolean showParseTime = False;

int main(int argc, char **argv)
{
  if (argc < 2) {
    printf("Usage: mdpanda [filename.html] [-t (Will show parse time as HTML comment)] \n");
    printf("Converts HTML file to markdown.\n");
    exit( EXIT_FAILURE );
  } else if (argc > 2) {
    if (strcmp(argv[2], "-t") == 0)
      showParseTime = True;
  }
  
  clock_t begin = clock(); // Get start time

  // Load HTML file and convert it to
  // markdown in four lines of code. 
  char *filename = argv[1];
  HtmlObject html = load_html_from_file( filename );
  char *toMarkdown = mdpanda_to_markdown( html );
  printf("%s", toMarkdown);
  destroy_html_object( html );

  clock_t end = clock();   // Get end time
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  if ( showParseTime )
    printf("\n<!-- Generated markdown in %f seconds. -->\n", time_spent);

  return 0;
}
