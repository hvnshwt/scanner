#include <tuple>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

class Exchange
{
private:
    // data
public:
    Exchange(std::string&& baseAPI) : baseAPI(std::move(baseAPI)){};
    virtual double getPrice(const char* id) const = 0;
    virtual ~Exchange() = default;
protected:
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
protected:
    const std::string baseAPI;
    CURL *curl;
    std::string readBuffer;
    nlohmann::json objJson = NULL;
};