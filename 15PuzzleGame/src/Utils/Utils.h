//
// Created by childcity on 12.09.20.
//

#ifndef WEBTEXTSEARCHER_UTILS_H
#define WEBTEXTSEARCHER_UTILS_H

#include <QDebug>
#include <QDateTime>
#include <memory>


#define DEBUG(msg_) qDebug() << Utils::DebugPrinter(__FILE__, __LINE__) << msg_ << "<";
//#define DEBUGGAM(msg_) // Off debug


namespace Utils {


struct DebugPrinter {
    const char *file;
    int line;
    DebugPrinter(const char *file_, int line_);
    friend QDebug operator<<(QDebug stream, const DebugPrinter printer);
};

struct QObjectDeleteLater {
    void operator()(QObject *o);
};

template<typename T>
using qt_unique_ptr = std::unique_ptr<T, QObjectDeleteLater>;

template<class T, class... _Types, std::enable_if_t<! std::is_array<T>::value, int> = 0>
inline qt_unique_ptr<T> qt_make_unique(_Types &&... _Args)
{
    return (qt_unique_ptr<T>(new T(std::forward<_Types>(_Args)...)));
}


// clang-format off
template<typename Functor>
struct OnExit {
    Functor func;
    OnExit(Functor &&f) : func(std::forward<Functor>(f)) {}
    ~OnExit() { func(); }
};
// clang-format on

template<typename Functor>
OnExit(Functor &&frv) -> OnExit<Functor>;


struct CustomMessageHandler {
    static QtMessageHandler GetDefaultMessageHandler();
    static void Handler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};


size_t GetFastNotSecureRandomNum(size_t min, size_t max);

template<class T>
const T &GetRandomElement(const std::list<T> &urls)
{
    size_t rndIndex = GetFastNotSecureRandomNum(0, urls.size() - 1);

    auto it = urls.cbegin();
    std::advance(it, rndIndex);

    assert(it != urls.end());

    return *it;
}


} // namespace Utils

#endif //WEBTEXTSEARCHER_UTILS_H
