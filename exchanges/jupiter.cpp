#include "jupiter.h"
#include <iostream>

double Jupiter::getPrice(const std::string &tag)
{
    nlohmann::json objJson = NULL;
    std::string readBuffer;
    CURLcode result;

    if(curl) {
        std::string url = "https://price.jup.ag/v6/price?ids=" + tag;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        result = curl_easy_perform(curl);
    
        if(result != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(result));

        objJson = nlohmann::json::parse(readBuffer);

        curl_easy_reset(curl);

        double price = objJson["data"][tag]["price"].get<nlohmann::json::number_float_t>();
        
        return price;
    }
    return 0;
}

nlohmann::json Jupiter::getAllTokens()
{
    nlohmann::json objJson = NULL;
    std::string readBuffer;
    CURLcode result;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://quote-api.jup.ag/v6/tokens");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        result = curl_easy_perform(curl);

        if (result != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(result));

        objJson = nlohmann::json::parse(readBuffer);
    }
    curl_easy_reset(curl);
    return objJson;
}

Jupiter::Jupiter(/* args */)
{
    curl = curl_easy_init();
}

Jupiter::~Jupiter()
{
    curl_easy_cleanup(curl);
}