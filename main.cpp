#include <stdio.h>

#include <curl/curl.h>
#include <string>

#include <iostream>

#include <libpq-fe.h>
#include <nlohmann/json.hpp>

#include <database.h>


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
  // int libpq_ver = PQlibVersion();
  // printf("Version of libpq: %d\n", libpq_ver);

  conn = PQconnectdb("user=postgres password=devs_pc host=127.0.0.1 dbname=postgres");
  if(PQstatus(conn) != CONNECTION_OK)
  terminate(1);
  PQsetNoticeProcessor(conn, processNotice, NULL);

  int server_ver = PQserverVersion(conn);
  char *user = PQuser(conn);
  char *db_name = PQdb(conn);

  printf("Server version: %d\n", server_ver);
  printf("User: %s\n", user);
  printf("Database name: %s\n", db_name);

  // res = PQexec(conn, "INSERT INTO exchanges (token_id, tag, price) VALUES ('DhFkxvDAGCQQdfMXDw8gHfD3KKQBDPvta4tiqEXnvkat', 'PLANECAT', 0.00000285487);");
  // res = PQexec(conn, "ALTER DATABASE scannerdb RENAME TO postgres;");
  res = PQexec(conn, "DELETE FROM exchanges *;");
  if(PQresultStatus(res) != PGRES_COMMAND_OK)
    terminate(1);
  clearRes();

  int nrows = PQntuples(res);
  printf("Total: %d rows\n", nrows);

  CURL *curl;
  CURLcode result;
  std::string readBuffer;
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://price.jup.ag/v6/price?ids=DhFkxvDAGCQQdfMXDw8gHfD3KKQBDPvta4tiqEXnvkat");
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
    
    double price = objJson["data"]["DhFkxvDAGCQQdfMXDw8gHfD3KKQBDPvta4tiqEXnvkat"]["price"].get<nlohmann::json::number_float_t>();

    std::cout << price << std::endl;

 
    /* always cleanup */
    curl_easy_cleanup(curl);
  }

  clearRes();
  terminate(0);
  return 0;
}