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

#include "application.hpp"
#include "window.hpp"
#include "ui_window.h"

Window::Window()
{
    this->hide();
    connect(Application::instance(), &Application::starting_up, this, &Window::starting_up);
}

void Window::toggle_visibility()
{
    if(this->isHidden() || this->isMinimized())
    {
        this->showNormal();
        this->activateWindow();
    }

    else
    {
        this->hide();
    }
}

void Window::hideEvent(QHideEvent* event)
{
    QMainWindow::hideEvent(event);
    emit visibility_changed(false);
}

void Window::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);
    emit visibility_changed(true);
}

void Window::starting_up()
{
    auto app = Application::instance();

    connect(app, &Application::started, this, &Window::started);
    connect(app, &Application::shutting_down, this, &Window::shutting_down);

    this->ui = new Ui::window;
    this->ui->setupUi(this);
}

void Window::started()
{
    this->show();
}

void Window::shutting_down()
{
    this->hide();
    this->setCentralWidget(nullptr);
    delete this->ui;
    this->ui = nullptr;
}
