/**
 * \file main2.cpp
 * \brief Entry point of 2nd demo
 * \author Diego Iastrubni (elcuco@kde.org)
 * License GPL
 */

// $Id$ 

#include <QApplication>
#include "mainwindow3.h"

int main( int argc, char *argv[] )
{
        QApplication app( argc, argv );
        MainWindow3 w;
        w.show();
        return app.exec();
}
