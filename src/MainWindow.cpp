#include "MainWindow.h"
#include "DesktopAutomationEngine.h"
#include <QThread>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 1. Configurar la ventana principal de AnSoGicLabs
    this->setWindowTitle("AnSoGicLabs // RPA Control Panel");
    this->resize(400, 150);

    // 2. Crear los componentes gráficos por código nativo
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    txtDataEntry = new QLineEdit("Texto automatizado desde AnSoGicLabs", this);
    btnStartAutomation = new QPushButton("Iniciar Inyección de Datos", this);

    layout->addWidget(txtDataEntry);
    layout->addWidget(btnStartAutomation);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // 3. Configurar el entorno multihilo para el Motor de Automatización
    QThread *automationThread = new QThread(this);
    AutomationWorker *worker = new AutomationWorker();
    worker->moveToThread(automationThread);

    // Conectar el botón al proceso del bot en segundo plano de manera segura
    connect(btnStartAutomation, &QPushButton::clicked, this, [=]() {
        QString texto = txtDataEntry->text();
        btnStartAutomation->setEnabled(false); // Bloquear botón para evitar doble envío
        
        // Ejecutar la tarea en el hilo del bot (coordenadas de ejemplo X:500, Y:400)
        QMetaObject::invokeMethod(worker, "executeDataEntry", Qt::QueuedConnection,
                                  Q_ARG(QString, texto), Q_ARG(int, 500), Q_ARG(int, 400));
    });

    // Reactivar el botón cuando el bot termine su ciclo
    connect(worker, &AutomationWorker::taskFinished, this, [=](bool success) {
        btnStartAutomation->setEnabled(true);
        QMessageBox::information(this, "RPA Status", "Secuencia de automatización nativa completada.");
    });

    // Gestión del ciclo de vida de la memoria
    connect(automationThread, &QThread::finished, worker, &QObject::deleteLater);
    automationThread->start();
}

MainWindow::~MainWindow() {}
