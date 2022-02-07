

//
//  main.cpp
//  assignment_1_Strings
//
//  Created by Rami Adel on 06.02.22.
//  Copyright © 2022 Rami Adel. All rights reserved.
//


#include <iostream>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

/*FUNCTION TO HANDLE EACH LINE INDIVDUALLY
 TO EXTRACT THE EXTENSION AND THE SIZE
 */
void line_handles (string line , map<string,int>&m);

/*
 FUNCTION TO PRINT THE FORMATTED OUTPUT
 */
void printOutput  (map<string,int>&m);

int main() {
    //OPEN THE INPUT FILE AND READ FROM IT INSTEAD OF THE STANDARD INPUT
    //freopen("/Users/me-mac/Desktop/assignement_1_Strings/input.txt", "r", stdin);
    
    //GET THE [ TESTCASES & QUERIES ] LINE
    string line;
    stringstream ss;
    int testCases ;
    getline(cin,line);
    ss.str(line);
    
    //EXTRACT THE NUMBER OF TEST CASES & THE SIZE OF EACH CASE
    ss >> testCases ;
    
    //USE A VECTOR OF SIZE #testCases TO STORE THE SIZES OF EACH QUERY
    vector<int>sizes(testCases);

    //USE A MAP TO STORE THE SIZE SUMMITION OF EACH EXTENSION [KEY --> EXTENSION , VALUE --> FILE_SIZE]
    map <string ,int > m ;
    
    
    int i = 0 ;
    
    //EXTRACT THE SIZE OF EACH QUERY
    while(ss >> sizes[i]){
        
        //HANDLE EACH LINE OF EACH QUERY INDIVIDUALLY
        for (int j = 0 ; j < sizes[i] ; j ++){
            //GET THE FULL LINE TILL THE NEWLINE CHARACTER
            getline(cin,line);
            line_handles(line,m);
        }
        
        //ITERATE OVER THE MAP TO MATCH THE EXTENSIONS TO THE FORMATTED OUTPUT EXTENSIONS
        for(auto it = m.begin(); it != m.end() ; it++ ){
            string ext = it->first;
            if(ext == "mp3"|| ext == "aac" || ext == "flac" ){
                m["music"] += m[ext];
            }
            else if(ext == "jpg"|| ext == "bmp" || ext == "gif" ){
                m["images"] += m[ext];
            }
            else if (ext == "mp4"|| ext == "mkv" || ext == "avi" ){
                m["movies"] += m[ext];
            }
            else if(ext !="music" && ext !="images" && ext !="movies" && ext !="other" && (ext == "7z"||ext =="txt"||ext=="zip")){
                m["other"] += m[ext];
            }
        }
        printOutput(m);
        //CLEAR THE MAP TO THE NEXT QUERY
        m.clear();
        
        i++;
    }
   


}


void line_handles (string input, map<string,int> &m){
    
    stringstream ss(input);
    string line ,string_size ;
    int int_size ;
    getline(ss, line);

    size_t idx = line.find(".") , lastDotidx= idx ;

    //SKIP ALL '.' CHARACTER TILL THE LAST '.' TO GET THE EXTENSION CORRECTLY
    while(idx !=string::npos){
        idx = line.find(".", idx+1);
        if(idx!=-1) lastDotidx = idx;
    }

    line = line.substr(lastDotidx);
    string extension {};
    
    int i = 1;
    //START FROM THE CHARACTER RIGHT AFTER THE '.' CHARACTER TO GET THE EXTENSION
    while(line[i]!=' '){
        extension += line[i];
        i++;
    }
    
    //CONTINUE FROM THE CHARACTER AFTER THE SPACE CHARACTER TO GET THE SIZE WITHOUT 'b'
    while(line[i] != 'b' && i < line.size()){
        string_size += line[i];
        i++;
    }
    int_size = stoi(string_size);
    m[extension] += int_size ;
    
}

void printOutput(map<string,int>&m){
    cout<<"music "<< m["music"] << "b "<< "images " << m["images"]<<"b "<<"movies "<<m["movies"]<< "b "<<"other "<<m["other"]<<"b"<<endl;
}

