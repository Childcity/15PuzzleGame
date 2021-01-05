#pragma once

#include <list>
#include <regex>
#include <chrono>
#include <stdexcept>

#include <QByteArray>

namespace Dal::Image {


size_t getFastNotSecureRandomNum(size_t min, size_t max)
{
    using namespace std::chrono;
    unsigned now = static_cast<unsigned>(system_clock::now().time_since_epoch().count());
    ::srand(now);
    size_t rnd = static_cast<size_t>(::rand());
    return min + rnd % (max - min + 1);
}

template<typename T>
const T &getRandomElement(const std::list<T> &urls)
{
    if (urls.empty())
        throw std::invalid_argument("urls.size() must be > 0");

    if (urls.size() == 1)
        return *urls.begin();

    size_t rndIndex = getFastNotSecureRandomNum(0, urls.size() - 1);

    auto it = urls.cbegin();
    std::advance(it, rndIndex);

    return *it;
}

std::list<std::string> findAllMatches(QByteArray text, const std::regex reg)
{
    std::list<std::string> foundList;

    auto matchesBegin = std::regex_token_iterator(text.cbegin(), text.cend(), reg);
    auto matchesEnd = std::regex_token_iterator<QByteArray::const_iterator>();
    std::move(matchesBegin, matchesEnd, std::back_inserter(foundList));

    return foundList;
}

}
