#include <curl/curl.h>
#include <algorithm>
#include <iostream>
#include <string>
#include "nlohmann/json.hpp"

#include "OfflineCalib.h"
OfflineCalib::OfflineCalib() {}
void OfflineCalib::SetEndPoint(std::string endpoint) {
    // endpoint_url = "https://bragg.if.uj.edu.pl/SiFiCCData/Prototype/api/" + endpoint;
    endpoint_url = "http://127.0.0.1:8001/Prototype/api/" + endpoint;
}
void OfflineCalib::SetUsernameAndPassword(std::string username, std::string password) {
    username_and_password = username + ":" + password;
}
void OfflineCalib::FetchData() {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, endpoint_url.c_str() );
        curl_easy_setopt(curl, CURLOPT_USERPWD, username_and_password.c_str() );
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](void *contents, size_t size, size_t nmemb, void *userp) -> size_t {
            ((std::string*)userp)->append((char*)contents, size * nmemb);
            return size * nmemb;
        });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        //parse JSON into vector<map<str, str> >
        nlohmann::json j = nlohmann::json::parse(readBuffer);
        for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
            std::map<std::string, std::string> m;
            for(nlohmann::json::iterator it2 = it.value().begin(); it2 != it.value().end(); ++it2) {
                try {
                    m[it2.key() ] = it2.value();
                } catch (nlohmann::json::type_error &e) {
                    //doesn't fit the initial data type so dump it into string
                    m[it2.key() ] = it2.value().dump();
                }
            }
            container.push_back(m);
        }
    }
}
void OfflineCalib::WriteData(const std::vector<std::map<std::string, std::string> > &data) { 

nlohmann::json jout;

    for(int i = 0; i < data.size(); ++i){
        for(std::map<std::string, std::string>::const_iterator it = data[i].begin(); it != data[i].end(); ++it){
            nlohmann::json json_holder;
            json_holder[it->first] = it->second;
            jout.push_back(json_holder);
        }    
    }
    CURLcode res = CURLE_FAILED_INIT;
    CURL* curl = curl_easy_init();
    std::string url(endpoint_url);
    std::string string_holder = jout.dump();
    struct curl_slist* headers = NULL;

     if(curl){

        headers = curl_slist_append(headers, "Expect:");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        std::string header = "Content-Type: application/json";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERPWD, username_and_password.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, string_holder.c_str());
        res = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

}
void OfflineCalib::Print() {
    std::cout << "[ " << std::endl;
	for(std::vector<std::map<std::string, std::string> >::const_iterator it = container.begin(); it != container.end(); ++it) {
		std::cout << "{ ";
		for(std::map<std::string, std::string>::const_iterator it2 = it->begin(); it2 != it->end(); ++it2) {
			std::cout << it2->first << ": " << it2->second << ", ";
		}
		std::cout << " }, ";
	}
	std::cout << " ]" << std::endl;
}
