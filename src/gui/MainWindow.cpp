#include "MainWindow.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), statusLabel(new QLabel("Qt GUI is working")) {

    setWindowTitle("Simple Bank System");
    resize(900, 600);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QLabel *titleLabel = new QLabel("Simple Bank System");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    statusLabel->setStyleSheet("font-size: 14px;");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(statusLabel);
    mainLayout->addStretch();

    setCentralWidget(centralWidget);
}