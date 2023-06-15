/**
 * @file main.cpp
 *
 * @brief Este archivo contiene la implementación del logIn
 */

#include "login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    w.show();
    return a.exec();
}
