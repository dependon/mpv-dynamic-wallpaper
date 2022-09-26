#include <QObject>

#include "application.h"
#include "wallpaper.h"
#include "dbuswallpaperservice.h"
#include <malloc.h>
#include "settingwindow.h"
#include <QMainWindow>


int main(int argc, char *argv[])
{
#ifdef Q_OS_LINUX
    mallopt(M_ARENA_MAX, 1);
    setlocale(LC_NUMERIC, "C");
#endif
    Application a(argc, argv);

    if (true) {
        a.setApplicationName(QObject::tr("Deepin DreamScene"));
        a.setApplicationVersion("Version 0.1");

        Wallpaper *w = new Wallpaper;
        DBusWallpaperService *dbusInter = new DBusWallpaperService(w);
        Q_UNUSED(dbusInter);

        QDBusConnection::sessionBus().registerService("com.github.wallpaper.fantascene");
        QDBusConnection::sessionBus().registerObject("/com/github/wallpaper/fantascene", "com.github.wallpaper.fantascene", w);

        settingWindow *window=new settingWindow();

        window->show();

    }

    return a.exec();
}


