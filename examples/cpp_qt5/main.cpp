#include <iostream>
#include <markdownpanda/qt.hpp>

int main()
{
    MarkdownPandaQt panda;
    panda.loadHtmlFile(":/index.html");
    // panda.loadHtmlString("<p>You could also use a QString like this</p>");
    QString markdown = panda.markdown();

    std::cout << markdown.toStdString();
}
