
#include<string>
#include<vector>
#include<fstream>
#include<unordered_map> 
#include "httpHeader.h"

#ifndef _HTTP_RESPONSE
#define _HTTP_RESPONSE
class httpResponse{
	public:
		httpResponse();
		~httpResponse();

		void showResponse();
		
		//void addData(char* , int&);

		int setVersion(Version );
		int setStatusCode(int );
		int setReasonPhrase();
		int setResponseBody(std::string&& );
		//int setHTTPHeader(string headerName, string headerContent);
		//int setHTTPHeaderVector(vector<pair<string, string> >* const );


		Version getVersion( );
		int getStatusCode( );
		std::string getReasonPhrase();
		std::string& getResponseBody();
		//string getHTTPHeader(string headerName);
		//vector<pair<string, string> >* getHTTPHeaderVector(void );	
		int getResponseSize();
		std::string& getResponse();

		int addHttpHeader(std::string&& key,std::string&& value);
		std::unordered_map<std::string,std::string> getHttpHeader();

		int setResponse();
	private:
		Version _Version;
		int _status;
		std::string _reasonPhrase;
		std::string _responseBody;

		std::string _responseString;

		std::unordered_map<std::string,std::string> _header_key_to_header_value;

    //===============================================
		int parseResponse(std::string&);
        
		int copyFromFile(std::ifstream&, int);
		int copyToFile(std::ofstream& );
};
#endif