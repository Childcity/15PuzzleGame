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
    explicit Downloader(QObject *parent = nullptr);

    ~Downloader();

    void download(const QUrl fileURL);

    bool unZip(const QString destFolder, const QString password);

    double getDownloadProgress() const;

signals:
    void sigComplite(bool isOk);

    void sigProgressChanged(qint64 bytesRead, qint64 bytesTotal, double progresInPercent);

private slots:
    void slotFinished(QNetworkReply *reply);

    void slotProgressChanged(qint64 bytesRead, qint64 bytesTotal);

private:
    QPointer<QNetworkAccessManager> manager_;
    QPointer<QNetworkReply> reply_;
    QPointer<QTemporaryFile> file_;
    double downloadProgress_ = -1;
};

#endif // DOWNLOADER_H
