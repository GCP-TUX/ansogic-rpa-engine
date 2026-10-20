#ifndef DESKTOPAUTOMATIONENGINE_H
#define DESKTOPAUTOMATIONENGINE_H

#include <QObject>
#include <QThread>
#include <QRect>
#include <QPixmap>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class AutomationWorker : public QObject {
    Q_OBJECT
public:
    explicit AutomationWorker(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    // Tarea 1: Inyección Nativa de Eventos (Escribir texto y hacer clics de fondo)
    void executeDataEntry(const QString &textToType, int targetX, int targetY) {
        qDebug() << "Iniciando secuencia de automatización en coordenadas:" << targetX << "," << targetY;

#ifdef Q_OS_WIN
        // 1. Simular Movimiento de Mouse y Clic
        // Convertir coordenadas de pantalla a coordenadas absolutas de la API de Windows (0 a 65535)
        double screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
        double screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
        
        int posX = static_cast<int>((targetX * 65535) / screenWidth);
        int posY = static_cast<int>((targetY * 65535) / screenHeight);

        INPUT inputs[3] = {};
        
        // Mover mouse
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dx = posX;
        inputs[0].mi.dy = posY;
        inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

        // Clic abajo
        inputs[1].type = INPUT_MOUSE;
        inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

        // Clic arriba
        inputs[2].type = INPUT_MOUSE;
        inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

        ::SendInput(3, inputs, sizeof(INPUT));
        QThread::msleep(200); // Latencia controlada para dar tiempo a la UI empresarial

        // 2. Inyección de caracteres (Data Entry desatendido)
        for (const QChar &ch : textToType) {
            INPUT keyInput = {};
            keyInput.type = INPUT_KEYBOARD;
            keyInput.ki.wScan = ch.unicode();
            keyInput.ki.dwFlags = KEYEVENTF_UNICODE;
            ::SendInput(1, &keyInput, sizeof(INPUT));
            
            // Simular soltar la tecla
            keyInput.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
            ::SendInput(1, &keyInput, sizeof(INPUT));
            QThread::msleep(30); // Velocidad de tecleado controlada
        }
#else
        qDebug() << "Inyección nativa no implementada para esta plataforma corporativa (Falta X11/Wayland).";
#endif
        emit taskFinished(true);
    }

    // Tarea 2: Procesamiento de Pantalla para Extracción de Datos (Screen Scraping ROI)
    void inspectScreenRegion(const QRect &roi) {
        qDebug() << "Inspeccionando región crítica de la pantalla:" << roi;
        
        QScreen *screen = QGuiApplication::primaryScreen();
        if (!screen) {
            emit screenProcessed(QPixmap());
            return;
        }

        // Capturar únicamente la región de interés del sistema legado (ahorra memoria y CPU)
        QPixmap screenshot = screen->grabWindow(0, roi.x(), roi.y(), roi.width(), roi.height());
        
        // Aquí es donde en una solución de producción integrarías OpenCV (como en tu AnSoGicScroll) 
        // o un motor de OCR como Tesseract para convertir los pixeles en texto crudo.
        
        emit screenProcessed(screenshot);
    }

signals:
    void taskFinished(bool success);
    void screenProcessed(const QPixmap &extractedRegion);
};

#endif // DESKTOPAUTOMATIONENGINE_H
