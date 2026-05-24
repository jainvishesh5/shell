#include <string>
#include <iostream>
 using namespace std;

 int main(){
    cout << unitbuf;
    cerr << unitbuf;
string command;
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

   cout << command << ": command not found" << "\n";
  }
}
