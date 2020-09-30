#ifndef IFUTUREIVENTS_H
#define IFUTUREIVENTS_H


namespace childcity::StdFutureWatcher {


class IFutureEvents {
public:
	virtual void sigResultReady() = 0;
};


} // namespace childcity::StdFutureWatcher


#endif // IFUTUREIVENTS_H
