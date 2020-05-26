#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <sstream>
#include <utility>
#include <algorithm>
#include <cstdbool> 

using namespace std;
namespace fs = std::filesystem;

size_t findi(string data, string toSearch, size_t pos = 0)
{
	//Convert complete given String to lower case
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	// Convert complete given Sub String to lower case
	std::transform(toSearch.begin(), toSearch.end(), toSearch.begin(), ::tolower);
	// Find sub string in given string
	return data.find(toSearch, pos);
}
//Function to split a string using a char delimiter
vector<string> explode(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}
//Function to load all files into memory by using a map
map<string,string> getData() {
	map <string,string> dataset;
	int isStart, isEnd;
	string path,filename, line, title, content;
       	path = "../data/dataset/";
	for (const auto & entry : fs::directory_iterator(path)){
		filename = entry.path();
		ifstream myfile(filename);
		if (myfile.is_open()) {
			while(getline(myfile, line)){
				isStart = line.find("<doc");
				isEnd	= line.find("</doc");
				if (isStart != -1){
					string boom	=	explode(line, "\"")[3];
					title		=	boom;
					content		=	"";
				}
				else if (isEnd != -1){
					dataset[title]	=	content;
					content	= "";
				}
				else {
					if (line.length() == 0 || line.find("ENDOFARTICLE") != -1)
						continue;	
					content += line + "\n";
				}
			}
			myfile.close();
		}
	}
	return dataset;
}
//Main function
int main() {
	int qtime, occr;
	string query, line;
	map <string,string> dataset;
	
	cout << "Loading index..." 	<< endl;
	dataset = getData();
	while (true) {
		occr	=	0;
		cout << "Insert your query: ";
		getline(cin,query);

		const clock_t begin_time = clock();	
		
		for( auto const& [key, val] : dataset ){
			if (key.find(query) != -1) {

			}

		//	if (findi(val, query) != -1) {
		//		occr++;
		//	}

			if (val.find(query) != -1) {
				occr++;
			}


		}
		cout << occr << " results ("<< float( clock () - begin_time ) /  CLOCKS_PER_SEC << "s)" << endl;
	}
	return 0;
}
