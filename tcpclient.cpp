#include "tcpclient.h"
#include <QDataStream>
#include <QDebug>

TcpClient::TcpClient(QObject *parent) : QObject(parent),
    imgSocket(new QTcpSocket(this)), cmdSocket(new QTcpSocket(this))
{
    // Connect socket signals to the class slots
    connect(imgSocket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(imgSocket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(imgSocket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(imgSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &TcpClient::onErrorOccurred);

    connect(cmdSocket, &QTcpSocket::readyRead, this, &TcpClient::onReadyReadCmd);
}

void TcpClient::connectToServerImg(const QString &host, quint16 port)
{
    imgSocket->connectToHost(host, port);
}
void TcpClient::connectToServerCmd(const QString &host, quint16 port)
{
    cmdSocket->connectToHost(host, port);
}

void TcpClient::sendCommand(const QString &data)
{
    cmdSocket->flush();
     if (cmdSocket->state() == QAbstractSocket::ConnectedState)  {
        qDebug() << "cmdSocket:" << cmdSocket->isOpen();
        cmdSocket->write(data.toUtf8() + "#\n");  // Append last separator + Linefeed
        qDebug() << "cmdSocket->write:" << data.toUtf8() + "#\n";
    }else {
         emit errorOccurred("cmdSocket is not connected.");
     }
}


QTcpSocket* TcpClient::getImgSocket() const {
    return imgSocket;
}

QTcpSocket* TcpClient::getCmdSocket() const {
    return cmdSocket;
}

void TcpClient::disconnectFromServer()
{
    imgSocket->disconnectFromHost();
}

void TcpClient::onReadyRead()
{

    QDataStream in(imgSocket);
    in.setVersion(QDataStream::Qt_6_8);

    // If we haven't received the image size yet, read it first
    if (!sizeReceived) {

        //if (tcpSocket->bytesAvailable() < sizeof(quint32)) {
        if (imgSocket->bytesAvailable() < 4) {
            // Wait for the complete size to be available
            return;
        }

        // Read the image size (4 bytes, quint32)
        in >> imageSize;
        sizeReceived = true;
        qDebug() << "Expecting image of size:" << imageSize;
    }

    // Now read the image data
    if (imgSocket->bytesAvailable() < imageSize) {
        // Wait until we have the full image data
        return;
    }

    // Read the image data into the buffer
    imageBuffer.append(imgSocket->read(imageSize));


    emit dataReceived(imageBuffer);  // Emit signal when new data is received

    // Reset for the next image
    imageBuffer.clear();
    imageSize = 0;
    sizeReceived = false;
}

void TcpClient::onReadyReadCmd()
{
    QDataStream in(cmdSocket);
    in.setVersion(QDataStream::Qt_6_8);

    qDebug() << "CMD bytesAvailable:" << cmdSocket->bytesAvailable();
    // if (cmdSocket->bytesAvailable() < 6) {
    //     // Wait until we have the full image data
    //     return;
    // }
    // Read the image data into the buffer
    cmdBuffer = cmdSocket->readAll();
    qDebug() << "cmdBuffer:" << cmdBuffer;

    if (cmdBuffer.length() > 0){
        QChar cmd = cmdBuffer[0];
        qDebug() << "cmd:" << cmd;
    }
    if (cmdBuffer.length() > 1){
        QByteArray setting = cmdBuffer.mid(1);
        qDebug() << "setting:" << setting;
    }
    cmdBuffer.clear();
}

void TcpClient::onConnected()
{
    qDebug() << "Connected to server.";
    emit connected();
}

void TcpClient::onDisconnected()
{
    qDebug() << "Disconnected from server.";
    emit disconnected();
}

void TcpClient::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    emit errorOccurred(imgSocket->errorString());
}
