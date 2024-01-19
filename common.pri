gcc:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
msvc:QMAKE_CXXFLAGS_WARN_ON += -wd4100

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# This crap lets us run files with the same name, in the same project, through moc, without conflicts.
# Good idea? Probably not.

CONFIG += object_parallel_to_source
CONFIG += no_batch # above doesn't properly work with nmake without this

defineReplace(fnPath) {
    variable = $$1
    variable = $$replace(variable, $$basename(variable), "")
    variable = $$replace(variable, "\\\\", "_")
    variable = $$replace(variable, "\.", "_")
    variable = $$replace(variable, "/", "_")
    return($$variable)
}

QMAKE_H_MOD_MOC = moc_${QMAKE_FUNC_fnPath}
QMAKE_CPP_MOD_MOC = moc_${QMAKE_FUNC_fnPath}
QMAKE_MOD_RCC = qrc${QMAKE_FUNC_fnPath}

# But I'm going with it anyway.

INCLUDEPATH += src

SOURCES += $$files(src/parser/*.cpp, true)
SOURCES += $$files(src/parser/*.c, true)
HEADERS += $$files(src/parser/*.h, true)
