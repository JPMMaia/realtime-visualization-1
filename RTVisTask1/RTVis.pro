QT       += core gui widgets

TARGET = RTVisTask1
TEMPLATE = app

SOURCES += main.cpp

SOURCES += \
    Mainwidget.cpp

HEADERS += \
    Mainwidget.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target