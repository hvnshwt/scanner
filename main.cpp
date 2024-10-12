#include <stdio.h>

#include <curl/curl.h>
#include <string>

#include <iostream>

#include <libpq-fe.h>
#include <nlohmann/json.hpp>

#include <database.h>

#include <algorithm>
#include <chrono>


#define UNUSED(x) (void)(x)

static PGconn* conn = NULL;
static PGresult* res = NULL;

static void
terminate(int code)
{
    if(code != 0)
        fprintf(stderr, "%s\n", PQerrorMessage(conn));

    if(res != NULL)
        PQclear(res);

    if(conn != NULL)
        PQfinish(conn);

    exit(code);
}

static void
processNotice(void *arg, const char *message)
{
    UNUSED(arg);
    UNUSED(message);

    // do nothing
}

static void
clearRes()
{
    PQclear(res);
    res = NULL;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


int main(void)
{ 
  Database *database;
  database = new Database("postgres", "devs_pc", "127.0.0.1", "postgres");

  CURL *curl;
  CURLcode result;
  std::string readBuffer;
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://quote-api.jup.ag/v6/tokens");
    // curl_easy_setopt(curl, CURLOPT_URL, "https://price.jup.ag/v6/price?ids=DhFkxvDAGCQQdfMXDw8gHfD3KKQBDPvta4tiqEXnvkat");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    result = curl_easy_perform(curl);
 
    /* Check for errors */
    if(result != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(result));

    // std::cout << readBuffer << std::endl;

    nlohmann::json objJson;

    objJson = nlohmann::json::parse(readBuffer);
    
    std::string token = objJson[0];
    
    const int range = objJson.size();

    std::string multi_row_query;
    // multi_row_query.resize(49 * range);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    // составление строки вида "(0, 0), (0, 0), (0, 0), (0, 0);"
    for (int i = 0; i < range; i++)
    {
      multi_row_query.append("('" + objJson[i].dump().substr(1, objJson[i].dump().size() -2) + "')");
      if (i != range - 1)
        multi_row_query.append(",");
    }
    // std::replace(multi_row_query.begin(), multi_row_query.end(), '"', '\'');
    database->multi_insert_token_id("exchanges", multi_row_query);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Diff(ms) = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;

 
    /* always cleanup */
    curl_easy_cleanup(curl);
  }

  clearRes();
  terminate(0);
  return 0;
}