#include "../../lib/oatpp/web/server/api/ApiController.hpp"
#include "../../lib/oatpp/core/macro/codegen.hpp"
#include "../../lib/oatpp/core/macro/component.hpp"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class MyController : public oatpp::web::server::api::ApiController {
protected:
    MyController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
    {}
public:
    
    static std::shared_ptr<MyController> createShared(const std::shared_ptr<ObjectMapper>& objectMapper = nullptr){
        return std::shared_ptr<MyController>(new MyController(objectMapper));
    }
    
    /**
     *  Begin ENDPOINTs generation ('ApiController' codegen)
     */
    #include OATPP_CODEGEN_BEGIN(ApiController)
    
    ENDPOINT("GET", "/", root) {
        return createResponse(Status::CODE_200, "XD");
    }
    
    ENDPOINT("GET", "/FScounter", fscounter) {
        string countS;
        ifstream myfileread ("amk.txt");
        if (myfileread.is_open())
        {
            getline (myfileread,countS);
            myfileread.close();
        }
        
        int count = 0;
        if(countS != "")
            count = stoi(countS);
        else
            countS = "0";
        
        ofstream myfile ("amk.txt");
        if (myfile.is_open())
        {
            count++;
            myfile << count;
            myfile.close();
        }
        else cout << "Unable to open file";
        
        return createResponse(Status::CODE_200, ("counter: "+countS).c_str());
    }
    
    int memcount = 0;
    
    ENDPOINT("GET", "/MEMcounter", memcounter) {
        return createResponse(Status::CODE_200, ("counter: " + to_string(memcount++)).c_str());
    }
    
    int pc = 0;
    ENDPOINT("GET", "/DICKcount", pcounter) {
        pc++;
        string dick = "8";
        for(int i = 0; i < pc; i++) {
            dick.append("=");
        }
        dick.append("D");
        return createResponse(Status::CODE_200, (dick).c_str());
    }
    /* put your endpoints here */
    
    /**
     *  Finish ENDPOINTs generation ('ApiController' codegen)
     */
    #include OATPP_CODEGEN_END(ApiController)
    
};
