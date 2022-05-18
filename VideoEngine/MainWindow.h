#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Camera-Capture-Thread.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
	Ui::MainWindow *ui;
    CameraCaptureThread capture_thread;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

protected:
	void timerEvent(QTimerEvent *event);
};

#endif // MAINWINDOW_H
