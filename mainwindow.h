#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLabel>
#include <QByteArray>
#include "tcpclient.h"
#include "dbdialogs.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void sendCommand(const QString &data);

public slots:
    void onDataReceived(const QByteArray &data); // Slot to handle incoming data
    void onCmdReceived(const QByteArray &data);  // Slot to handle incoming command
    //void onConnected();          // Slot for successful connection
    void onDisconnected();       // Slot for handling disconnection
    void onReadyReadCmd();
    void getParameters(int, int, int, int, int, int);

    void startRun();
    void stopRun();
    void lightCheckbox(int arg1);
    void reverseFast();
    void reverseSlow();
    void motorStop();
    void forwardSlow();
    void forwardFast();

    void breakCmd();
    void fileSelector();

    void on_analogueGainBox_valueChanged(double arg1);


private slots:
    void rbPreview();
    void rbTestRun();
    void rbScanRun();

private:
    Ui::MainWindow *ui;
    TcpClient *tcpClient;  // Network-related functionality is handled by Tcp
    dbDialogs *dbdialogs;
    void showError(const QString &message, QWidget *parent = nullptr);
    bool isValidFileName(const QString &fileName);
    void createDir(const QString &path, const QString &directoryName);
    QString addLeadingZerosToInteger(QString numberStr, int leadingZeros);
    QString runMode = "preview";


};

#endif // MAINWINDOW_H
