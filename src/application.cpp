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

#include <Windows.h>
#include <PowrProf.h>
#include <QApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include "application.hpp"
#include "window.hpp"

void displays_off(Window* window);
void computer_suspend(Window* window);

Application::Application() {}

Application::~Application() {}

int Application::run(int argc, char** argv)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    int zero = 0;
    QApplication app(zero, nullptr);

    QTimer::singleShot(0, this, &Application::startup);

    return app.exec();
}

void Application::startup()
{
    window.reset(new Window);

    auto display_timer = new QTimer(this);
    auto suspend_timer = new QTimer(this);

    display_timer->setObjectName("display_timer");
    suspend_timer->setObjectName("suspend_timer");

    connect(display_timer, &QTimer::timeout, this, &Application::update_display_button);
    connect(suspend_timer, &QTimer::timeout, this, &Application::update_suspend_button);

    auto display_button = window->findChild<QPushButton*>("display_button");
    connect(display_button, &QPushButton::clicked, this, &Application::display_button_clicked);

    auto suspend_button = window->findChild<QPushButton*>("suspend_button");
    connect(suspend_button, &QPushButton::clicked, this, &Application::suspend_button_clicked);

    window->show();
}

void Application::display_button_clicked()
{
    auto display_timer  = this->findChild<QTimer*>("display_timer");
    auto display_button = window->findChild<QPushButton*>("display_button");
    auto suspend_button = window->findChild<QPushButton*>("suspend_button");

    display_button->setDisabled(true);
    suspend_button->setDisabled(true);

    display_timer->start(1000);
    update_display_button();
}

void Application::suspend_button_clicked()
{
    auto suspend_timer  = this->findChild<QTimer*>("suspend_timer");
    auto display_button = window->findChild<QPushButton*>("display_button");
    auto suspend_button = window->findChild<QPushButton*>("suspend_button");

    display_button->setDisabled(true);
    suspend_button->setDisabled(true);

    suspend_timer->start(1000);
    update_suspend_button();
}

void Application::update_display_button()
{
    static unsigned int countdown = 5;

    auto display_timer  = this->findChild<QTimer*>("display_timer");
    auto display_button = window->findChild<QPushButton*>("display_button");
    auto suspend_button = window->findChild<QPushButton*>("suspend_button");

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

        displays_off(window.get());
    }
}

void Application::update_suspend_button()
{
    static unsigned int countdown = 5;

    auto suspend_timer  = this->findChild<QTimer*>("suspend_timer");
    auto display_button = window->findChild<QPushButton*>("display_button");
    auto suspend_button = window->findChild<QPushButton*>("suspend_button");

    if(countdown > 0)
    {
        suspend_button->setText(QString::number(countdown));
        countdown -= 1;
    }

    else
    {
        countdown = 5;
        suspend_button->setText("Sleep");
        suspend_button->setEnabled(true);
        display_button->setEnabled(true);
        suspend_timer->stop();

        computer_suspend(window.get());
    }
}

void displays_off(Window* window)
{
    HWND   handle      = (HWND) window->winId();
    LPARAM display_off = 2;

    SendMessageW(handle, WM_SYSCOMMAND, SC_MONITORPOWER, display_off);
}

void computer_suspend(Window* window)
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

        QMessageBox::warning(window, "Error", err_str);
    }
}
