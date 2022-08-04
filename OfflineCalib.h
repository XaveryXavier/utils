#ifndef OFFLINECALIB_H
#define OFFLINECALIB_H

#include <curl/curl.h>
#include <algorithm>
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>

class OfflineCalib {
    private:
        std::vector<std::map<std::string, std::string> > container;
        std::string endpoint_url;
        std::string username_and_password;
	public:
		OfflineCalib();
        /*
         * REST API endpoint to get data from
         * @params std::endpoint
         */
		void SetEndPoint(std::string endpoint);
        /*
         * Set username and password
         * @params std::string username
         * @params std::string password
         */
		void SetUsernameAndPassword(std::string username, std::string password);
        /*
         * Downloads and parses JSON from the REST API endpoint
         */
		void FetchData();
        /* Upload or write data to the endpoint
         * @params const std::vector<std::map<std::string, std::string> > &data
         */
		void WriteData(const std::vector<std::map<std::string, std::string> > &data);
        /* Data from the endpoint
         * @return std::vector<std::map<std::string, std::string> > 
         */
        const std::vector<std::map<std::string, std::string> > Data() { return container; };
		void Print();
};
#endif /* OFFLINECALIB_H */
