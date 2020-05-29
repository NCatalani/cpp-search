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

size_t findi(string data, string toSearch, size_t pos = 0)
{
	//Convert complete given String to lower case
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	// Convert complete given Sub String to lower case
	std::transform(toSearch.begin(), toSearch.end(), toSearch.begin(), ::tolower);
	// Find sub string in given string
	return data.find(toSearch, pos);
}
//Function to find how many times a substring appear within a string
int count (const string s, const string target) {
	int 			occurrences = 0;
	string::size_type 	pos = 0;

	while ((pos = s.find(target, pos )) != std::string::npos) {
		++ occurrences;
		pos += target.length();
	}
	return occurrences;
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
		occr	=	res	=	i	=	total_occr	=	0;
		//Multimap usado pois permite keys repetidas
		//Parametro greater para ordenar desc
		multimap <int,string, greater<int>> 	results;
		map	 <string, string>		rank;
		cout << "Insert your query: ";
		getline(cin,query);

		begin_t = clock();	
		for( auto const& [title, content] : dataset ){
			occr		=	count(content, query);	
			if (occr != 0) {
				results.insert(make_pair(occr,title));
				total_occr			+=	occr;
			}
		}

		end_t = clock();
		qtime = ( float(end_t - begin_t) / CLOCKS_PER_SEC );
		cout << "...About " << total_occr << " results (" << qtime << " seconds)" << endl;
		//ELECAR OS RESULTADOS
		for(auto [occr,title] : results) {
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
	}
	return 0;
}
