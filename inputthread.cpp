#include "inputthread.h"
#include <QDebug>


void InputThread::run()
{
    QString input;

    std::string i;
    std::cout << mCwd.toStdString();
    std::getline(std::cin, i);
    input = QString::fromStdString(i);
    processCommand(input);

}

void InputThread::setCurrentDirectory(const QString &cwd)
{
    if (mCwd != cwd)
        mCwd = cwd + "> ";
}

void InputThread::processCommand(const QString &inputString)
{
    if (inputString.isEmpty())
        return run();
    auto cmd = inputString.split(" ");
    auto program = cmd.at(0);
    cmd.removeFirst();
    QString args = cmd.join(" ");

    QJsonObject jDoc;
    jDoc["program"] = program;
    jDoc["args"] = args;
    //qDebug() << program << " : " << args;
    emit ready(jDoc);
}
