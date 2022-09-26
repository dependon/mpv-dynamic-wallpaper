#ifndef WALLPAPER_H
#define WALLPAPER_H

#include "mpvwidget.h"

#include <QWidget>
#include <QVBoxLayout>
#ifdef Q_OS_WINDOWS
#include <windows.h>
#endif
class Wallpaper : public QWidget
{
    Q_OBJECT
public:
    explicit Wallpaper(QWidget *parent = nullptr);

public slots:
    void setFile(const QString &path);
    void setVolume(const qint32 volume);
    void clear();
    void play();
    void pause();
    void stop();

private:
    void registerDesktop();
    void updateGeometry();

private:
    QVBoxLayout *mainlayout;
    MpvWidget *m_mpv;
#ifdef Q_OS_WINDOWS
    HHOOK hook;
    HWND workerW;
#endif
};

#endif // WALLPAPER_H
