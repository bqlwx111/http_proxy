#include "httpRequest.h"
#include <sstream>
#include <iostream>

int httpRequest::handleRequest(std::string& request_string)
{
    int beforeCursor=0;
    int afterCursor=0;

    afterCursor=request_string.find_first_of(" ",beforeCursor);    
    std::string httpMethod=request_string.substr(beforeCursor,afterCursor-beforeCursor);

    if(httpMethod=="GET")
        _Method=GET;
    else 
        if(httpMethod=="PUT")
        _Method=PUT;
    else 
        if(httpMethod=="HEAD")
        _Method=HEAD;
    else 
        if(httpMethod=="POST")
        _Method=POST;
    else 
        _Method=NOT_IMPLEMENTED;
// uri
    beforeCursor=afterCursor+1;
    afterCursor=request_string.find_first_of(" ",beforeCursor);    
    _uri=request_string.substr(beforeCursor,afterCursor-beforeCursor);

// version
    beforeCursor=afterCursor+1;
    afterCursor=request_string.find_first_of(" ",beforeCursor);    
    std::string httpMethod=request_string.substr(beforeCursor,afterCursor-beforeCursor);

    if(httpMethod=="HTTP/1.0")
        _Version=HTTP1_0;
    else 
        if(httpMethod=="HTTP/1.1")
        _Version=HTTP1_1;
    else 
        _Version=HTTP_UNSUPPORTED;
// \n
    beforeCursor=afterCursor+1;
//======================================
//header
    while(true)
    {
        afterCursor=request_string.find_first_of("\n",beforeCursor);
        if(beforeCursor==afterCursor)
        {
            beforeCursor++;
            break;
        }
        std::string requestHeader=request_string.substr(beforeCursor,afterCursor-beforeCursor);
        beforeCursor=afterCursor+1;

        int pos;
        std::string key;
        std::string value;

        pos=requestHeader.find(":");
        key=requestHeader.substr(0,pos-1);
        value=requestHeader.substr(pos+1);

        _header_key_to_header_value[key]=value;

    }
    for(auto&i : _header_key_to_header_value)
    {
        std::cout<<"key:"<<i.first()<<" value:"<<i.second()<<endl;
    }
//body
    _requestBody=request_string.substr(beforeCursor);
    return 1;
}