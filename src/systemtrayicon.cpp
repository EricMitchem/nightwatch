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

#include <QMenu>
#include "application.hpp"
#include "systemtrayicon.hpp"
#include "window.hpp"

SystemTrayIcon::SystemTrayIcon()
{
    connect(Application::instance(), &Application::starting_up, this, &SystemTrayIcon::starting_up);
}

void SystemTrayIcon::starting_up()
{
    auto app    = Application::instance();
    auto window = Application::window();

    connect(app, &Application::started, this, &SystemTrayIcon::started);
    connect(app, &Application::shutting_down, this, &SystemTrayIcon::shutting_down);
    connect(this, &SystemTrayIcon::activated, this, &SystemTrayIcon::_activated);

    auto menu = new QMenu;

    auto act_showhide = new QAction(menu);
    act_showhide->setObjectName("showhide");
    act_showhide->setText("Show/Hide");
    connect(act_showhide, &QAction::triggered, window, &Window::toggle_visibility);

    auto act_about = new QAction(menu);
    act_about->setObjectName("about");
    act_about->setText("About nightwatch");
    connect(act_about, &QAction::triggered, app,  &Application::about_nightwatch);

    auto act_exit = new QAction(menu);
    act_exit->setObjectName("exit");
    act_exit->setText("Exit");
    connect(act_exit, &QAction::triggered, app, &Application::quit);

    menu->addAction(act_showhide);
    menu->addAction(act_about);
    menu->addAction(act_exit);

    this->setContextMenu(menu);
    this->setIcon(QIcon(":/icons/nightwatch.ico"));
    this->setToolTip("nightwatch");
    this->show();
}

void SystemTrayIcon::started()
{
    auto window    = Application::window();
    bool hidden    = window->isHidden();
    bool minimized = window->isMinimized();

    update_visibility(!(hidden || minimized));

    connect(this, &SystemTrayIcon::double_clicked, window, &Window::toggle_visibility);
    connect(window, &Window::visibility_changed, this, &SystemTrayIcon::update_visibility);
}

void SystemTrayIcon::shutting_down()
{
    delete this->contextMenu();
    this->setContextMenu(nullptr);
}

void SystemTrayIcon::_activated(ActivationReason reason)
{
    if(reason == DoubleClick)
    {
        emit double_clicked();
    }
}

void SystemTrayIcon::update_visibility(bool visible)
{
    auto act_showhide = this->contextMenu()->findChild<QAction*>("showhide");

    if(visible == true)
    {
        act_showhide->setText("Hide");
    }

    else
    {
        act_showhide->setText("Show");
    }
}
