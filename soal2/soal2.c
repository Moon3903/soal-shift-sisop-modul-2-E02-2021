#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>

#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void iniA();
void delFolder();
void sisanya();

int main() {
    iniA();
    delFolder();
    sisanya();

    printf("aman\n");
}

void teh(char nama[],char jenis[],char fname[]){
    char dest[200],source[200];
    pid_t child_id;
    int status;

    strcpy(source,"/home/moon/modul2/petshop/");
    strcat(source,fname);

    strcpy(dest,"/home/moon/modul2/petshop/");
    strcat(dest,jenis);

    child_id = fork();

    if (child_id == 0) {
        char *argv[] = {"mkdir", "-p", dest, NULL};
        execv("/bin/mkdir", argv);
    }while ((wait(&status)) > 0);

    strcat(dest,"/");
    strcat(dest,nama);
    strcat(dest,".jpg");
    // printf("\n%s %s\n",source,dest);
    child_id = fork();

    if (child_id == 0) {
        char *argv[] = {"cp", "-b", source,dest, NULL};
        execv("/bin/cp", argv);
    }while ((wait(&status)) > 0);
}

void hapus(char nama[]){
    pid_t child_id;
    int status;
    
    char path[200];
    
    strcpy(path,"/home/moon/modul2/petshop/");
    strcat(path,nama);
    
    child_id = fork();

    if (child_id == 0) {
        char *argv[] = {"rm", "-f", path, NULL};
        execv("/bin/rm", argv);
    }while ((wait(&status)) > 0);
}

void keterangan(char nama[],char umur[],char jenis[]){
    FILE * fp;

    char path[200];

    strcpy(path,"/home/moon/modul2/petshop/");
    strcat(path,jenis);
    strcat(path,"/keterangan.txt");

    /* open the file for writing*/
    fp = fopen (path,"a");
    fprintf(fp,"nama : %s\numur : %s tahun\n\n",nama,umur);

    /* close the file*/  
    fclose (fp);
}

void sisanya(){
    struct stat file_stats;
    DIR *dirp;
    struct dirent* dent;

    char jenis[100],nama[100],umur[100];
    int i,x;

    dirp=opendir("/home/moon/modul2/petshop");
    do {
        dent = readdir(dirp);
        if (dent)
        {   
            if((strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)){
                continue;
            }
            i = 0;
            while(1){

                x = 0;
                while(dent->d_name[i]!=';'){
                    jenis[x] = dent->d_name[i];
                    x++;
                    i++;
                }
                jenis[x]='\0';

                i++;
                x = 0;
                while(dent->d_name[i]!=';'){
                    nama[x] = dent->d_name[i];
                    x++;
                    i++;
                }
                nama[x]='\0';

                i++;
                x=0;
                while(dent->d_name[i+1]!='j' && dent->d_name[i]!='_'){
                    umur[x] = dent->d_name[i];
                    x++;
                    i++;
                }
                umur[x] = '\0';

                teh(nama,jenis,dent->d_name);
                keterangan(nama,umur,jenis);

                if(dent->d_name[i]!='_'){
                    hapus(dent->d_name);
                    break;
                }
                i++;
            }
        }
    } while (dent);
    closedir(dirp);
}

void delFolder(){
    struct stat file_stats;
    DIR *dirp;
    struct dirent* dent;
    pid_t child_id;
    int status;

    char path[200] = "/home/moon/modul2/petshop";
    dirp=opendir(path);
    do {
        dent = readdir(dirp);
        if (dent)
        {
            char tmp[100],hapus[200];
            strcpy(tmp,dent->d_name);
            if(strlen(tmp)<3){
                continue;
            }
            int len = strlen(tmp);
            if(tmp[len-3]=='j' && tmp[len-2]=='p' && tmp[len-1]=='g'){
                continue;
            }
            else{
                strcpy(hapus,path);
                strcat(hapus,"/");
                strcat(hapus,tmp);
                printf("DELETED : %s\n",hapus);
                child_id = fork();

                if (child_id == 0) {
                    char *argv[] = {"rm", "-rf", hapus, NULL};
                    execv("/bin/rm", argv);
                } while ((wait(&status)) > 0);
            }
        }
    } while (dent);
    closedir(dirp);
}

void iniA(){
    pid_t child_id;
    int status;

    char zip_loc[100] = "pets.zip";
    char zip_dest[100] = "/home/moon/modul2/petshop";


    child_id = fork();

    if (child_id == 0) {
        char *argv[] = {"mkdir", "-p", zip_dest, NULL};
        execv("/bin/mkdir", argv);
    } while ((wait(&status)) > 0);
    printf("PATH CREATED\n");

    child_id = fork();

    if (child_id == 0) {
        char *argv[] = {"unzip", "-d", zip_dest,"-oqq", zip_loc, NULL};
        execv("/usr/bin/unzip", argv);
    } while ((wait(&status)) > 0);
    printf("Decompressed\n");
}