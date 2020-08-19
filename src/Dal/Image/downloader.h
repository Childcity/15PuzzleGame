#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QTemporaryFile>
#include <QNetworkAccessManager>
#include <QObject>
#include <QPointer>
#include <QNetworkReply>
#include <QProcess>
#include <QTemporaryDir>
#include <memory>

#include "main.h"

class Downloader : public QObject {
    Q_OBJECT
public:
    explicit Downloader(QObject *){}

    ~Downloader(){}

signals:

private:

};

#endif // DOWNLOADER_H
