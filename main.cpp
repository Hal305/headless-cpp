#define CURL_STATICLIB
#include <iostream>
#include <curl/curl.h>
#include "nlohmann/json.hpp"

using json = nlohmann::json;


//https://stackoverflow.com/a/9786295
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


int main(void)
{
    const char* url = "https://www.wp-hosting.no/wp-json/wp/v2/posts";
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        readBuffer.clear();
        //https://curl.se/libcurl/c/curl_easy_setopt.html
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        json j = json::parse(readBuffer);

        //std::cout << j.dump(2) << std::endl; 

        for (auto& el : j.items())
        {
            std::cout << el.key() << "\n";
            std::cout << el.value().find("title").value().front() << "\n";
            std::cout << el.value().find("date").value() << "\n";
            std::cout << el.value().find("id").value() << "\n";
        }

        /* always cleanup */
        curl_easy_cleanup(curl);

    }
    return 0;
}