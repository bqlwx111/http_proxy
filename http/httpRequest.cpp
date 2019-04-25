#include "httpRequest.h"
#include <sstream>


httpRequest::handleRequest(std::string& request_string)
{
    int beforeCursor=0;
    int afterCursor=0;

    afterCursor=request_string.find_first_of(" ",beforeCursor);    
    string httpMethod=request_string.substr(beforeCursor,afterCursor-beforeCursor);
    
    if()
}