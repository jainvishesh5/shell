#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sstream>
#include <sys/wait.h>
#include <fcntl.h>
    using namespace std;

    struct Redirection
    {
        bool redirect_stdout = false;
        string stdout_file;
        bool redirect_stderr = false;
        string stderr_file;
        bool append_stdout = false;
        bool append_stderr = false;
    };
    
    const string builtin_commands[] = {"echo" , "type" , "exit" , "pwd" , "cd"};
    const string home = getenv("HOME");
    vector <string> getpaths(){
        string path = getenv("PATH");
        stringstream ss(path);
        string dir;
        vector <string> paths;
        while(getline(ss , dir , ':')){
            paths.push_back(dir);
        }
        return paths;
    }
    int get_flags(const bool append){
        int flags = O_WRONLY | O_CREAT;
        if(append){
            flags |= O_APPEND;
        }
        else{
            flags |= O_TRUNC;
        }    
        return flags;
    }
    
    vector<string> tokenize(const string &command){
        string arg = "";
        vector<string> args;
        bool in_single_quotes = false;
        bool in_double_quotes = false;

        for(size_t i=0 ; i<command.size() ;i++){
            if(command[i] == '\\' && !in_single_quotes){
                if(i+1 < command.size()){
                    arg += command[i+1];
                    i++;
                    continue;
                }
            }
            if(command[i] == '\'' && !in_double_quotes){
                in_single_quotes = !in_single_quotes;
                continue;
            }
            if(command[i] == '"' && !in_single_quotes){
                in_double_quotes = !in_double_quotes;
                continue;
            }
            if(in_single_quotes || in_double_quotes){
                arg += command[i];
                continue;
            }
            else{
                if(command[i] == ' '){
                    if(!arg.empty()){
                        args.push_back(arg);
                        arg = "";
                    }
                }
                else{
                    arg += command[i];
                }
            }
        }
        if(!arg.empty()){
            args.push_back(arg);
        }
        if(in_single_quotes){
            cerr << "Error: unmatched single quote\n";
            return{};
        }
        if(in_double_quotes){
            cerr << "Error: unmatched double quote\n";
            return{};
        }
        return args;
    }

    bool is_builtin(const string &cmd){
        for(const string &builtin : builtin_commands){
            if(cmd == builtin){
                return true;
            }
        }
        return false;
    }

    bool run_builtin(const vector<string> &args , const vector<string> &paths , const Redirection &redir){
        bool should_continue = true;
        if(args[0] == "exit"){
            should_continue = false;
        }
        int saved_stdout;
        int saved_stderr;
        if(redir.redirect_stdout){
            int flags = get_flags(redir.append_stdout);
            saved_stdout = dup(STDOUT_FILENO);
            int fd = open(redir.stdout_file.c_str() ,flags, 0644);
            if(fd < 0){
                perror("open");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if(redir.redirect_stderr){
            int flags = get_flags(redir.append_stderr);
            saved_stderr = dup(STDERR_FILENO);
            int fd = open(redir.stderr_file.c_str() ,flags, 0644);
            if(fd < 0){
                perror("open");
                exit(1);
            }
            dup2(fd, STDERR_FILENO);
            close(fd);
        }

        if(args[0] == "echo"){
            for(size_t i=1 ; i<args.size() ; i++){
                if(i != args.size()-1){
                    cout << args[i] <<" ";
                }
                else{
                    cout << args[i];
                }
            }
            cout << "\n";
            
        }
   
        if(args[0] == "type"){
            if(args.size() < 2){
                cerr <<"type missing arguments\n";
                
            }
            else{
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
            }
        }

        if(args[0] == "pwd"){
            char cwd[1024];
            if(getcwd(cwd , sizeof(cwd)) == nullptr){
                perror("pwd");    
            }
            else{
                cout << cwd <<"\n";
            }    
        }

        if(args[0] == "cd"){
            if(args.size() < 2){
                cerr << "cd: missing argument\n";    
            }
            else if(args[1] == "~"){
                if(chdir(home.c_str()) != 0){
                    perror("cd");
                }    
            }
            else if(chdir(args[1].c_str())!=0){
                string error_msg = "cd: "+ args[1];
                perror(error_msg.c_str());
            }    
        }
        if(redir.redirect_stdout){
            dup2(saved_stdout , STDOUT_FILENO);
            close(saved_stdout);
        }
        if(redir.redirect_stderr){
            dup2(saved_stderr , STDERR_FILENO);
            close(saved_stderr);
        }
        return should_continue;
    }

    vector <char*> build_argv(const vector<string> &args){
        vector <char*> argv;
        for(const string &arg:args){
            argv.push_back((char*)arg.data());
        }
        argv.push_back(nullptr);
        return argv;
    }

    void run_external(vector <char*> &argv , const Redirection &redir){
        pid_t pid = fork();
        if(pid < 0){
            perror("fork");
            return;
        }
        if(pid == 0){
            if(redir.redirect_stdout){
                int flags = get_flags(redir.append_stdout);
                int fd = open(redir.stdout_file.c_str() ,flags, 0644);
                if(fd < 0){
                    perror("open");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            if(redir.redirect_stderr){
                int flags = get_flags(redir.append_stderr);
                int fd = open(redir.stderr_file.c_str() ,flags, 0644);
                if(fd < 0){
                    perror("open");
                    exit(1);
                }
                dup2(fd, STDERR_FILENO);
                close(fd);
            }
            
            execvp(argv[0] , argv.data());
            cerr << argv[0] << ": command not found\n";
            exit(1);
        }
        else{
            waitpid(pid , nullptr , 0);
        }
    }

    Redirection parse_redirection(vector<string> &args){
        Redirection redir;
        for(int i=args.size()-1 ; i>=0 ; i--){
            if(args[i] == ">" || args[i] == "1>"){
                if(i+1 >= args.size()){
                    cerr << "no output file\n";
                    break;
                }
                redir.redirect_stdout = true;
                redir.stdout_file = args[i+1];
                args.resize(i);
                break;
            }
            if(args[i] == "2>"){
                if(i+1 >= args.size()){
                    cerr << "no output file\n";
                    break;
                }
                redir.redirect_stderr = true;
                redir.stderr_file = args[i+1];
                args.resize(i);
                break;
            }
            if(args[i] == ">>" || args[i] == "1>>"){
                if(i+1 >= args.size()){
                    cerr << "no output file\n";
                    break;
                }
                redir.redirect_stdout = true;
                redir.stdout_file = args[i+1];
                redir.append_stdout = true;
                args.resize(i);
                break;
            }
            if(args[i] == "2>>"){
                if(i+1 >= args.size()){
                    cerr << "no output file\n";
                    break;
                }
                redir.redirect_stderr = true;
                redir.stderr_file = args[i+1];
                redir.append_stderr = true;
                args.resize(i);
                break;
            }
        }
        return redir;
    }
    

    int main(){
        cout << unitbuf;
        cerr << unitbuf;
        
        string command;
        vector<string> paths = getpaths();
  
        while(true){
            cout << "$ ";
            getline(cin , command);
            
            vector<string> args = tokenize(command);
            
            if(args.empty()){
                continue;
            }

            Redirection redir = parse_redirection(args);

            if(is_builtin(args[0])){
                if(!run_builtin(args , paths ,redir)){
                    break;
                }   
                continue;
            }

            vector <char*> argv = build_argv(args);
            run_external(argv,redir);
        }
        return 0;
    }
