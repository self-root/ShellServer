#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>
#include <QPair>


#include "clienthandler.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QTcpServer *parent=nullptr);
    void startServer();
private:
    ClientHandler *clientHandler;
private slots:
    void newConnection();
    void clientDisconnected(QPair<QString, int> clientInfo);
};

#endif // SERVER_H
