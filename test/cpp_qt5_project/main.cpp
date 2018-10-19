#include <iostream>

// C++ Include
#include <markdownpanda/cpp.hpp>

// Qt5 include
#include <markdownpanda/qt.hpp>

void cpp_example()
{
    MarkdownPanda panda;

    /// Converting HTML to Markdown
    panda.loadHtmlString("<h1>Welcome to MarkdownPanda</h1>");
    //panda.loadHtmlFile("index.html");
    std::cout << panda.markdown();

    /// Converting Markdown to HTML
    panda.loadMarkdownString("## I hope you enjoy this library");
    //panda.loadMarkdownFile("index.md");
    std::cout << panda.html();
}

void qt_example()
{
    MarkdownPandaQt panda;

    // Converting HTML to Markdown
    panda.loadHtmlFile(":/index.html");
    // panda.loadHtmlString("<p>You could also use a QString like this</p>");
    std::cout << panda.markdown().toStdString();

    // Converting Markdown to HTML
    panda.loadMarkdownFile(":/index.md");
    // panda.loadHtmlString("You could also use a **QString** like this.");
    std::cout << panda.html().toStdString();
}

int main()
{
    cpp_example();
    std::cout << "\n";
    qt_example();
}
