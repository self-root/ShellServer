#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QString>
#include <QPair>

#include "inputthread.h"

class ClientHandler : public QObject
{
    Q_OBJECT
public:
    explicit ClientHandler(QTcpSocket *client, QObject *parent = nullptr);

    QString clientIp() const;
    int clientPort() const;

private:
    QTcpSocket *mClient;
    InputThread *inputThread;

private:
    void handleClientResponse(const QJsonDocument &clientMessage);
    void writeData(const QByteArray &data, const QString &filename);

signals:
    void clientDisconnected(const QPair<QString, int> &clientAddress);

private slots:
    void clientResponse();
    void disconnected();
    void sendCommand(QJsonObject command);

};

#endif // CLIENTHANDLER_H
