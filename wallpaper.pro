TEMPLATE = app

QT += gui widgets dbus
CONFIG += c++11 link_pkgconfig
LIBS+=-luser32
linux:PKGCONFIG += mpv

TARGET = simple-wallpaper

SOURCES += main.cpp \
    wallpaper.cpp \
    dbuswallpaperservice.cpp \
    mpvwidget.cpp \
    settingwindow.cpp \
    application.cpp \
    moviepreview.cpp

HEADERS += \
    wallpaper.h \
    dbuswallpaperservice.h \
    mpvwidget.h \
    settingwindow.h \
    application.h \
    moviepreview.h

RESOURCES += \
    qrc.qrc

target.path = /usr/bin/

icon.path=/usr/share/icons
icon.files= $$PWD/install/simple-wallpaper.png

desktop.path = /usr/share/applications/
desktop.files = $$PWD/install/simple-wallpaper.desktop

INSTALLS += target icon desktop

FORMS += \
    settingwindow.ui
RC_FILE += $$PWD/app.rc
#win32: LIBS += -L$$PWD/3rd/ -llibmpv.dll

INCLUDEPATH += $$PWD/3rd
INCLUDEPATH += $$PWD/3rd/mpv
DEPENDPATH += $$PWD/3rd
#LIBS+=D:/LMH/mpv/mpv-examples-master/mpv-examples-master/libmpv/qt_opengl/lib/libmpv.dll.a
#win32:!win32-g++: LIBS += $$PWD/3rd/libmpv.dll.lib
win32: LIBS += $$PWD/3rd/libmpv.dll.a
