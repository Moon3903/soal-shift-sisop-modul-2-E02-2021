#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <wait.h>
#include <signal.h>

char* caesarChiper(const char* text, int shift){ 
    char *encrypt = (char*)calloc(sizeof(char),20);
    strcpy(encrypt, text);

    for(int i = 0; i < strlen(encrypt); i++){
        if(encrypt[i] >= 'A' && encrypt[i] <= 'Z'){
            encrypt[i] = (encrypt[i] - 'A' + shift) % 26 + 'A';
            
        }else if(encrypt[i] >= 'a' && encrypt[i] <= 'z'){
            encrypt[i] = (encrypt[i] - 'a' + shift) % 26 + 'a';
        }
    }
    return encrypt;
}

void makeProgramKiller(char *projectPath,int daemonSID){
    FILE *fileout;
    char fileKiller[100];
    strcpy(fileKiller,projectPath);
    strcat(fileKiller,"Killer.sh");
    fileout = fopen(fileKiller,"w");
    fputs("#!/bin/bash\n",fileout);
    fprintf(fileout,"pkill -15 -s %d\n",daemonSID);
    fprintf(fileout,"rm %s\n",fileKiller);
    fclose(fileout);

    pid_t childID = fork();
    if(childID < 0) exit(EXIT_FAILURE);
    else if(childID == 0){
        char *args[] = {"chmod", "+x", fileKiller, NULL};
        execv("/bin/chmod",args);
    }else{
        int status;
        while((wait(&status)) > 0);
    }
}

int main(int argc, char *argv[]){
    if(argc < 2 || !(strcmp(argv[1],"-x") == 0 || strcmp(argv[1],"-z") == 0))  {
        printf("argumen tidak valid\n");
        return -1;
    }
    pid_t child, childSID;

    child = fork();

    if(child < 0) exit(EXIT_FAILURE);
    else if(child > 0) exit(EXIT_SUCCESS);

    umask(0);
    childSID = setsid();

    if(childSID < 0) exit(EXIT_FAILURE);

    if((chdir("/")) < 0) exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    char projectPath[100];
    strcpy(projectPath,"/home/ryan/Documents/soal-shift-sisop-modul-2-E02-2021/soal3/");
    makeProgramKiller(projectPath, childSID);

    while(1){

        pid_t child1 = fork();

        if(child1 < 0) exit(EXIT_FAILURE);
        else if(child1 == 0){
            if(argc == 2 && strcmp(argv[1],"-x") == 0){
                int child1SID = setsid();
            }

            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            char folderName[20];
            strftime(folderName, 20, "%Y-%m-%d_%H:%M:%S", tm);

            char folderPath[100];
            strcpy(folderPath,projectPath);
            strcat(folderPath,folderName);

            pid_t child2 = fork();

            if(child2 < 0) exit(EXIT_FAILURE);
            else if(child2 == 0){
                char *args[] = {"mkdir", "-p", folderPath, NULL};
                execv("/bin/mkdir", args);
            }else{
                int status;
                while((wait(&status))>0);
                
                pid_t child3 = fork();

                if(child3 < 0) exit(EXIT_FAILURE);
                else if(child3 == 0){
                    for(int i = 0; i < 10; i++){
                        pid_t child4 = fork ();
                        if(child4 < 0) exit(EXIT_FAILURE);
                        else if(child4 == 0){
                            time_t t = time(NULL);
                            struct tm *tm = localtime(&t);
                            char fileName[20];
                            strftime(fileName, 20, "%Y-%m-%d_%H:%M:%S", tm);
                            char filePath[100];
                            strcpy(filePath,folderPath);
                            strcat(filePath,"/");
                            strcat(filePath,fileName);

                            char source[100];
                            sprintf(source,"https://picsum.photos/%d",(((int)t) % 1000) + 50);

                            char *args[] = {"wget", "-O", filePath, source, NULL };
                            execv("/usr/bin/wget", args);
                        }
                        sleep(5);
                    }
                    exit(EXIT_SUCCESS);
                }else{
                    int status2;
                    while((wait(&status2)) > 0);

                    FILE *fileout;
                    char fileStatus[100];
                    strcpy(fileStatus,folderPath);
                    strcat(fileStatus,"/");
                    strcat(fileStatus,"status.txt");
                    fileout = fopen(fileStatus,"w");
                    fputs(caesarChiper("Download Success",5),fileout);
                    fclose(fileout);

                    pid_t child5 = fork();
                    if(child5 < 0) exit(EXIT_FAILURE);
                    else if(child5 == 0){
                        chdir(projectPath);
                        char *args[] = {"zip", "-rm" , folderPath, folderName, NULL};
                        execv("/usr/bin/zip",args);    
                    }else{
                        int status3;
                        while((wait(&status3)) > 0);
                    }
                }
            }
            exit(EXIT_SUCCESS);
        }else{
            sleep(40);
        }
    }
    return 0;
}