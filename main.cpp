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

    Jupiter *jupiter_api;
    jupiter_api = new Jupiter();

    std::vector<std::string> tags;
    tags = database->get_tags("exchanges");

    std::vector<double> prices;

    for (int i = 0; i < tags.size(); i++)
    {
        prices.push_back(jupiter_api->getPrice(tags[i]));
        std::cout << tags[i] << " - " << jupiter_api->getPrice(tags[i]) << std::endl;
    }

    database->update_prices("exchanges", tags, prices);

    return 0;
}