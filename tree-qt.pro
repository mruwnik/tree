#-------------------------------------------------
#
# Project created by QtCreator 2013-10-10T18:51:53
#
#-------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tree-qt
TEMPLATE = app
LIBS+=-lGLEW -lGLU -lpng



SOURCES += main.cpp\
        mainwindow.cpp \
    bud.cpp \
    leaf.cpp \
    objloader.cpp \
    parts.cpp \
    segment.cpp \
    tree.cpp \
    glade.cpp \
    texture.cpp \
    glsl.cpp \
    weather.cpp \
    treeparams.cpp

HEADERS  += mainwindow.h \
    bud.h \
    leaf.h \
    objloader.h \
    parts.h \
    segment.h \
    tree.h \
    glade.h \
    texture.h \
    glsl.h \
    weather.h \
    treeparams.h

FORMS    += mainwindow.ui

OTHER_FILES +=

RESOURCES += \
    envTest.qrc

install_it.path = /home/dan
install_it.files += leaf.vert leaf.frag

INSTALLS += install_it

QMAKE_EXTRA_TARGETS += install
