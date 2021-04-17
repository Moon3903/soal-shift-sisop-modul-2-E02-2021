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

void download(char *source, char *projectPath, char *fileName){
    pid_t child = fork();
    if(child < 0) exit(EXIT_FAILURE);
    else if(child == 0){
        char filePath[150];
        strcpy(filePath,projectPath);
        strcat(filePath,fileName);
        char *args[] = {"wget","--no-check-certificate", source, "-O", filePath, NULL};
        execv("/usr/bin/wget",args);
    }else{
        int status;
        while((wait(&status)) > 0);
    }
}

void unzip(char *fileName, char *projectPath, char *folderName){
    pid_t child = fork();
    if(child < 0) exit(EXIT_FAILURE);
    else if(child == 0){
        char filePath[150], destPath[150];
        strcpy(filePath,projectPath);
        strcat(filePath,fileName);

        strcpy(destPath, projectPath);
        strcat(destPath, folderName);

        char *args[] = {"unzip","-j", filePath, "-d", destPath, NULL};
        execv("/usr/bin/unzip",args);
    }else{
        int status;
        while((wait(&status)) > 0);
    }
}

int main(){
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

    char projectPath[80];
    strcpy(projectPath, "/home/ryan/Documents/soal-shift-sisop-modul-2-E02-2021/soal1/");

    while(1){
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char month[5], date[5], hour[5], minute[5], second[5];
        strftime(month, 5, "%m", tm);
        strftime(date, 5, "%d", tm);
        strftime(hour, 5, "%H", tm);
        strftime(minute, 5, "%M", tm);
        strftime(second, 5, "%S", tm);

        if( strcmp(date,"09") == 0 && strcmp(month,"04") == 0 && strcmp(hour,"16") == 0 && strcmp(minute,"22") == 0 && strcmp(second,"00") == 0){
            pid_t child1 = fork();

            if(child1 < 0) exit(EXIT_FAILURE);
            else if(child1 == 0){
                char folderMusyik[100], folderFylm[100], folderPyoto[100];
                strcpy(folderMusyik,projectPath);
                strcat(folderMusyik,"Musyik/");

                strcpy(folderFylm,projectPath);
                strcat(folderFylm,"Fylm/");

                strcpy(folderPyoto,projectPath);
                strcat(folderPyoto,"Pyoto/");

                char *args[]={"mkdir","-p",folderMusyik,folderFylm,folderPyoto,NULL};
                execv("/bin/mkdir", args);
            }else{
                download("https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download",projectPath,"Foto_for_Stevany.zip");
                download("https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download",projectPath,"Musik_for_Stevany.zip");
                download("https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download",projectPath,"Film_for_Stevany.zip");
                unzip("Foto_for_Stevany.zip",projectPath,"Pyoto");
                unzip("Musik_for_Stevany.zip",projectPath,"Musyik");
                unzip("Film_for_Stevany.zip",projectPath,"Fylm");
            }
        }

        if( strcmp(date,"09") == 0 && strcmp(month,"04") == 0 && strcmp(hour,"22") == 0 && strcmp(minute,"22") == 0 && strcmp(second,"00") == 0){
            pid_t child2 = fork();
            if(child2 < 0) exit(EXIT_FAILURE);
            else if(child2 == 0){
                chdir(projectPath);
                char *args[] = {"zip", "-rm","Lopyu_Stevany.zip", "Fylm/", "Musyik/", "Pyoto/", NULL};
                execv("/usr/bin/zip",args);    
            }else{
                int status;
                while((wait(&status)) > 0);
            }
        }
    }
    return 0;
}
