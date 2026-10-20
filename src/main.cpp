#include <QApplication>
#include <QThread>
#include <QDebug>
#include "DesktopAutomationEngine.h"

int main(int argc, char *argv[])
{
    // 1. Inicializar la infraestructura gráfica de Qt para interactuar con las pantallas del SO
    QApplication app(argc, argv);
    
    qDebug() << "==================================================";
    // Marca de agua de tu laboratorio en la salida de consola corporativa
    qDebug() << "[AnSoGicLabs] Inicializando Native RPA Engine PoC...";
    qDebug() << "==================================================";

    // 2. Configurar el entorno multihilo (Multi-Threaded Architecture)
    QThread* automationThread = new QThread();
    AutomationWorker* worker = new AutomationWorker();
    
    // Mover el trabajador al hilo secundario para evitar bloquear el sistema operativo
    worker->moveToThread(automationThread);

    // 3. Simulación de ejecución automatizada
    // Conectamos el inicio del hilo directamente a la tarea de inyección nativa
    // Parámetros de ejemplo: Texto a escribir, Coordenada X, Coordenada Y
    QObject::connect(automationThread, &QThread::started, worker, [=]() {
        // Simula dar un clic en la coordenada (500, 400) y escribir un texto corporativo
        // Puedes cambiar estos parámetros para apuntar a un campo de texto de un ERP real
        worker->executeDataEntry("AnSoGicLabs RPA Data Entry Line 1", 500, 400);
    });

    // 4. Ciclo de vida y limpieza de memoria automatizada
    // Cuando el bot termine la tarea, solicitamos al hilo que se detenga de forma segura
    QObject::connect(worker, &AutomationWorker::taskFinished, automationThread, &QThread::quit);
    
    // Al detenerse el hilo, cerramos la aplicación de Qt y liberamos memoria
    QObject::connect(automationThread, &QThread::finished, &app, &QApplication::quit);
    QObject::connect(automationThread, &QThread::finished, worker, &QObject::deleteLater);
    QObject::connect(automationThread, &QThread::finished, automationThread, &QThread::deleteLater);

    // 5. Arrancar el motor en segundo plano
    automationThread->start();

    // Iniciar el bucle de eventos principal (Event Loop)
    return app.exec();
}
