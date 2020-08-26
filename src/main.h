#ifndef MAIN_H
#define MAIN_H

#include <QDebug>
#include <QDateTime>

#include <memory>

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

struct QObjectDeleteLater {
    void operator()(QObject *o) {
        if (o->parent()){
            o->deleteLater();
        } else {
            delete o;
        }
    }
};

template<typename T>
using qt_unique_ptr = std::unique_ptr<T, QObjectDeleteLater>;

template<class T, class... _Types, std::enable_if_t<! std::is_array<T>::value, int> = 0>
inline qt_unique_ptr<T> qt_make_unique(_Types&&... _Args){
    return (qt_unique_ptr<T>(new T(std::forward<_Types>(_Args)...)));
}

#define DEBUG(msg_) qDebug() << DebugPrinter(__FILE__, __LINE__) << msg_ << "<";
//#define DEBUGGAM(msg_) // Off debug

#endif // MAIN_H
