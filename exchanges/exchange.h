#pragma once

#include <tuple>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

class Exchange
{
private:
    // data
public:
    // Exchange(std::string&& baseAPI) : baseAPI(std::move(baseAPI)){};
    double getPrice(const std::string tag);
    virtual ~Exchange() = default;
protected:
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
protected:
    const std::string baseAPI;
    CURL *curl;
};