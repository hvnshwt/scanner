#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include <libpq-fe.h>

int main(void)
{
  std::cout << "Blya" << std::endl;
  int libpq_ver = PQlibVersion();
  printf("Version of libpq: %d\n", libpq_ver);

  CURL *curl;
  CURLcode res;
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://price.jup.ag/v6/price?ids=DhFkxvDAGCQQdfMXDw8gHfD3KKQBDPvta4tiqEXnvkat");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
    /* Perform the request, res gets the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  return 0;
}