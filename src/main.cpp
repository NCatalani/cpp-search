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
#include <vector>

using namespace std;
namespace fs = std::filesystem;

bool findString(const std::string & strHaystack, const std::string & strNeedle){
	  auto it = std::search(
			      strHaystack.begin(), strHaystack.end(),
			      strNeedle.begin(),   strNeedle.end(),
			      [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); } );

	    return (it != strHaystack.end() );
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
void viewContent(string content){
	int			i;
	istringstream 		stream(content);
	string 			line, cont;
	i	=	0;
	while (getline(stream,line)) {
		i++;
		cout << line << endl;
		if (i == 20)
			break;
	}
}

//Main function
int main() {
	int 		total_occr, occr, res, i;
	float 		qtime;
	string 		query, linex, more, realTitle;
	map 		<string,string> dataset;
	clock_t 	begin_t, end_t;

	printf("\033c");
	cout << "...loading index" << flush;
	dataset = getData();
	cout << ", done!" << endl;
	while (true) {
		occr	=	i	=	0;
		map <string,string> 	results;
		map <string,string>	rank;

		cout << "Insert your query: ";
		getline(cin,query);

		begin_t = clock();	
		for( auto const& [title, content] : dataset ){
			if (findString(content, query) == true) {
				occr++;
			}
			if (findString(title, query) == true) {
				results[title];
			}
		}

		end_t = clock();
		qtime = ( float(end_t - begin_t) / CLOCKS_PER_SEC );
		cout << "...About " << occr << " results (" << qtime << " seconds)" << endl;

		for(auto [title, foo] : results) {
			i++;
			rank[to_string(i)] = title;

			cout << "[" << i << "] " << title << endl; 
			if (i%20 == 0){
				cout << "Do you want to open any result [n or result number]?  ";
				getline(cin,more);	
				
				if (more == "n"){
					printf("\033c");
					continue;
				}
				else{
					realTitle	=	rank[more];

					cout << realTitle << endl;
					viewContent(dataset[realTitle]);
					break;
				}
			}
		}
		
		cout << "Do you want do a new search(y/n)?  ";
		getline(cin,more);
		if (more == "n")
			break;
		
		printf("\033c");
	}
	return 0;
}
