#ifndef MARKDOWNPANDA_CPP_HPP
#define MARKDOWNPANDA_CPP_HPP

#include <iostream>
#include <string.h>
#include "markdownpanda.h"

class MarkdownPanda {

public:
  MarkdownPanda();
  MarkdownPanda(int STRING_TYPE__HTML_OR_MARKDOWN, std::string str);

  bool loadHtmlString(std::string html_string);
  bool loadMarkdownString(std::string markdown_string);
  bool loadHtmlFile(std::string filename);
  bool loadMarkdownFile(std::string filename);

  std::string markdown();
  std::string html();

private:
  std::string m_html_string;
  std::string m_markdown_string;
  bool toMarkdown(HtmlObject obj); // Parses m_html_string into m_markdown_string
};

MarkdownPanda::MarkdownPanda() {}

MarkdownPanda::MarkdownPanda(int STRING_TYPE__HTML_OR_MARKDOWN, std::string str)
{
  switch (STRING_TYPE__HTML_OR_MARKDOWN) {
  case TYPE_HTML:
    m_html_string = str;
    loadHtmlString(m_html_string);
    break;
  case TYPE_MARKDOWN:
    m_markdown_string = str;
    loadMarkdownString(m_markdown_string);
    break;
  }
}

bool MarkdownPanda::loadHtmlString(std::string html_string)
{
  char *html_cstring = strdup( html_string.c_str() );
  HtmlObject obj = load_html_from_string( html_cstring );
  bool result = toMarkdown( obj );
  delete html_cstring;
  destroy_html_object( obj );
  return result;
}

bool MarkdownPanda::loadMarkdownString(std::string markdown_string)
{
  std::cout << "loadMarkdownString function not yet implemented" << std::endl;
  return false;
}

bool MarkdownPanda::loadHtmlFile(std::string filename)
{
  char *filename_cstring = strdup( filename.c_str() );
  HtmlObject obj = load_html_from_file( filename_cstring );
  bool result = toMarkdown( obj );
  delete filename_cstring;
  destroy_html_object( obj );
  return result;
}

bool MarkdownPanda::loadMarkdownFile(std::string filename)
{
  std::cout << "loadMarkdownFile function not yet implemented" << std::endl;
  return false;
}

bool MarkdownPanda::toMarkdown(HtmlObject obj)
{
  m_markdown_string = mdpanda_to_markdown( obj );

  if ( m_markdown_string == HTML_FILE_NOT_FOUND_MESSAGE )
    return false;
  return true;  
}

std::string MarkdownPanda::markdown()
{
  return m_markdown_string;
}

std::string MarkdownPanda::html()
{
  return m_html_string;
}

#endif
