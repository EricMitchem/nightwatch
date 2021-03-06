/******************************************************************************
* MIT License
*
* Copyright (c) 2017 Eric Mitchem
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************/

#include <cassert>
#include <Windows.h>
#include <PowrProf.h>
#include <QApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include "aboutdialog.hpp"
#include "application.hpp"
#include "systemtrayicon.hpp"
#include "window.hpp"

static unsigned int SuspendCountdown = 5;

Application* Application::_instance = nullptr;

Application::Application()
{
    assert(Application::_instance == nullptr);
    Application::_instance = this;
}

Application::~Application()
{
    assert(Application::_instance == this);
    Application::_instance = nullptr;
}

int Application::run(int argc, char** argv)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    int zero = 0;
    QApplication app(zero, nullptr);

    QTimer::singleShot(0, this, &Application::startup);

    return app.exec();
}

void Application::turn_off_displays()
{
    HWND   handle      = (HWND) this->_window->winId();
    LPARAM display_off = 2;

    SendMessageW(handle, WM_SYSCOMMAND, SC_MONITORPOWER, display_off);
}

void Application::suspend_computer()
{
    BOOLEAN result = SetSuspendState(FALSE, FALSE, FALSE);

    if(result == 0)
    {
        DWORD  err_code = GetLastError();
        LPWSTR msg;

        DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
                      FORMAT_MESSAGE_FROM_SYSTEM     |
                      FORMAT_MESSAGE_IGNORE_INSERTS  ;

        FormatMessageW(
                    flags,
                    NULL,
                    err_code,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPWSTR) &msg,
                    0,
                    NULL);

        QString err_str = QString::fromWCharArray(msg);

        LocalFree(msg);

        QMessageBox::warning(this->_window, "Error", err_str);
    }
}

Application* Application::instance()
{
    return Application::_instance;
}

Window* Application::window()
{
    return Application::instance()->_window;
}

QString Application::version()
{
    return QString("%1.%2.%3").arg(version_major).arg(version_minor).arg(version_patch);
}

void Application::about_nightwatch()
{
    this->_about->show();
}

void Application::quit()
{
    qApp->quit();
}

void Application::startup()
{
    qApp->setQuitOnLastWindowClosed(false);

    this->_window = new Window;
    this->_icon   = new SystemTrayIcon;
    this->_about  = new AboutDialog(this->_window);

    emit starting_up();
    emit started();

    connect(qApp, &QApplication::aboutToQuit, this, &Application::shutdown);

    auto display_timer = new QTimer(this);
    auto suspend_timer = new QTimer(this);

    display_timer->setObjectName("display_timer");
    suspend_timer->setObjectName("suspend_timer");

    connect(display_timer, &QTimer::timeout, this, &Application::update_display_button);
    connect(suspend_timer, &QTimer::timeout, this, &Application::update_suspend_button);

    auto display_button = this->_window->findChild<QPushButton*>("display_button");
    connect(display_button, &QPushButton::clicked, this, &Application::display_button_clicked);

    auto suspend_button = this->_window->findChild<QPushButton*>("suspend_button");
    connect(suspend_button, &QPushButton::clicked, this, &Application::suspend_button_clicked);
}

void Application::shutdown()
{
    emit shutting_down();

    delete this->_about;
    this->_about = nullptr;

    delete this->_icon;
    this->_icon = nullptr;

    delete this->_window;
    this->_window = nullptr;
}

void Application::display_button_clicked()
{
    auto display_timer  = this->findChild<QTimer*>("display_timer");
    auto display_button = this->_window->findChild<QPushButton*>("display_button");
    auto suspend_button = this->_window->findChild<QPushButton*>("suspend_button");

    display_button->setDisabled(true);
    suspend_button->setDisabled(true);

    display_timer->start(1000);
    update_display_button();
}

void Application::suspend_button_clicked()
{
    auto suspend_timer  = this->findChild<QTimer*>("suspend_timer");
    auto display_button = this->_window->findChild<QPushButton*>("display_button");
    auto suspend_button = this->_window->findChild<QPushButton*>("suspend_button");

    display_button->setDisabled(true);

    disconnect(suspend_button, &QPushButton::clicked, this, &Application::suspend_button_clicked);
    connect(suspend_button, &QPushButton::clicked, this, &Application::cancel_suspend_clicked);

    suspend_timer->start(1000);
    update_suspend_button();
}

void Application::cancel_suspend_clicked()
{
    auto suspend_timer  = this->findChild<QTimer*>("suspend_timer");
    auto display_button = this->_window->findChild<QPushButton*>("display_button");
    auto suspend_button = this->_window->findChild<QPushButton*>("suspend_button");

    SuspendCountdown = 5;

    suspend_button->setText("Sleep");
    disconnect(suspend_button, &QPushButton::clicked, this, &Application::cancel_suspend_clicked);
    connect(suspend_button, &QPushButton::clicked, this, &Application::suspend_button_clicked);

    display_button->setEnabled(true);

    suspend_timer->stop();
}

void Application::update_display_button()
{
    static unsigned int countdown = 5;

    auto display_timer  = this->findChild<QTimer*>("display_timer");
    auto display_button = this->_window->findChild<QPushButton*>("display_button");
    auto suspend_button = this->_window->findChild<QPushButton*>("suspend_button");

    if(countdown > 0)
    {
        display_button->setText(QString::number(countdown));
        countdown -= 1;
    }

    else
    {
        countdown = 5;
        display_button->setText("Monitors Off");
        display_button->setEnabled(true);
        suspend_button->setEnabled(true);
        display_timer->stop();

        turn_off_displays();
    }
}

void Application::update_suspend_button()
{
    auto suspend_timer  = this->findChild<QTimer*>("suspend_timer");
    auto display_button = this->_window->findChild<QPushButton*>("display_button");
    auto suspend_button = this->_window->findChild<QPushButton*>("suspend_button");

    if(SuspendCountdown > 0)
    {
        suspend_button->setText(QString("Cancel (%1)").arg(QString::number(SuspendCountdown)));
        SuspendCountdown -= 1;
    }

    else
    {
        SuspendCountdown = 5;

        suspend_button->setText("Sleep");
        disconnect(suspend_button, &QPushButton::clicked, this, &Application::cancel_suspend_clicked);
        connect(suspend_button, &QPushButton::clicked, this, &Application::suspend_button_clicked);

        display_button->setEnabled(true);

        suspend_timer->stop();

        suspend_computer();
    }
}
