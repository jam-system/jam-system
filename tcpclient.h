#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);

    void connectToServerImg(const QString &host, quint16 port);
    void connectToServerCmd(const QString &host, quint16 port);
    void sendCommand(const QString &data);
    void disconnectFromServer();
    QTcpSocket* getImgSocket() const;  // <-- Getter for tcpSocket
    QTcpSocket* getCmdSocket() const;  // <-- Getter for tcpSocket

    QTcpSocket *imgSocket;  // Now public
    QTcpSocket *cmdSocket;


signals:
    void dataReceived(const QByteArray &data);
    void cmdReceived(const QByteArray &cmd);
    void connected();
    void disconnected();
    void errorOccurred(const QString &errorMessage);

private slots:
    void onReadyRead();
    void onReadyReadCmd();
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    bool sizeReceived = false;   // Whether the size has been received
    QByteArray imageBuffer;      // Buffer for image data
    quint32 imageSize = 0;       // Expected image size

    QByteArray cmdBuffer;       // Buffer for command data


};

#endif // TCPCLIENT_H
