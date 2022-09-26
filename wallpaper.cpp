#include "wallpaper.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QApplication>
#include <QScreen>
#ifdef Q_OS_LINUX
#include <QtX11Extras/QX11Info>
#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>
#include <QtPlatformHeaders/QXcbWindowFunctions>
#endif
#include <QApplication>
#include <QPainter>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QDesktopWidget>
#include <QDebug>
#include "application.h"


#include <QWindow>

#ifdef Q_OS_WINDOWS
#include <windows.h>

WId viewId;

LRESULT CALLBACK HookShoot(_In_ int nCode, _In_ WPARAM wParam,LPARAM lParam){
    if(wParam == WM_MOUSEMOVE || wParam == WM_NCMOUSEMOVE){
         MOUSEHOOKSTRUCT * mshook = (MOUSEHOOKSTRUCT *)lParam;
         PostMessage((HWND)viewId,WM_MOUSEMOVE,0,MAKELPARAM(mshook->pt.x,mshook->pt.y));
    };
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

HWND _WORKERW = nullptr;

inline BOOL CALLBACK EnumWindowsProc(_In_ HWND tophandle, _In_ LPARAM topparamhandle)
{
    HWND defview = FindWindowEx(tophandle, 0, L"SHELLDLL_DefView", nullptr);
    if (defview != nullptr)
    {
        _WORKERW = FindWindowEx(0, tophandle, L"WorkerW", 0);
    }
    return true;
}



HWND GetWorkerDesktop(){
    int result;
    HWND windowHandle = FindWindow(L"Progman", nullptr);
    //使用 0x3e8 命令分割出两个 WorkerW
    SendMessageTimeout(windowHandle, 0x052c, 0 ,0, SMTO_NORMAL, 0x3e8,(PDWORD_PTR)&result);
    //遍历窗体获得窗口句柄
    EnumWindows(EnumWindowsProc,(LPARAM)nullptr);
    // 显示WorkerW
    ShowWindow(_WORKERW,SW_HIDE);
    return windowHandle;
}
#endif

Wallpaper::Wallpaper(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    setLayout(layout);

    registerDesktop();

    m_mpv = new MpvWidget(this);

    m_mpv->setGeometry(geometry());
    layout->addWidget(m_mpv);
    m_mpv->hide();

    m_mpv->setProperty("loop", true);

    connect(qApp->desktop(), &QDesktopWidget::resized, this, &Wallpaper::updateGeometry);
    QTimer::singleShot(1, this, &Wallpaper::updateGeometry);

    QTimer::singleShot(100, this, [=] {
        int index=0;
    for (const QString &arg : qApp->arguments()) {
        if(index!=0){
            if(QFileInfo(arg).isFile()){
                setFile(arg);
                play();
            }
        }
        index++;
    }
    });
//    QTimer::singleShot(100, this, [=] {
//        setFile("/home/lmh/Desktop/动态壁纸素材/12.mp4");
//        play();
//    });
connect(dApp,&Application::setPlayPath,this,&Wallpaper::setFile);
connect(dApp,&Application::setMpvPlay,this,&Wallpaper::play);
connect(dApp,&Application::setMpvpause,this,&Wallpaper::pause);
connect(dApp,&Application::setMpvstop,this,&Wallpaper::stop);
connect(dApp,&Application::setMpvVolume,this,&Wallpaper::setVolume);

}

void Wallpaper::setFile(const QString &path)
{
    m_mpv->show();
    m_mpv->command(QStringList() << "loadfile" << path);
    m_mpv->setProperty("pause", true);
}

void Wallpaper::setVolume(const qint32 volume)
{
    m_mpv->setProperty("volume", volume);
}

void Wallpaper::clear()
{
    stop();
    hide();
}

void Wallpaper::play()
{
    m_mpv->show();
    m_mpv->setProperty("pause", false);
}

void Wallpaper::pause()
{
    m_mpv->setProperty("pause", true);
}

void Wallpaper::stop()
{
    m_mpv->setProperty("stop", true);
}

void Wallpaper::registerDesktop()
{
#ifdef Q_OS_LINUX
    this->winId();
    QWindow *window = this->windowHandle();
    window->setOpacity(0.99);
    if (QApplication::platformName() == "wayland") {
        qDebug() << "wayland set desktop";
    } else {
        QXcbWindowFunctions::setWmWindowType(window, QXcbWindowFunctions::Desktop);
    }

    QTimer::singleShot(1, this, [=] {
        show();
        lower();
    });
#else
    workerW =  GetWorkerDesktop();

    if(this){
        viewId = this->winId();
        // 返回workerW窗口句柄

        //设置窗口样式
        GetWindowLongA((HWND)viewId, GWL_STYLE);
        //设置壁纸窗体的父窗体
        SetParent((HWND)viewId,workerW);
        SetWindowPos((HWND)viewId,HWND_TOP,0,0,0,0,WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR|WS_EX_NOACTIVATE);
        // 设置全局鼠标事件钩子
//        hook = SetWindowsHookEx(WH_MOUSE_LL,HookShoot,GetModuleHandle(NULL),0);

        QDesktopWidget *desktop = QApplication::desktop();
        this->move(QPoint(0,0));
        int height = desktop->height();
        int width = desktop->width();
        this->resize(QSize(width,height));
        this->showFullScreen();
    }

#endif


}

void Wallpaper::updateGeometry()
{
    QTimer::singleShot(100, this, [=] {
        const QRect &rec = qApp->desktop()->screenGeometry(qApp->desktop()->primaryScreen());
        setGeometry(rec);
        m_mpv->move(rect().topLeft());
        m_mpv->setFixedSize(size());
        lower();
    });
}
