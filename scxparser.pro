include(common.pri)

TARGET = scxparser
TEMPLATE = app
CONFIG += console
QMAKE_CXXFLAGS += -std=c++17 -lstdc++fs
SOURCES += src/SCXParser.cpp
SOURCES += $$files(src/parser/*.cpp, true)
SOURCES += $$files(src/parser/*.c, true)
HEADERS += $$files(src/parser/*.h, true)