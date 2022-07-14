CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Algorithm.cpp \
    DRSA.cpp \
    DynamicMath.cpp \
    DynamicMong.cpp \
    DynamicPrime.cpp \
    DynamicUInt.cpp \
    __drsa_global.cpp

HEADERS += \
    Algorithm.h \
    DRSA.h \
    DynamicMath.h \
    DynamicMong.h \
    DynamicPrime.h \
    DynamicUInt.h \
    __drsa_global.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += C:/DothProject/DTL/
