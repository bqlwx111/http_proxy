#include <stdlib.h>
#include <string>

namespace headerParse
{
    std::string FindField(const char * buf ,const char * start,const char * end ,int bufsize)//find http header field
    {
        if(bufsize<=0)
            bufsize=strlen(buf);
        
        const char * msg_start=buf ;
        const char * msg_end=buf+bufsize;

        int len = 0;
        
        if(start!= NULL)
        {
            len=strlen(start);
            msq_start=strstr(buf,start);
        }
        if(msg_start==NULL)
            return "";
        
        msg_start += len;
	    if (end != NULL) 
        {
		    msg_end = strstr(msg_start, end);
	    	if (msg_end == NULL) 
            {
			    return "";
		    }
        }
        return string (msg_start,msg_end);
	}
}