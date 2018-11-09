#include <stdio.h>
#include "helper.h"
#include "markdownpanda.h"
#include <time.h>

// CLI Options
boolean   option_showParseTime = False;
char **filesToProcess; // Array of HTML & markdown files user provides to be converted
int filesToProcess_count;

const char *get_filename_extension_from_string(const char *filename) {
  char filename_lower[ sizeof(char) * strlen(filename) ];
  strcpy(filename_lower, filename);
  for(int i = 0; filename_lower[i]; i++){
    filename_lower[i] = tolower(filename_lower[i]);
  }
  const char *dot = strrchr(filename, '.');
  if(!dot || dot == filename) return "";
  return dot;
}

void print_filename_header(char *filename)
{
  puts("");
  puts  ("==============================");
  printf(" %s\n", filename);
  puts  ("==============================");
  puts("");
}

void display_help_menu_and_exit()
{
  puts("Usage: mdpanda [filename] [options] ...");
  puts("Converts HTML or markdown files to markdown or HTML.");
  puts("Options:");
  puts("\t\"-t\" - Display the total parse time as an HTML comment");
  puts("\t\"-std\" - Print the output to the terminal.\n\t\tNote: Does not work for multiple files");
  exit( 0 );
}

void parse_cli_args(int argc, char **argv)
{
  if (argc < 2)
    display_help_menu_and_exit();

  for (int i = 1; i < argc; i++) {

    if (strcmp(argv[i], "-t") == 0)
      option_showParseTime = True;
    else {
      char **new_filesToProcess = malloc( sizeof(char*) * (filesToProcess_count + 1) );
      for (int f = 0; f < filesToProcess_count; f++)
	new_filesToProcess[f] = filesToProcess[f];
      new_filesToProcess[ filesToProcess_count ] = argv[i];
      free(filesToProcess );
      filesToProcess = new_filesToProcess;
      filesToProcess_count++;
    }

  }

}

int main(int argc, char **argv)
{
  parse_cli_args(argc, argv);

  clock_t begin = clock(); // Get start time
  for (int i = 0; i < filesToProcess_count; i++) {

    char *filename = filesToProcess[i];
    const char *filename_extension = get_filename_extension_from_string(filename);

    if (filesToProcess_count > 1)
      print_filename_header(filename);

    if ( string_equals(filename_extension, ".html")) {
      HtmlObject html = load_html_from_file( filename );
      char *toMarkdown = mdpanda_to_markdown( html );
      printf("%s", toMarkdown);
      destroy_html_object( html );
    } else if ( string_equals(filename_extension, ".md")) {
      char *text = file_to_string(filename);
      char *toHtml = mdpanda_to_html(text);
      printf("%s", toHtml);
      free(toHtml);
    } else {
      printf("Invalid file extension %s\n", filename_extension);
    }

  }
  clock_t end = clock();   // Get end time

  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  if ( option_showParseTime )
    printf("\n<!-- Generated markdown in %f seconds. -->\n", time_spent);

  return 0;
}
