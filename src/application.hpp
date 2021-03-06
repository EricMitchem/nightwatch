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

#ifndef NIGHTWATCH_APPLICATION_HPP
#define NIGHTWATCH_APPLICATION_HPP

#include <QObject>

class AboutDialog;
class SystemTrayIcon;
class Window;

class Application : public QObject
{
    Q_OBJECT

public:
    Application();
    ~Application();

    int run(int argc, char** argv);

    void turn_off_displays();
    void suspend_computer();

    static Application*    instance();
    static Window*         window();
    static QString         version();

public slots:
    void about_nightwatch();
    void quit();

signals:
    void starting_up();
    void started();
    void shutting_down();

private:
    Window*         _window;
    SystemTrayIcon* _icon;
    AboutDialog*    _about;

    static Application* _instance;

    static const unsigned int version_major = 1;
    static const unsigned int version_minor = 0;
    static const unsigned int version_patch = 2;

    Application(const Application&)            = delete;
    Application(Application&&)                 = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&)      = delete;

private slots:
    void startup();
    void shutdown();

    void display_button_clicked();
    void suspend_button_clicked();
    void cancel_suspend_clicked();

    void update_display_button();
    void update_suspend_button();
};

#endif
