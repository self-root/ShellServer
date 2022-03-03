#include "clienthandler.h"

#include <QDataStream>
#include <QDir>
#include <QHostAddress>

ClientHandler::ClientHandler(QTcpSocket *client, QObject *parent)
    :QObject(parent) ,  mClient(client)
{
    inputThread = new InputThread;
    QPair<QString, QJsonValue> cmd("program", QJsonValue("echo Root_"));
    sendCommand(QJsonObject({cmd}));

    connect(mClient, &QTcpSocket::readyRead, this, &ClientHandler::clientResponse);
    connect(mClient, &QTcpSocket::disconnected, this, &ClientHandler::disconnected);
    connect(inputThread, &InputThread::ready, this, &ClientHandler::sendCommand);
    //connect(inputThread, &InputThread::finished, this, [](){std::cout << "Input thread finished\n";});
    //connect(inputThread, &InputThread::destroyed, this, [](){std::cout << "InputThread destroyed\n";});

}

QString ClientHandler::clientIp() const
{
    return mClient->peerAddress().toString();
}

int ClientHandler::clientPort() const
{
    return  mClient->peerPort();
}

void ClientHandler::handleClientResponse(const QJsonDocument &clientMessage)
{
    auto jsonObject = clientMessage.object();
    std::cout << jsonObject.value("res").toString().toStdString();
    inputThread->setCurrentDirectory(jsonObject.value("cwd").toString());
    if (jsonObject.contains("data"))
    {
        QByteArray data = QByteArray::fromBase64(QByteArray::fromStdString(jsonObject.value("data").toString().toStdString()));
        writeData(data, jsonObject.value("filename").toString());
    }
    inputThread->start();
}

void ClientHandler::writeData(const QByteArray &data, const QString &filename)
{
    QDir destinationPath(QDir::homePath() + "/serverfiles");
    if (!destinationPath.exists())
        destinationPath.mkpath(destinationPath.path());
    //qDebug() << "Writing: " << filename;
    QFile outFile(destinationPath.path() + "/" + filename);
    if (outFile.open(QIODevice::WriteOnly))
    {
        outFile.write(data);
        outFile.close();
        return;
    }

    std::cout << "Couldn't write the file: " << filename.toStdString() << " : " + outFile.errorString().toStdString() << std::endl;
}

void ClientHandler::clientResponse()
{
    QByteArray data;
    QDataStream stream(mClient);

    for (;;)
    {
        stream.startTransaction();
        stream >> data;
        if (stream.commitTransaction())
        {
            QJsonParseError err;
            const QJsonDocument jDoc = QJsonDocument::fromJson(data, &err);
            if (err.error == QJsonParseError::NoError)
            {
                handleClientResponse(jDoc);
            }
        }

        else
        {
            break;
        }
    }
}

void ClientHandler::disconnected()
{
    mClient->deleteLater();
    emit clientDisconnected(QPair<QString, int>(clientIp(), clientPort()));
}

void ClientHandler::sendCommand(QJsonObject command)
{
    if(mClient->isOpen() && mClient->isWritable())
    {
        QDataStream stream(mClient);
        //std::cout << "Sending command...\n";
        stream << QJsonDocument(command).toJson(QJsonDocument::Compact);
        //inputThread->start();
    }
}
