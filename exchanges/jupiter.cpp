#include "jupiter.h"

double Jupiter::getPrice(const char* id)
{
    CURLcode result;

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://price.jup.ag/v6/price?ids=DhFkxvDAGCQQdfMXDw8gHfD3KKQBDPvta4tiqEXnvkat");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        result = curl_easy_perform(curl);
    
        if(result != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(result));

        // std::cout << readBuffer << std::endl;

        objJson = nlohmann::json::parse(readBuffer);
    
        curl_easy_cleanup(curl);

        double price = objJson["data"]["DhFkxvDAGCQQdfMXDw8gHfD3KKQBDPvta4tiqEXnvkat"]["price"].get<nlohmann::json::number_float_t>();
        
        return price;
    }
    return 0;
}

Jupiter::Jupiter(/* args */)
:Exchange("https://price.jup.ag/v6/")
{
    curl = curl_easy_init();
}

Jupiter::~Jupiter()
{
}