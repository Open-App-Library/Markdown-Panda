# START - Default QMake Settings
QT -= gui
CONFIG += c++11 console
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
# END - Default QMake Settings


# Important Configuration
SOURCES += \
        main.cpp

HEADERS += \
        $$PWD/../../build/include/markdownpanda/qt.hpp \
        $$PWD/../../build/include/markdownpanda/cpp.hpp

INCLUDEPATH += $$PWD/../../build/include

# Add MarkdownPanda and MyHTML
LIBS += -L $$PWD/../../build/lib -lmarkdownpanda_static -lmyhtml_static -lcmark-gfm-extensions -lcmark-gfm

# Add PThread Support
QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread

RESOURCES += \
    resources.qrc
