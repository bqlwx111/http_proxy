#include <iostream>
#include "httpResponse.h"

httpResponse::httpResponse()
{

}

httpResponse::~httpResponse()
{

}
void httpResponse::showResponse()
{
    std::cout<<"---Response Begin---"<<std::endl<<_responseString
    <<"\n---Response End---"<<std::endl;
}

/*
void httpResponse::addData(char* buf ,int& len)
{
    _responseString.append(buf,len);
}
*/

int httpResponse::setVersion(Version version)
{
    _Version=version;
    return 1;
}

Version httpResponse::getVersion()
{
    return _Version;
}

int httpResponse::setStatusCode(int code)
{
    _status=code;
    setReasonPhrase();
    return 1;
}

int httpResponse::getStatusCode()
{
    return _status;
}

int httpResponse::setReasonPhrase()
{
    switch(_status)
    {
		case 200:
			_reasonPhrase = "OK";
			break;
		case 201:
			_reasonPhrase = "Created";
			break;
		case 400:
			_reasonPhrase = "Bad Request";
			break;
		case 403:
			_reasonPhrase = "Forbidden";
			break;
		case 404:
			_reasonPhrase = "Not Found";
			break;
		case 411:
			_reasonPhrase = "Length Required";
			break;
		case 500:
			_reasonPhrase = "Internal Server Error";
			break;
		case 501:
			_reasonPhrase = "Not Implemented";
			break;
		case 502:
			_reasonPhrase = "Bad Gateway";
			break;
		case 505:
			_reasonPhrase = "HTTP Version Not Supported";
			break;
		default:
			return -1;
			break;
	}
    return 1;  
}

std::string httpResponse::getReasonPhrase()
{
    return _reasonPhrase;
}

int httpResponse::addHttpHeader(std::string&& key,std::string&& value)
{
    _header_key_to_header_value[std::move(key)]=std::move(value);
    return 1;   
}

std::unordered_map<std::string,std::string> httpResponse::getHttpHeader()
{
    return _header_key_to_header_value;
}

int httpResponse::setResponseBody(std::string&& body)
{
    _responseBody=std::move(body);
}

std::string& httpResponse::getResponseBody()
{
    return _responseBody;
}

int httpResponse::setResponse()
{
    Version version;
    std::string temp;

	switch(version)
    {
		case HTTP1_0:
			temp= "HTTP/1.0";
			break;
		case HTTP1_1:
			temp= "HTTP/1.1";
			break;
		default:
			return -1;
			break;
	}

	_responseString+=temp;
    _responseString+=" "+std::to_string(_status);
    _responseString+=" "+_reasonPhrase+"\r\n";

    for(auto&i : _header_key_to_header_value)
        _responseString+=i.first+": "+i.second+"\r\n";
	
    _responseString+="\r\n";

    _responseString+=_responseBody;

    _responseString+="\r\n";
	return 0;
 
}
/*
        Response = Status-Line CRLF
	   (Response-Header CRLF)*
	   CRLF
	   [message-body]

	   Status-Line = HTTP/1.0 <space> Status-Code <space> Reason-Phrase CRLF
*/

int httpResponse::parseResponse(std::string& response_string)
{
    int beforeCursor=0;
    int afterCursor=0;

    afterCursor=response_string.find_first_of(" ",beforeCursor);    
    std::string httpVersion=response_string.substr(beforeCursor,afterCursor-beforeCursor);

  	if(httpVersion == "HTTP/1.0")
		_Version= HTTP1_0;
    else
        if(httpVersion == "HTTP/1.1")
        _Version = HTTP1_1;
	else
    {
		_Version = HTTP_UNSUPPORTED;
		return 0;
    }
// uri
    beforeCursor=afterCursor+1;
    afterCursor=response_string.find_first_of(" ",beforeCursor);    
    _status=std::atoi(response_string.substr(beforeCursor,afterCursor-beforeCursor).c_str());

// version
    beforeCursor=afterCursor+1;
    afterCursor=response_string.find_first_of(" ",beforeCursor);    
    _reasonPhrase=response_string.substr(beforeCursor,afterCursor-beforeCursor);
// \n
    beforeCursor=afterCursor+1;
//======================================
//header
    while(true)
    {
        afterCursor=response_string.find_first_of("\n",beforeCursor);
        if(beforeCursor==afterCursor)
        {
            beforeCursor++;
            break;
        }
        std::string requestHeader=response_string.substr(beforeCursor,afterCursor-beforeCursor);
        beforeCursor=afterCursor+1;

        int pos;
        std::string key;
        std::string value;

        pos=requestHeader.find(":");
        key=requestHeader.substr(0,pos);
        value=requestHeader.substr(pos+1);

        _header_key_to_header_value[key]=value;

    }
    /*
    for(auto&i : _header_key_to_header_value)
    {
        std::cout<<"key:"<<i.first<<" value:"<<i.second<<std::endl;
    }
//body
    _requestBody=request_string.substr(beforeCursor);
    std::cout<<"body"<<_requestBody<<std::endl;
    */
    return 1;
}

int httpResponse::getResponseSize()
{
    return _responseString.size();
}

std::string& httpResponse::getResponse()
{
    return _responseString;
}









