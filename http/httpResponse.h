
#include<string>
#include<vector>
#include<fstream>

#include"HTTP.h"


class HttpResponse{
	public:
		HTTPResponse();
		~HTTPResponse();

		void showResponse(void );
		void addData(const char *, const int&);
	
		int setVersion(Version );
		int setStatusCode(int );
		int setReasonPhrase(void );
		int setResponseBody(std::string& );
		//int setHTTPHeader(string headerName, string headerContent);
		//int setHTTPHeaderVector(vector<pair<string, string> >* const );


		Version getProtocol(void );
		int getStatusCode(void );
		string getReasonPhrase(void );
		string getResponseBodyPtr(void );
		//string getHTTPHeader(string headerName);
		//vector<pair<string, string> >* getHTTPHeaderVector(void );	
		size_t getResponseSize(void );
		string* getResponseDataPtr(void );


	private:
		Version _Version;
		int _status;
		std::string reasonPhrase;
		std::string responseBody;

		std::string data;

    //===============================================
		int parseResponse(void );
		int prepareResponse(void );
        
		int copyFromFile(ifstream&, int);
		int copyToFile(ofstream& );
};
