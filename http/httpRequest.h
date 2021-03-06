#include <string>
#include "httpHeader.h"
#include <unordered_map>
#include <iostream>
#include <thread>
#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H



class httpRequest
{
    public:
        httpRequest(){}
        ~httpRequest(){_header_key_to_header_value.clear();}

        //httpRequest(const httpRequest&& request){}

        void showRequest()
        {
            std::cout<<"--request: --\n"<<_request_string<<std::endl;
            //std::cout<<"--request--::\n "<<std::this_thread::get_id()<<std::endl;
        }

        Method getMethod(){return _Method;}
        Version getVersion(){return _Version;}
        std::string getUri(){return _uri;}
        std::string getUserAgent(){return _userAgent;}
        std::string getHostName(){return _hostName;}
        std::string getRequestBody(){return _requestBody;}
        std::string getData(){return _data;}

        int setVersion(Version& v)   {_Version=v; return 1;}
        int setMethod(Method& m) {_Method=m;return 1;}
        int setUrl(std::string& uri)    {_uri=uri;return 1;}
        int setUserAgent(std::string& userAgent) {_userAgent=userAgent;return 1;}
        int setHostName(std::string& hostName)   {_hostName=hostName; return 1;}
        int setRequestBody(std::string& body)    {_requestBody=body; return 1;}
        int setData(std::string& data)  {_data=data; return 1;}

        int parseRequest(std::string& request_string);
    private:
        Version _Version;
        Method _Method;
        std::string _uri;
        std::string _userAgent;
        std::string _hostName;
        std::unordered_map<std::string,std::string> _header_key_to_header_value;
        std::string _requestBody;
        std::string _request_string;
        std::string _data;
};

#endif 