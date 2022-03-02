#ifndef INPUTTHREAD_H
#define INPUTTHREAD_H

#include <QThread>
#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>

#include <iostream>
#include <string>

class InputThread : public QThread
{
    Q_OBJECT
public:
    void run() override;
    void setCurrentDirectory(const QString &cwd);
    void processCommand(const QString &inputString);
signals:
    void ready(QJsonObject d);
private:
    QString mCwd = ">";

};

#endif // INPUTTHREAD_H
