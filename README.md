# soal-shift-sisop-modul-2-E02-2021

## Anggota Kelompok E02
05111940000001 - Christoper Baptista

05111940000101 - Zulfiqar Fauzul Akbar

05111940000152 - Ryan Fernaldy

## Daftar Isi
[Soal 1](https://github.com/Moon3903/soal-shift-sisop-modul-2-E02-2021#soal-1) </br>
[Soal 2](https://github.com/Moon3903/soal-shift-sisop-modul-2-E02-2021#soal-2) </br>
[Soal 3](https://github.com/Moon3903/soal-shift-sisop-modul-2-E02-2021#soal-3) </br>

# Soal 1
## Penjelasan
## Penyelesaian


# Soal 2
## Penjelasan
a) Mengekstrak zip pets.zip ke `/home/[user]/modul2/petshop` dan menghapus folder yang tidak di butuhkan </br>
b) Foto peliharaan dikategorikan sesuai jenis kedalam folder yang sama misal jenis cat dimasukkan kedalam folder `petshop/cat` </br>
c) Memindahkan foto kedalam folder jenis yang sesuai dan file direname sesuai nama peliharaan misal `petshop/cat/neko.jpg` </br>
d) Foto dapat mengandung lebih dari satu peliharaan maka harus di masukkan kedalam kategori yang sesuai misal `dog;baro;1_cat;joni;2.jpg` menjadi `/petshop/cat/joni.jpg` dan `/petshop/dog/baro.jpg` </br>
e) Disetiap folder harus ada file `keterangan.txt` yang berisi nama dan umur peliharaan dengan format
```
nama : joni
umur  : 3 tahun

nama : miko
umur  : 2 tahun

```
</br>

## Penyelesaian
Pada Main akan memanggil 3 fungsi yaitu fungsi `iniA()` untuk mengekstrak lalu `delFolder()` untuk menghapus folder yang tidak diperlukan dan `sisanya()` untuk menyelesaikan point b-e
```c
int main() {
    iniA();
    delFolder();
    sisanya();

    printf("aman\n");
}
```
### Soal 2a
Pertama membuat directory `/home/[user]/modul2/petshop` terlebih dahulu dengan mempassing parameter ke `bin/mkdir`</br>
setelah itu di diekstrak dengan unzip pada `/usr/bin/unzip` dengan mempassing beberapa parameter yaitu perintah unzip `unzip`, lalu destination folder dengan `-d` dan `zip_dest`, ada parameter `-oqq` o disini untuk overwrite tanpa persetujuan dan qq adalah very silent mode jadi tidak ada output pada terminal, dan parameter terakhir adalah `zip_loc` atau lokasi file zip nya
```c
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
```
setelah diekstrak selanjutnya adalah menghapus file dan folder yang tidak perlu, karena semua filenya adalah .jpg maka selain format tersebut akan dihapus, penghapusan menggunakan library `<dirent.h>` lalu menggunakan fungsi `opendir()` dan `readdir` untuk mengtransverse folder tersebut jika ditemukan yang bukan .jpg maka di hapus dengan `/bin/rm` dengan param `rm` `-rf` dan `path` ke file/folder nya
```c
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
```
### Soal 2b - 2e
```c
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

                if(dent->d_name[i+1]=='j'){
                    hapus(dent->d_name);
                    break;
                }
                i++;
            }
        }
    } while (dent);
    closedir(dirp);
}
```
Pertama adalah mendapatkan nama jenis dan umur dari hewan tersebut dengan menggunakan `<dirent.h>`, `opendir()`, dan `readdir` yang setelah itu di looping. Setelah itu memanggil fungsi `teh()` untuk mengcopy file kedalam jenis yang sesuai dengan format nama yang benar dengan menggunakan `/bin/mkdir` untuk membuat directory dan `/bin/cp` untuk mengcopy filenya
```c
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
```
setelah itu di cek apakah ada hewan lain difoto tersebut atau tidak dengan 
```c
if(dent->d_name[i+1]=='j'){
    hapus(dent->d_name);
    break;
}
```
dimana jika if tersebut bernilai true maka tidak ada hewan lain lalu file tersebut dihapus, namun jika masih ada hewan lain kembali mengambil nama, jenis, dan umur kembali sampai tidak ada hewan yang belum dimasukkan dalam foto tersebut.</br>
untuk proses penghapusan sendiri menggunakan `/bin/rm` dengan mempassing parameter `rm` `-f` dan `path` filenya
```c
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
```
### Hasil
Run:
![Screenshot from 2021-04-17 12-15-46](https://user-images.githubusercontent.com/62832487/115102711-c9786500-9f76-11eb-98fe-360682831e27.png)
Folder `/home/moon/modul2/petshop` :
![Screenshot from 2021-04-17 12-15-56](https://user-images.githubusercontent.com/62832487/115102722-e0b75280-9f76-11eb-8d42-caf922ff7a17.png)
Isi folder `petshop/cat` :
![Screenshot from 2021-04-17 12-16-02](https://user-images.githubusercontent.com/62832487/115102732-faf13080-9f76-11eb-88f7-f68baf0e09b8.png)
Isi file `petshop/cat/keterangan.txt` :
![Screenshot from 2021-04-17 12-16-14](https://user-images.githubusercontent.com/62832487/115102739-0b091000-9f77-11eb-9829-454728a7afb4.png)
</br>
# Soal 3
## Penjelasan
a) Membuat sebuah program C untuk membuat direktori setiap 40 detik dengan format nama [YYYY-mm-dd_HH:ii:ss]  </br>
b) Mengisi direktori yang sudah dibuat dengan 10 gambar berbentuk persegi berukuran (n%1000) + 50 pixel dimana n adalah detik Epoch Unix yang di download dari https://picsum.photos/ dengan format nama [YYYY-mm-dd_HH:ii:ss] </br>
c) Setelah selesai mengunduh 10 gambar, program akan membuat sebuah sebuah file ``status.txt`` dengan pesan "Download Success" yang telah dienkripsi menggunakan teknik Caesar Cipher dengan shift. Melakukan zip pada direktori dan menghapus direktori (hanya tersisa file zip) </br>
d) Membuat executable program Killer berupa program bash untuk menterminasi proses yang sedang berjalan. </br>
e) Program dapat dijalankan dengan dua mode. Mode pertama dengan argumen -z, ketika program Killer dijalankan maka seluruh proses akan dihentikan. Mode kedua dengan argumen -x, ketika program Killer dijalankan program utama akan dihentikan tetapi membiarkan proses di setiap direktori yang masih berjalan hingga selesai. </br>


