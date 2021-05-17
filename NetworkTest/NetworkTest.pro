QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

TARGET = AccessServer

TEMPLATE = app

DESTDIR += $$PWD/Bin

include($(GLDRS)/Glodon/shared/GLDStaticLib.pri)
include($(GLDRS)/Glodon/shared/GSPStaticLib.pri)
include($(GLDRS)/Glodon/shared/GSCNew.pri)
include($(GLDRS)/Glodon/shared/GSCRSARefGLDStaticLib.pri)
include($(GLDRS)/Glodon/shared/GSPKeyLib.pri)
include($(GLDRS)/ThirdPart/VMProtect/VMProtect.pri)

INCLUDEPATH += \
    Proj/Include/Core \
    Proj/Include/Frame \
    Proj/Include/Utils \

SOURCES += \
 Proj/Source/Core/AccessService.cpp \
 Proj/Source/Core/MainController.cpp \
 Proj/Source/Frame/Widget.cpp \
 Proj/Source/Utils/GCoreHttpService.cpp \
 Proj/main.cpp

HEADERS += \
 Proj/Include/Core/AccessService.h \
 Proj/Include/Core/MainController.h \
 Proj/Include/Frame/Widget.h \
 Proj/Include/Utils/GBQServerConsts.h \
 Proj/Include/Utils/GCoreHttpService.h \
 Proj/Include/Utils/GLDProject.h
