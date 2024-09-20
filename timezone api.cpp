#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"

using namespace std; 
using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* userp) 
{
    size_t totalSize = size * nmemb;
    userp->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

int main() 
{
    CURL* curl;
    CURLcode res;
    string readBuffer;

    string timezone;
    cout << "Enter a timezone ";
    cin >> timezone;

    string apiUrl = "http://worldtimeapi.org/api/timezone/" + timezone;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) 
	{
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) 
		{
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        } 
		else 
		{
            cout << "Response from API:\n" ;
            
            try 
            {
                json jsonResponse = json::parse(readBuffer);
                string datetime = jsonResponse["datetime"];
                string timezoneResponse = jsonResponse["timezone"];
                
                cout << "Current datetime: " << datetime << endl;
                cout << "Timezone: " << timezoneResponse << endl;
            } 
            catch (json::parse_error& e) 
            {
                cerr << "JSON parse error: " << e.what() << endl;
            }
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}
