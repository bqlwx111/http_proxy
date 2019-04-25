#include <stdlib.h>
#include <string>

#include <cstring>

namespace headerParse
{
    std::string FindField(const char * buf ,const char * start,const char * end ,int bufsize)//find url field
    {
        if(bufsize<=0)
            bufsize=std::strlen(buf);
        
        const char * msg_start=buf ;
        const char * msg_end=buf+bufsize;

        int len = 0;
        
        if(start!= NULL)
        {
            len=std::strlen(start);
            msg_start=std::strstr(buf,start);
        }
        if(msg_start==NULL)
            return "";
        
        msg_start += len;
	    if (end != NULL) 
        {
		    msg_end = std::strstr(msg_start, end);
	    	if (msg_end == NULL) 
            {
			    return "";
		    }
        }
        return std::string (msg_start,msg_end);
	}
}