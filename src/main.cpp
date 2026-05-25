#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sstream>
#include <sys/wait.h>
 using namespace std;

 int main(){
    cout << unitbuf;
    cerr << unitbuf;
string command;
  string builtin_commands[] = {"echo" , "type" , "exit"};
  string home = getenv("HOME");
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
   stringstream cmd_stream(command);
   string arg;
   vector<string> args;
   while(cmd_stream >> arg){
    args.push_back(arg);
   }

   vector <char*> argv;
   for(string& arg:args){
    argv.push_back(arg.data());
   }
   argv.push_back(nullptr);

   if(args.empty())continue;
     
    if(args[0] == "exit"){
    break;
  }
    if(args[0] == "echo"){
    for(size_t i=1 ; i<args.size() ; i++){
     cout << args[i] <<" ";
    }
     cout << "\n";
    continue;
  }
   if(args[0] == "type"){
    if(args.size() < 2){
     cerr <<"type missing arguments\n";
     continue;
    }
     string arg = args[1];
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

   if(args[0] == "pwd"){
    char cwd[1024];
    if(getcwd(cwd , sizeof(cwd)) == nullptr){
     perror("pwd");
     continue;
    }
    cout << cwd <<"\n";
    continue;
   }

   if(args[0] == "cd"){
    if(args.size() < 2){
     cerr << "cd: missing argument\n";
     continue;
    }
    if(args[1] == "~"){
     if(chdir(home.c_str()) != 0){
      perror("cd");
     }
     continue;
    }
    if(chdir(args[1].c_str())!=0){
     string error_msg = "cd: "+ args[1];
     perror(error_msg.c_str());
    }
    continue;
   }

   pid_t pid = fork();
   if(pid == 0){
    execvp(argv[0] , argv.data());
    cerr << command << ": command not found\n";
    exit(1);
   }
   else{
    waitpid(pid , nullptr , 0);
   }
  }
  return 0;
}
