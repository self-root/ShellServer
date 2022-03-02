#include "server.h"
#include <QDebug>
#include <iostream>

Server::Server(QTcpServer *parent)
 : QTcpServer(parent)
{
}

void Server::startServer()
{
    connect(this, &QTcpServer::newConnection, this, &Server::newConnection);

    if (this->listen(QHostAddress::Any, 9003))
    {
        qDebug() << "Server listening into the port 9003";
    }

    else
    {
        qDebug() << "Error: " << this->errorString();
    }
}

void Server::newConnection()
{
    clientHandler = new ClientHandler(this->nextPendingConnection());
    qDebug() << clientHandler->clientIp() << " : " << clientHandler->clientPort() << " Connected";
    this->pauseAccepting();

    connect(clientHandler, &ClientHandler::clientDisconnected, this, &Server::clientDisconnected);
}

void Server::clientDisconnected(QPair<QString, int> clientInfo)
{
    qDebug() << clientInfo.first << " : " << clientInfo.second << " Disconnected";
    this->resumeAccepting();
}
