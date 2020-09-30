#ifndef FUTUREWATCHER_H
#define FUTUREWATCHER_H

#include "ifutureevents.h"
#include "ifuturewatcher.h"

#include <QThread>
#include <QPointer>
#include <memory>
#include <future>


namespace childcity::StdFutureWatcher {


class FutureWatcherBase : public QObject,
                          public IFutureEvents {
private:
    Q_OBJECT

public:
    explicit FutureWatcherBase(QObject *parent = nullptr)
        : QObject(parent)
        , waiterThread_(nullptr)
    {}

    ~FutureWatcherBase() override
    {
        if (waiterThread_) {
            if (waiterThread_->isRunning()) {
                waiterThread_->disconnect(waiterThread_, &QThread::finished, nullptr, nullptr);
                waiterThread_->wait();
                delete waiterThread_;
                waiterThread_ = nullptr;
            }
        }
    }

protected:
    template<class FResult>
    void startWatching(std::shared_ptr<std::future<FResult>> futureToWait)
    {
        // don't support invalid future here
        assert(futureToWait->valid());

        waiterThread_ = QThread::create([futureToWait = std::move(futureToWait)] {
            // waiting on future in background thread
            futureToWait->wait();
        });

        assert(waiterThread_);
        connect(waiterThread_, &QThread::finished, this, &FutureWatcherBase::sigResultReady, Qt::QueuedConnection);
        waiterThread_->start();
    }

signals:
    void sigResultReady() override;

private:
    QPointer<QThread> waiterThread_;
};

template<class FResult>
class FutureWatcher : public FutureWatcherBase,
                      public IFutureWatcher<FResult> {
public:
    explicit FutureWatcher(QObject *parent = nullptr)
        : FutureWatcherBase(parent)
    {}

    ~FutureWatcher() override {}

    void setFuture(std::future<FResult> future) override
    {
        // future_ must be invalid. If future_ is valid, then future_ has set before
        // FutureWatcher is watching only for one future
        assert(! future_);

        future_ = std::make_shared<std::future<FResult>>(std::move(future));
        startWatching(future_);
    }

    FResult getResult() override
    {
        return future_->get();
    }

    std::shared_ptr<std::future<FResult>> getFuture() const override
    {
        return future_;
    }

private:
    std::shared_ptr<std::future<FResult>> future_;
};


} // namespace childcity::StdFutureWatcher

#endif // FUTUREWATCHER_H
