#ifndef IFUTUREWATCHER_H
#define IFUTUREWATCHER_H

#include <future>


namespace childcity::StdFutureWatcher {


template<class FResult>
class IFutureWatcher {
public:
    virtual void setFuture(std::future<FResult> future) = 0;
    virtual std::shared_ptr<std::future<FResult>> getFuture() const = 0;
    virtual FResult getResult() = 0;
    virtual ~IFutureWatcher() {}
};


} // namespace childcity::StdFutureWatcher


#endif // IFUTUREWATCHER_H
