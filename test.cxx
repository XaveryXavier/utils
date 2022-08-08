#include <iostream>
#include "OfflineCalib.h"
#include <fstream>

using json = nlohmann::json;
using namespace std;



vector<map<string, string>> tableVec_prep_fun(string fileName) {
    ifstream input_file;
    input_file.open(fileName);
    json j;
    input_file >> j;
    input_file.close();
    // cout << j.dump(4) << endl;               // checking if data stream is correct

    vector<map<string, string>> tableVec;
    map<string, string> smap;

    for(json::iterator it = j.begin(); it < j.end(); ++it){   
        for(json::iterator it2 = it.value().begin(); it2 != it.value().end(); ++it2){   //iterates through json inner elements
            try {
                   smap[it2.key()] = it2.value();
                }
            catch (nlohmann::json::type_error &e) {
                    //doesn't fit the initial data type so dump it into string
                    smap[it2.key() ] = it2.value().dump();
                }
        }
    tableVec.push_back(smap);
    }
    // for(int i = 0; i < tableVec.size(); ++i){                                                                    //checking if tableVec is properlly 
    //     for(map<string, string>::iterator it = tableVec[i].begin(); it != tableVec[i].end(); ++it){              //filled with data
    //         std::cout << it->first << " : " << it->second << endl;
    //     } 
    // }
    return tableVec;
}


//comment
int main() {
    const std::vector<std::map<std::string, std::string>> data_to_upload = tableVec_prep_fun("sample_data3.json");
    // for(int i = 0; i < data_to_upload.size(); ++i){                                                                    //checking if data_to_upload is properlly 
    //     for(map<string, string>::const_iterator it = data_to_upload[i].begin(); it != data_to_upload[i].end(); ++it){              //filled with data
    //         std::cout << it->first << " : " << it->second << endl;
    //     } 
    // }
    
    

    OfflineCalib calib;
    calib.SetEndPoint("calib_hypmed_energies/");
    calib.SetUsernameAndPassword("michal", "7ujm&UJM");
    calib.WriteData(data_to_upload);
    // calib.FetchData();
    // const std::vector<std::map<std::string, std::string> > data = calib.Data();
    // calib.Print();
   
    return 0;
}
