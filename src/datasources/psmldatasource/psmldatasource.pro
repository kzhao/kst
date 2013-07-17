TOPOUT_REL=../../..
include($$PWD/$$TOPOUT_REL/kst.pri)
include($$PWD/../../../datasourceplugin.pri)

TARGET = $$kstlib(kst2data_psmldatasource)
INCLUDEPATH += $$OUTPUT_DIR/src/datasources/psmldatasource/tmp

SOURCES += \
    psmldatasource.cpp

HEADERS += \
    psmldatasource.h
