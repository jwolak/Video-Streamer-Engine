#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "headers/camviewer.h"
#include "headers/Camera-Capture-Thread.h"

#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool isCameraOn;
    ~MainWindow();

private slots:
    void on_Start_pushButton_clicked();
    void updateView(void);

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    CameraCaptureThread capture_thread;
};

#endif // MAINWINDOW_H
