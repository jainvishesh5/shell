#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sstream>
 using namespace std;

 int main(){
    cout << unitbuf;
    cerr << unitbuf;
string command;
  string builtin_commands[] = {"echo" , "type" , "exit"};

  string path = getenv("PATH");
  stringstream ss(path);
  string dir;
  vector <string>paths;
  while(getline(ss , dir , ':')){
   paths.push_back(dir);
  }
  
  while(true){
    cout << "$ ";
   
    getline(cin , command);
     
    if(command == "exit"){
    break;
  }
    if(command.rfind("echo " , 0)==0){
    cout << command.substr(5) << "\n";
    continue;
  }
   if(command.rfind("type " , 0)==0){
     string arg = command.substr(5);
    bool found = false;
    for(const string &cmd: builtin_commands){
     if(arg == cmd){
      cout << arg << " is a shell builtin\n";
       found = true;
      break;
     }
    }
    if(!found){
     for(const string &dir : paths){
      string full_path = dir + "/" + arg;
      if(access(full_path.c_str() , X_OK) == 0){
       cout << arg << " is " << full_path << "\n";
          found = true;
          break;
      }
     }
    }
    if(!found){
     cout <<arg << ": not found\n";
    }
    continue;
   }

   cout << command << ": command not found" << "\n";
  }
  return 0;
}
