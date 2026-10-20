#include <QApplication>
#include "MainWindow.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    qDebug() << "[AnSoGicLabs] Arrancando panel de control e interfaz gráfica...";
    
    MainWindow w;
    w.show(); // Muestra la ventana en pantalla

    return app.exec();
}

