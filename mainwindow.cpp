#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDataStream>
#include <QPixmap>
#include <QImage>
#include <QDebug>
#include <QDir>
#include "tcpclient.h"
#include <QFile>
#include <QRegularExpression>
#include <QMessageBox>
#include "dbdialogs.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tcpClient(new TcpClient(this))  // Initialize TcpClient

{
    ui->setupUi(this);
    // // Connect TcpClient signals to slots in MainWindow
    connect(tcpClient, &TcpClient::dataReceived, this, &MainWindow::onDataReceived);
    connect(tcpClient, &TcpClient::cmdReceived, this, &MainWindow::onCmdReceived);

    tcpClient->connectToServerImg("192.168.1.147", 8000);
    tcpClient->connectToServerCmd("192.168.1.147", 8001);

    connect(ui->btMotorStop,    &QPushButton::clicked, this,  &MainWindow::motorStop);
    connect(ui->btReverseFast,  &QPushButton::clicked, this,  &MainWindow::reverseFast);
    connect(ui->btReverseSlow,  &QPushButton::clicked, this,  &MainWindow::reverseSlow);
    connect(ui->btForwardFast,  &QPushButton::clicked, this,  &MainWindow::forwardFast);
    connect(ui->btForwardSlow,  &QPushButton::clicked, this,  &MainWindow::forwardSlow);
    connect(ui->btStartRun,   &QPushButton::clicked, this,  &MainWindow::startRun);
    connect(ui->btStopRun,    &QPushButton::clicked, this,  &MainWindow::stopRun);
    connect(ui->btBreak,      &QPushButton::clicked, this,  &MainWindow::breakCmd);
    connect(ui->lightCheckbox,&QPushButton::clicked, this,  &MainWindow::lightCheckbox);
    connect(ui->fileSelector, &QPushButton::clicked, this,  &MainWindow::fileSelector);
    connect(ui->rbPreview,    &QRadioButton::clicked, this,  &MainWindow::rbPreview);
    connect(ui->rbTestRun,    &QRadioButton::clicked, this,  &MainWindow::rbTestRun);
    connect(ui->rbScanRun,    &QRadioButton::clicked, this,  &MainWindow::rbScanRun);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendCommand(const QString &data)
{
    tcpClient->sendCommand(data.toUtf8());
}

void MainWindow::onDisconnected()
{
    qDebug() << "Disconnected from server!";
    ui->imageLabel->setText("Disconnected from server.");
}

void MainWindow::onDataReceived(const QByteArray &imageBuffer)
{
    QImage image;
    if (image.loadFromData(imageBuffer, "JPEG")) {
        // Convert the QImage to QPixmap and display it in QLabel
        QPixmap pixmap = QPixmap::fromImage(image);
        ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        qDebug() << "Image received and displayed!";
    } else {
        qDebug() << "Failed to load image";
    }

    // // Save the image to disk (optional)
    // QString filePath = QDir::homePath() + "/received_image_" + QDateTime::currentDateTime().toString("mmss.zzz") + ".jpg";
    // if (image.save(filePath, "JPEG")) {
    //     qDebug() << "Image saved to" << filePath;
    // } else {
    //     qDebug() << "Failed to save image to disk";
    // }

    // // Reset for the next image
    // imageBuffer.clear();
    // imageSize = 0;
    // sizeReceived = false;
}

QString MainWindow::addLeadingZerosToInteger(QString numberStr, int leadingZeros){
    return QString("%1").arg(numberStr.toInt(), leadingZeros, 10, QChar('0'));
}

void MainWindow::createDir(const QString &path, const QString &directoryName)
{
    QDir dir(path);
    qDebug() << path;
    if (!dir.exists(directoryName)) {
        dir.mkdir(directoryName);
        //emit messageDialog("INFO", path+"/"+directoryName+"/\nDirectory created");
        return;
    }else{
        //emit messageDialog("WARNING", path+"/"+directoryName+"/\nDirectory already exists");
        return;
    }
}

void MainWindow::getParameters(int currentReelId, int currentSceneId, int currentStartFrame,
                                int currentEndFrame, int currentGain, int currentExposure  ){
    qDebug() << "received: " << currentReelId<< currentSceneId<< currentStartFrame<< currentEndFrame<<
                              currentGain<< currentExposure  ;
}

void MainWindow::onCmdReceived(const QByteArray &cmd){

}

void MainWindow::onReadyReadCmd()
{
    QTcpSocket *cmdSocket = tcpClient->getCmdSocket();

    QDataStream in(cmdSocket);
    in.setVersion(QDataStream::Qt_5_15);

    QString buffer;

    while (cmdSocket->bytesAvailable())
    {
        buffer.append(cmdSocket->readAll());
    }
    qDebug() << "onReadyReadCmd" << buffer;
}

void MainWindow::startRun()
{
    sendCommand("s");
}

void MainWindow::stopRun()
{
    sendCommand("S");
}

void MainWindow::lightCheckbox(int arg1)
{
    if (arg1 > 0 ){
        sendCommand("l|");
    } else {
        sendCommand("L|");
    }
}

void MainWindow::reverseFast()
{
    sendCommand("D|5");
}

void MainWindow::reverseSlow()
{
    sendCommand("D|2");
}

void MainWindow::motorStop()
{
    sendCommand("p|");
}

void MainWindow::forwardSlow()
{
    sendCommand("d|2");
}

void MainWindow::forwardFast()
{
    sendCommand("d|5");
}


void MainWindow::breakCmd()
{
    sendCommand("x|");
    QCoreApplication::exit();
}


void MainWindow::fileSelector()
{
    dbDialogs *dbdialogs = new dbDialogs();
    connect(dbdialogs, &dbDialogs::setParameters, this, &MainWindow::getParameters);
       dbdialogs->exec();
}


void MainWindow::on_analogueGainBox_valueChanged(double arg1)
{
    qDebug() << "gainSpinbox" << arg1;
}


void MainWindow::rbPreview()
{
    runMode = "preview";
}

void MainWindow::rbTestRun()
{
    runMode = "testrun";
}

void MainWindow::rbScanRun()
{
    runMode = "scanrun";
}