## Penyelesaian

### Membuat Daemon Process
```c
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
```
Untuk menyelesaikan soal ini diperlukan proses yang dapat berjalan di background sehingga harus membuat daemon process terlebih dahulu.

### Soal 3a)
```c
while(1){

	pid_t child1 = fork();

        if(child1 < 0) exit(EXIT_FAILURE);
        else if(child1 == 0){
		...
		
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
		}
		...
	}else{
            int status4;
            while((wait(&status4)) > 0);
            sleep(40);
        }
}

```
Untuk membuat folder sesuai dengan format, dapat menggunakan fork, execv, dan fungsi ``strftime``. Mengunakan ``sleep(40)`` agar dapat berjalan secara terus menerus setiap 40 detik.


### Soal 3b)
```c
else{
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
}
```
Sebelum mengunduh gambar, harus melakukan ``wait`` untuk memastikan folder sudah terbuat. Untuk menyimpan gambar sesuai dengan format menggunakan fungsi ``strftime``. Variabel ``filepath`` merupakan string yang menyimpan path lengkap dari file baru yang akan di download. Untuk mendapatkan detik Epoch UNIX menggunakan ``time`` setelah itu di-typecast ke integer. Agar gambar yang diunduh bisa berbentuk persegi dengan ukuran yang diminta harus menambahkan ukuran pada link ``https://picsum.photos/<ukuran>``.Untuk mengunduh gambar menggunakan ``wget``, argumen ``-O`` untuk mengatur lokasi dan nama file yang diunduh. 


### Soal 3c)
```c
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
```
Fungsi ``caesarChiper`` akan mereturn hasil enkripsi sesuai dengan kata dan shift yang diberikan.


```c
else{
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
```
Sebelum membuat ``status.txt`` harus melakukan ``wait`` untuk memastikan proses download sudah selesai. Menggunakan ``FILE`` pada C untuk membuat file ``status.txt``. ``fputs`` untuk menuliskan hasil enkripsi "Download Success" yang dapat diperoleh dengan melakukan pemanggilan fungsi ``caesarChiper("Download Success",5)``. Fungsi ``chdir`` untuk memindahkan lokasi ke folder project. Untuk melakukan zip menggunakan fork dan execv dengan perintah ``zip``. Argumen ``-rm`` untuk melakukan zip seluruh isi folder sekaligus menghapus folder yang di-zip.


### Soal 3d)
```c
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
```
Unutk men-generate program Killer, menggunakan ``FILE`` pada C yang akan menuliskan script yang dibutuhkan. Untuk melakukan terminasi proses yang ada dapat menggunakan signal ``15``, argumen ``-s`` untuk menterimnasi proses berdasarkan Session ID (SID). Agar program Killer ini bisa menjadi executable maka harus mengubah file permission dari file tersebut dengan menggunakan fork dan execv dengan perintah ``chmod``, argumen ``+x`` agar file bisa di-execute.


### Soal 3e)
- Mode pertama ``-z`` sudah terselesaikan pada soal 3d).
- Mode kedua ``-x`` Karena pada soal 3d) menggunakan ``pkill -15 -s`` untuk melakukan terminasi berdasarkan Session ID (SID), maka agar proses di direktori bisa tetap berjalan harus melakukan fork dimana child harus memiliki SID yang berbeda dengan SID dari daemon process. Dengan demikian, jika program Killer dijalankan child tersebut akan menjadi tumpuan menggantikan daemon process. Berikut ini adalah potongan kodenya
```c
while(1){

	pid_t child1 = fork();

	if(child1 < 0) exit(EXIT_FAILURE);
	else if(child1 == 0){
	    if(argc == 2 && strcmp(argv[1],"-x") == 0){
		int child1SID = setsid();
	    }
	    ...
	}
	...
```

### Hasil
- Mode pertama
![Mode1](https://user-images.githubusercontent.com/68326540/115038022-52e95200-9ef9-11eb-92ff-f0bf8c7b8193.png)
- Mode kedua
![Mode2](https://user-images.githubusercontent.com/68326540/115038220-83c98700-9ef9-11eb-929b-69d17d111886.png)



