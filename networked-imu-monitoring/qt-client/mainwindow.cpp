#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStringList>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    cubeWidget = new CubeWidget(this);

    auto *layout = new QVBoxLayout(ui->cubeContainer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(cubeWidget);

    connect(socket, &QTcpSocket::connected,
            this, &MainWindow::onConnected);

    connect(socket, &QTcpSocket::disconnected,
            this, &MainWindow::onDisconnected);

    connect(socket, &QTcpSocket::readyRead,
            this, &MainWindow::onReadyRead);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
    QString ip = ui->lineEdit_ip->text();
    quint16 port = ui->lineEdit_port->text().toUShort();

    ui->label_status->setText("Status: Connecting...");

    socket->connectToHost(ip, port);
}

void MainWindow::onConnected()
{
    ui->label_status->setText("Status: Connected");
}

void MainWindow::onDisconnected()
{
    ui->label_status->setText("Status: Disconnected");
}

void MainWindow::onReadyRead()
{
    buffer += socket->readAll();

    while (buffer.contains('\n'))
    {
        int index = buffer.indexOf('\n');

        QString line = buffer.left(index).trimmed();

        buffer.remove(0, index + 1);

        QStringList parts = line.split(',');

        if (parts.size() == 4)
        {
            ui->label_q0->setText("q0: " + parts[0]);
            ui->label_q1->setText("q1: " + parts[1]);
            ui->label_q2->setText("q2: " + parts[2]);
            ui->label_q3->setText("q3: " + parts[3]);

            float q0 = parts[0].toFloat();
            float q1 = parts[1].toFloat();
            float q2 = parts[2].toFloat();
            float q3 = parts[3].toFloat();

            cubeWidget->setQuaternion(q0, q1, q2, q3);
        }
    }
}
