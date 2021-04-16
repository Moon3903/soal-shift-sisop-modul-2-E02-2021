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
## Penyelesaian

# Soal 3
## Penjelasan
a) Membuat sebuah program C untuk membuat direktori setiap 40 detik dengan format nama [YYYY-mm-dd_HH:ii:ss]  </br>
b) Mengisi direktori yang sudah dibuat dengan 10 gambar berbentuk persegi berukuran (n%1000) + 50 pixel dimana n adalah detik Epoch Unix yang di download dari https://picsum.photos/ dengan format nama [YYYY-mm-dd_HH:ii:ss] </br>
c) Setelah selesai mengunduh 10 gambar, program akan membuat sebuah sebuah file ``status.txt`` dengan pesan "Download Success" yang telah dienkripsi menggunakan teknik Caesar Cipher dengan shift. Melakukan zip pada direktori dan menghapus direktori (hanya tersisa file zip) </br>
d) Membuat executable program Killer berupa program bash untuk menterminasi proses yang sedang berjalan. </br>
e) Program dapat dijalankan dengan dua mode. Mode pertama dengan argumen -z, ketika program Killer dijalankan maka seluruh proses akan dihentikan. Mode kedua dengan argumen -x, ketika program Killer dijalankan program utama akan dihentikan tetapi membiarkan proses di setiap direktori yang masih berjalan hingga selesai. </br>


## Penyelesaian

### Membuat Daemon Process
```
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
```
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
```
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
```
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


```
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
```
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
```
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



