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

#ifndef NIGHTWATCH_SYSTEMTRAYICON_HPP
#define NIGHTWATCH_SYSTEMTRAYICON_HPP

#include <QSystemTrayIcon>

class SystemTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    SystemTrayIcon();
    ~SystemTrayIcon() = default;

signals:
    void double_clicked();

private:
    SystemTrayIcon(const SystemTrayIcon&)            = delete;
    SystemTrayIcon(SystemTrayIcon&&)                 = delete;
    SystemTrayIcon& operator=(const SystemTrayIcon&) = delete;
    SystemTrayIcon& operator=(SystemTrayIcon&&)      = delete;

private slots:
    void starting_up();
    void started();
    void shutting_down();
    void _activated(ActivationReason reason);
    void showhide_triggered();
    void exit_triggered();
};

#endif
