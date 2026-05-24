#include <string>
#include <iostream>
 using namespace std;

 int main(){
    cout << unitbuf;
    cerr << unitbuf;
string command;
  string builtin_commands[] = {"echo" , "type" , "exit"};
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
     cout << arg << " not found\n";
    }
    continue;
   }

   cout << command << ": command not found" << "\n";
  }
}
