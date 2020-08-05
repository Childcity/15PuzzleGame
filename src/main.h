#ifndef MAIN_H
#define MAIN_H

#include <QDebug>
#include <QDateTime>

struct DebugPrinter {
    const char *file;
    int line;

    DebugPrinter(const char *file_, int line_)
        : file(file_), line(line_)
    {}

    friend QDebug operator<<(QDebug stream, const DebugPrinter printer) {
        stream << QDateTime::currentDateTime().toString("mm:ss:zzz").toLatin1().data()
               << (QByteArray(printer.file) + '[' + QByteArray::number(printer.line) + "]>").data();
        return stream;
    }
};

#define DEBUG(msg_) qDebug() << DebugPrinter(__FILE__, __LINE__) << msg_ << "<"
//#define DEBUGGAM(msg_) // Off debug

#endif // MAIN_H
