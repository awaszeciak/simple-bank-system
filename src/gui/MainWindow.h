#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;

class MainWindow : public QMainWindow {
    public:
        explicit MainWindow(QWidget *parent = nullptr);

    private:
        QLabel *statusLabel;
};

#endif