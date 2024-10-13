#include <stdio.h>

#include <curl/curl.h>
#include <string>

#include <iostream>

#include <libpq-fe.h>
#include <nlohmann/json.hpp>

#include <database.h>
#include <jupiter.h>

#include <algorithm>
#include <chrono>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

int main(void)
{
    Database *database;
    database = new Database("postgres", "devs_pc", "127.0.0.1", "postgres");

    std::vector<std::string> tags;

    tags = database->get_tags("exchanges");

    for (int i = 0; i < tags.size(); i++)
    {
        std::cout << tags[i] << std::endl;
    }

    //   CURL *curl;
    //   CURLcode result;
    //   std::string readBuffer;

    //   curl = curl_easy_init();
    //   if(curl) {
    //     curl_easy_setopt(curl, CURLOPT_URL, "https://quote-api.jup.ag/v6/tokens");
    //     // curl_easy_setopt(curl, CURLOPT_URL, "https://price.jup.ag/v6/price?ids=DhFkxvDAGCQQdfMXDw8gHfD3KKQBDPvta4tiqEXnvkat");
    //     curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    //     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    //     curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    //     result = curl_easy_perform(curl);

    //     /* Check for errors */
    //     if(result != CURLE_OK)
    //       fprintf(stderr, "curl_easy_perform() failed: %s\n",
    //               curl_easy_strerror(result));

    //     // std::cout << readBuffer << std::endl;

    //     nlohmann::json objJson;

    //     objJson = nlohmann::json::parse(readBuffer);

    //     std::string token = objJson[0];

    // database->multi_insert_token_id("exchanges", objJson);

    /* always cleanup */
    // curl_easy_cleanup(curl);
    //   }

    return 0;
}