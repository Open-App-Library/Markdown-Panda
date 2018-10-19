#ifndef MARKDOWNPANDA_QT_HPP
#define MARKDOWNPANDA_QT_HPP

#include <QString>
#include <QFile>
#include "cpp.hpp"

class MarkdownPandaQt : MarkdownPanda
{
public:
  MarkdownPandaQt();
  MarkdownPandaQt(int STRING_TYPE__HTML_OR_MARKDOWN, QString str);

  bool loadHtmlString(QString html_string);
  bool loadMarkdownString(QString markdown_string);
  bool loadHtmlFile(QString filename);
  bool loadMarkdownFile(QString filename);

  QString markdown();
  QString html();
private:
  QString file2QString(QString filename);
};

MarkdownPandaQt::MarkdownPandaQt() : MarkdownPanda() {

}

MarkdownPandaQt::MarkdownPandaQt(int STRING_TYPE__HTML_OR_MARKDOWN, QString str) :
  MarkdownPanda(STRING_TYPE__HTML_OR_MARKDOWN, str.toStdString())
{
}

bool MarkdownPandaQt::loadHtmlString(QString html_string)
{
  MarkdownPanda *p = this;
  return p->loadHtmlString( html_string.toStdString() );
}
bool MarkdownPandaQt::loadMarkdownString(QString markdown_string)
{
  MarkdownPanda *p = this;
  return p->loadMarkdownString( markdown_string.toStdString() );
}
bool MarkdownPandaQt::loadHtmlFile(QString filename)
{
  QString html = this->file2QString(filename);
  MarkdownPanda *p = this;
  return p->loadHtmlString( html.toStdString() );
}
bool MarkdownPandaQt::loadMarkdownFile(QString filename)
{
  QString markdown = this->file2QString(filename);
  MarkdownPanda *p = this;
  return p->loadMarkdownString( markdown.toStdString() );
}

QString MarkdownPandaQt::markdown()
{
  MarkdownPanda *p = this;
  return QString::fromUtf8( p->markdown().c_str() );
}

QString MarkdownPandaQt::html()
{
  MarkdownPanda *p = this;
  return QString::fromUtf8( p->html().c_str() );
}

QString MarkdownPandaQt::file2QString(QString filename)
{
    QString val;
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    return val;
}

#endif
