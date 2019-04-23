
#include<string>
#include<vector>
#include<fstream>




class httpResponse{
	public:
		httpResponse();
		~httpResponse();

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
		std::string getReasonPhrase(void );
		std::string getResponseBodyPtr(void );
		//string getHTTPHeader(string headerName);
		//vector<pair<string, string> >* getHTTPHeaderVector(void );	
		size_t getResponseSize(void );
		std::string getResponseDataPtr(void );


	private:
		Version _Version;
		int _status;
		std::string reasonPhrase;
		std::string responseBody;

		std::string data;

    //===============================================
		int parseResponse(void );
		int prepareResponse(void );
        
		int copyFromFile(std::ifstream&, int);
		int copyToFile(std::ofstream& );
};
