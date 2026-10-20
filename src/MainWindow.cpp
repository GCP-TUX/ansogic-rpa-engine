#include "DesktopAutomationEngine.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Inicializar el hilo de ejecución para proteger la concurrencia
    QThread *automationThread = new QThread(this);
    AutomationWorker *worker = new AutomationWorker();
    worker->moveToThread(automationThread);

    // Conexiones de Señales y Slots para la ejecución asíncrona
    connect(ui->btnStartAutomation, &QPushButton::clicked, this, [=]() {
        QString data = ui->txtDataEntry->text();
        int targetX = ui->spinX->value();
        int targetY = ui->spinY->value();
        
        // Disparar tarea en segundo plano de forma segura
        QMetaObject::invokeMethod(worker, "executeDataEntry", Qt::QueuedConnection,
                                  Q_ARG(QString, data), Q_ARG(int, targetX), Q_ARG(int, targetY));
    });

    connect(ui->btnInspectROI, &QPushButton::clicked, this, [=]() {
        QRect targetROI(100, 100, 400, 200); // Región fija de ejemplo de un sistema ERP
        QMetaObject::invokeMethod(worker, "inspectScreenRegion", Qt::QueuedConnection,
                                  Q_ARG(QRect, targetROI));
    });

    // Respuestas del hilo de trabajo a la interfaz de usuario
    connect(worker, &AutomationWorker::taskFinished, this, [](bool success) {
        qDebug() << "Flujo de automatización completado con estatus:" << success;
    });

    connect(worker, &AutomationWorker::screenProcessed, this, [=](const QPixmap &pix) {
        if(!pix.isNull()) {
            ui->lblPreviewROI->setPixmap(pix.scaled(ui->lblPreviewROI->size(), Qt::KeepAspectRatio));
        }
    });

    // Limpieza de memoria al destruir el objeto
    connect(automationThread, &QThread::finished, worker, &QObject::deleteLater);
    automationThread->start();
}

MainWindow::~MainWindow() {
    delete ui;
}
