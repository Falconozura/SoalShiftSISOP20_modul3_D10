# SoalShiftSISOP20_modul3_D10

Clever Dicki Marpaung (05111840000116) 

Nodas Uziel Putra Serpara (05111840007007)

## Soal 1 (Belum Selesai)

## Soal 2
Pada soal nomor 2 ini, diminta untuk membuat game text console. Pada soal ini, terdapat
dua sisi:
1. Server Side
2. Client Side
Pada soal ini, kami belum dapat menyelesaikan soal sepenuhnya, dimana pada bagian 
`find match` kami belum dapat menyelesaikannya. Dalam pengerjaan soal ini, kami menggunakan
socket dan threading.

### Server Side
Source Code:
```c
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define PORT 8080

struct player {
  int jlh;
  struct sockaddr_in playerAddress;
  int addrlen;
};

struct player Player[1024];
int playerCount = 0;
pthread_t thread[1024];

void *process(void *PlayerUp) {
  struct player* playerUp = (struct player*) PlayerUp;
  int new_socket = playerUp->jlh;
  
  while(1) {
    char account[1024], checking[1024], buffer[1024], cmd[1024];
    int read = recv(new_socket, account, 1024, 0);
    account[read] = '\0';
    char cek = 0;
    memset(buffer, 0, sizeof buffer);
    if (strcmp(account, "Login") == 0 || strcmp(account, "login") == 0) {
      FILE *fp = fopen("akun.txt", "r");
      if(!fp) {
        FILE *file2 = fopen("akun.txt", "w");
        fclose(file2);
        FILE *file = fopen("akun.txt", "r");
      }
      read = recv(new_socket, account, 1024, 0);
      while (fscanf(fp, "%s", checking) != EOF) {
        if(!strcmp(checking, account)) {
          cek = 1;
          break;
        }
      }
      if(cek) {
        printf("Auth success\n");
        strcpy(cmd, "success");
        send(new_socket, cmd, 1024, 0);
      } else {
          printf("Auth failed\n");
          strcpy(cmd, "failed");
          send(new_socket, cmd, 1024, 0);
      }
      fclose(fp);
    } 
    else if (strcmp(account, "Register") == 0 || strcmp(account, "register") == 0) {
      FILE *fp = fopen("akun.txt", "a");
      read = recv(new_socket, account, 1024, 0);
      account[read] = '\0';
      fprintf(fp, "%s", account);
      fprintf(fp, "\n");
      strcpy(cmd,"success");
      fclose(fp);
      FILE *fp1 = fopen("akun.txt", "r");
      while (fscanf(fp1, "%s", checking) != EOF) {
        printf("%s\n", checking);
      }
      fclose(fp1);
      send(new_socket, cmd, 1024, 0);
    }
  }
}

int main(int argc, char const *argv[]){
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1, stock=1;

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
    perror("Could not create socket\n");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );
  
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
    perror("bind failed\n");
    exit(EXIT_FAILURE);
  }
    
  if (listen(server_fd, 3) < 0) {
    perror("listen failed\n");
    exit(EXIT_FAILURE);
  }

  printf("Waiting for making connection...\n"); 
  while (1)
  {
    Player[playerCount].jlh = accept(server_fd, (struct sockaddr *)&Player[playerCount].playerAddress, &Player[playerCount].addrlen);
    pthread_create(&thread[playerCount], NULL, process, &Player[playerCount]);
    playerCount++;
  }
  for(int i = 0; i < playerCount; i++)
    pthread_join(thread[i], NULL);
  return 0;
}

```

Dikarenakan fitur `find match` belum dapat kami selesaikan, maka di bagian server
ini terdapat 2 proses yang dapat dijalankan. Yaitu `Login` dan `Register`. Server
akan menjalankan perintah sesuai input yang diberikan oleh user melalui Client Side.

```c
if (strcmp(account, "Login") == 0 || strcmp(account, "login") == 0) {
      FILE *fp = fopen("akun.txt", "r");
      if(!fp) {
        FILE *file2 = fopen("akun.txt", "w");
        fclose(file2);
        FILE *file = fopen("akun.txt", "r");
      }
      read = recv(new_socket, account, 1024, 0);
      while (fscanf(fp, "%s", checking) != EOF) {
        if(!strcmp(checking, account)) {
          cek = 1;
          break;
        }
      }
      if(cek) {
        printf("Auth success\n");
        strcpy(cmd, "success");
        send(new_socket, cmd, 1024, 0);
      } else {
          printf("Auth failed\n");
          strcpy(cmd, "failed");
          send(new_socket, cmd, 1024, 0);
      }
      fclose(fp);
    } 
```

Bagian diatas akan dijalankan ketika Client Side memberikan inputan `Login`.
Akan dilakukan operasi file untuk nantinya digunakan dalam pengecekan akun
yang sudah terdaftar pada game ini, dimana semua data akun tersimpan di file
`akun.txt`. Client side akan mengirimkan `Username` dan `Password`, kemudian
server akan mengecek apakah inputan `Username` dan `Password` sesuai dengan
akun yang teregistrasi di file `akun.txt`. Pengecekan dilakukan secara iteratif
sampai ditemukannya kecocokan dengan inputan ataupun dicek sampai akhir file jika
tidak ditemukan kecocokan. Jika ditemukan kecocokan, `cek` akan bernilai `1` dan
pengecekan berhenti dilakukan. Kemudian jika `cek` bernilai `1`, maka login
berhasil dan pada Server Side akan mengeluarkan output `Auth success`, serta
mengirimkan string ke Client Side untuk mendandakan bahwa akun tersebut benar.
Jika `cek` tidak bernilai `1`, maka Server Side akan mengeluarkan output 
`Auth failed` dan mengirimkan string juga ke Client Side untuk menandakan bahwa
akun tidak benar.

```c
else if (strcmp(account, "Register") == 0 || strcmp(account, "register") == 0) {
      FILE *fp = fopen("akun.txt", "a");
      read = recv(new_socket, account, 1024, 0);
      account[read] = '\0';
      fprintf(fp, "%s", account);
      fprintf(fp, "\n");
      strcpy(cmd,"success");
      fclose(fp);
      FILE *fp1 = fopen("akun.txt", "r");
      while (fscanf(fp1, "%s", checking) != EOF) {
        printf("%s\n", checking);
      }
      fclose(fp1);
      send(new_socket, cmd, 1024, 0);
    }
```

Bagian diatas akan dijalankan ketika Client Side mengirimkan inputan
`Register`. Program akan melakukan operasi file untuk nantinya write file
untuk akun yang akan diregistrasi. Setelah akun disimpan di file `akun.txt`,
maka selanjutnya Server Side akan mengeluarkan output berupa semua akun
yang tersimpan di `akun.txt`. Pengecekan `EOF` berarti dilakukan sampai
akhir dari file tersebut. Kemudian Server Side akan memberikan tanda ke
Client Side bahwa proses registrasi telah berhasil.

### Client Side
Source Code:
```c
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#include <wait.h>
#define PORT 8080

int main(int argc, char const *argv[]){
    
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024], cmd[1024], username[1024], password[1024], account[1024];
    int screen = 1;
    int status;
    pid_t child_id;

    while(1) {
    //create socket
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            return -1;
        } 
        memset(&serv_addr, '0', sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
    
        // Convert IPv4 and IPv6 addresses from text to binary form 
        if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }
    
        //connect to server
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnection Failed \n");
            return -1;
        }

        if(screen == 1) {
            memset(buffer, 0, sizeof buffer);
            child_id = fork();
            if(child_id == 0) {
                char *argv[] = {"clear", NULL};
                execv("/usr/bin/clear", argv);
            } else
                while ((wait(&status)) > 0);
            printf("1. Login\n2. Register\nChoices: ");
            scanf("%s", cmd);
            send(sock, cmd, 1024, 0);
            if(strcmp(cmd, "Login") == 0 || strcmp(cmd, "login") == 0) {
                printf("Username: ");
                scanf("%s", username);
                printf("Password: ");
                scanf("%s", password);
                sprintf(account, "%s|%s", username, password);
                send(sock, account, 1024, 0);
                recv(sock, buffer, 1025, 0);
                child_id = fork();
                if(child_id == 0) {
                    char *argv[] = {"clear", NULL};
                    execv("/usr/bin/clear", argv);
                } else
                    while ((wait(&status)) > 0);
                if(strcmp(buffer, "success") == 0) {
                    screen = 2;
                    printf("login success\n");
                } else {
                    screen = 1;
                    printf("login failed\n");
                }
                continue;
            }
            else if(strcmp(cmd, "Register") == 0 || strcmp(cmd, "register") == 0) {
                printf("Username: ");
                scanf("%s", username);
                printf("Password: ");
                scanf("%s", password);
                sprintf(account, "%s|%s", username, password);
                send(sock, account, 1024, 0);
                recv(sock, buffer, 1024, 0);
                child_id = fork();
                if(strcmp(buffer, "success") == 0) {
                    screen = 1;
                    printf("register success\n");
                } else {
                    screen = 1;
                    printf("register failed\n");
                }
                continue;
            }

        }
        if (screen == 2) {
            printf("1. Find Match\n2. Logout\nChoices: ");
            scanf("%s", cmd);
            if (strcmp(cmd, "Logout") == 0 || strcmp(cmd, "logout") == 0) {
                screen = 1;
                continue;
            }
        }
        
    }
    return 0;
}
```

Pada Client Side, terdapat dua screen, yaitu screen pertama akan menampilkan opsi
`Login` dan opsi `Registrasi`. Kemudian, setelah berhasil login user akan masuk
ke screen kedua yang akan menampilkan opsi `Find Match` dan opsi `Logout`. Jika
user memilih opsi `Logout`, maka akan kembali ke screen pertama.

```c
printf("1. Login\n2. Register\nChoices: ");
scanf("%s", cmd);
send(sock, cmd, 1024, 0);
if(strcmp(cmd, "Login") == 0 || strcmp(cmd, "login") == 0) {
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);
    sprintf(account, "%s|%s", username, password);
    send(sock, account, 1024, 0);
    recv(sock, buffer, 1025, 0);
    child_id = fork();
    if(child_id == 0) {
         char *argv[] = {"clear", NULL};
         execv("/usr/bin/clear", argv);
    } else
         while ((wait(&status)) > 0);
    if(strcmp(buffer, "success") == 0) {
         screen = 2;
         printf("login success\n");
    } else {
         screen = 1;
         printf("login failed\n");
    }
    continue;
}
```

Jika user menginputkan `Login`, maka Client Side akan menampilkan `Username`
dan `Password` dimana user akan menginputkan data akun mereka yang nantinya
data tersebut akan dikirim ke Server Side untuk di cek kebenarannya. Data
tersebut disimpan di dalam variabel `account`. Screen akan di-clear dengan
command `clear`. Client Side menerima pesan dari Server Side. Jika pesan dari
server berisi `success`, maka proses login berhasil dan akan lanjut ke screen
kedua serta akan mengeluarkan output `login success`. Jika tidak, akan kembali
ke screen awal dan Client Side mengeluarkan ouput `login failed`.

```c
else if(strcmp(cmd, "Register") == 0 || strcmp(cmd, "register") == 0) {
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);
    sprintf(account, "%s|%s", username, password);
    send(sock, account, 1024, 0);
    recv(sock, buffer, 1024, 0);
    child_id = fork();
    if(strcmp(buffer, "success") == 0) {
        screen = 1;
        printf("register success\n");
    } else {
        screen = 1;
        printf("register failed\n");
    }
    continue;
}
```

Jika user menginputkan `Register`, sama seperti opsi `Login`, Client side akan
menampilkan `Username` dan `Password` yang akan diinputkan user untuk registrasi
akun baru. Data akan disimpan di variabel `account`. Client Side akan menerima
pesan dari Server Side. Apabila pesan yang dikirim berisi `success` maka proses
registrasi berhasil dan Client Side akan mengeluarkan output `register success`
dan kembali ke screen awal. Jika tidak berhasil, Client Side akan mengeluarkan
output `register failed` dan kembali ke screen awal.

```c
if (screen == 2) {
    printf("1. Find Match\n2. Logout\nChoices: ");
    scanf("%s", cmd);
    if (strcmp(cmd, "Logout") == 0 || strcmp(cmd, "logout") == 0) {
        screen = 1;
        continue;
    }
}
```
Setelah berhasil login, maka user akan masuk ke screen kedua. Nantinya di screen
kedua akan ditampilkan opsi `Find Match` dan opsi `Logout`. Ketika user menginputkan
`Logout`, maka screen akan kembali ke screen pertama.


## Soal 3
![rsz_1screenshot_from_2020-04-10_21-22-26](https://user-images.githubusercontent.com/61287359/78998291-7e14c500-7b72-11ea-8028-acd88b873be8.png)

Pada soal no 3, untuk mengecek apakah dia ``-f, *, atau -d`` terdapat pada bagian main dalam source kategori.c
```c
int main(int argc, char* argv[]) 
{ 
  getcwd(cwd, sizeof(cwd));
  if(strcmp(argv[1],"-f")==0)//command -f
  {
    pthread_t tid[1000]; //max 1000 thread
    int index = 0;
    for (int i = 2; i < argc; i++)
    {
      pthread_create(&tid[index], NULL, pindahf, argv[i]);
      sleep(0.5);
      index++; //
    }
    for (int i = 0; i < index; i++) {
        pthread_join(tid[i], NULL);
    }
  }
  else if(strcmp(argv[1],"*")==0)
  {
    char asal[] = "/home/nodz/Downloads/Praktikum3/soal3/soal3";
    
    sortHere(asal);
  }
  else if(strcmp(argv[1],"-d")==0){
      char asal[1000];
      strcpy(asal, argv[2]);
      sortHere(asal);
  }
  else
  {
      printf("salah argumen\n");
      return 0;
  }
  
	return 0; 
} 
```
pada saat input ``-f`` maka program tersebut akan membuat thread yang nantinya melakukan fungsi ``pindahf``(dimana fungsi tersebut mengecek dari mana asal file tersebut), lalu masuk ke fungsi ``pindahFile``(fungisi ini akan memilah mana yang tipe dan mana yang file, membuat semua huruf kapitala tipe file menjadi huruf kecil, mengecek apakah argumen yang kita berikan itu salah atau file yang kita argumentasikan itu sebenarnya file atau folder, mengecek apakah dia ``Unknown`` atau tidak, lalu membuat folder yang memisahkan dia dengan tipenya sendiri, setelah itu dipindahkan ke folder tersebut dipindahkan) lalu melakukan join di akhir ``if`` tersebut.

pada saat input ``*`` maka program akan menjalankan fungsi ``sortHere`` yang berguna untuk memilah apa saja tipe data yang berada pada folder tersebut, dan juga mengecek apakah itu folder atau bukan, setelah fungsi ``while ((entry = readdir(dirp)) != NULL)`` selesai, akan dilakukan thread seperti saat dimana argumen yang dimasukan adalah ``-f``(menjalkan fungsi ``pidahf`` dan ``pindahFile`` lalu melakukan join)

pada saat input ``-d`` maka program akan melakukan fungsi ``sortHere`` pada directory yang kita input


## SOAL4
Norland adalah seorang penjelajah terkenal. Pada suatu malam Norland menyusuri
jalan setapak menuju ke sebuah gua dan mendapati tiga pilar yang pada setiap
pilarnya ada sebuah batu berkilau yang tertancap. Batu itu berkilau di kegelapan dan
setiap batunya memiliki warna yang berbeda.
Norland mendapati ada sebuah teka-teki yang tertulis di setiap pilar. Untuk dapat
mengambil batu mulia di suatu pilar, Ia harus memecahkan teka-teki yang ada di
pilar tersebut. Norland menghampiri setiap pilar secara bergantian.
- Batu mulia pertama. Emerald. Batu mulia yang berwarna hijau mengkilat. Pada
batu itu Ia menemukan sebuah kalimat petunjuk. Ada sebuah teka-teki yang berisi:
  1. Buatlah program C dengan nama "4a.c", yang berisi program untuk
     melakukan perkalian matriks. Ukuran matriks pertama adalah 4x2, dan
     matriks kedua 2x5. Isi dari matriks didefinisikan di dalam kodingan. Matriks
     nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).
  2. Tampilkan matriks hasil perkalian tadi ke layar.
- Batu kedua adalah Amethyst. Batu mulia berwarna ungu mengkilat. Teka-tekinya
adalah:
  1. Buatlah program C kedua dengan nama "4b.c". Program ini akan
     mengambil variabel hasil perkalian matriks dari program "4a.c" (program
     sebelumnya), dan tampilkan hasil matriks tersebut ke layar.
     
     (Catatan!: gunakan shared memory)
  2. Setelah ditampilkan, berikutnya untuk setiap angka dari matriks
     tersebut, carilah nilai penjumlahan dari n sampai 1, dan tampilkan 
     hasilnya ke layar dengan format seperti matriks.
     Contoh: misal matriksnya:
     5     20     10     30     50
     10   56      ...      ...
     ...     ...
     ...     ... dst
     maka penghitungannya, untuk angka 5, adalah 5 + 4 + 3 + 2 + 1, untuk angka 20, 
     adalah 20 + 19 + 18 + 17 + ... + 1, angka 10, adalah 10 + 9 + 8 + ... + 1.

     (Catatan! : Harus menggunakan Thread dalam penghitungan penjumlahan)
- Batu ketiga adalah Onyx. Batu mulia berwarna hitam mengkilat. Pecahkan
teka-teki berikut!

   1. Buatlah program C ketiga dengan nama "4c.c". Program ini tidak
      memiliki hubungan terhadap program yang lalu.
   2. Pada program ini, Norland diminta mengetahui jumlah file dan
      folder di direktori saat ini dengan command "ls | wc -l". Karena sudah belajar
      IPC, Norland mengerjakannya dengan semangat.

      (Catatan! : Harus menggunakan IPC Pipes)

Begitu batu terakhir berhasil didapatkan. Gemuruh yang semakin lama semakin
besar terdengar. Seluruh tempat berguncang dahsyat, tanah mulai merekah. Sebuah
batu yang di atasnya terdapat kotak kayu muncul ke atas dengan sendirinya.

Sementara batu tadi kembali ke posisinya. Tanah kembali menutup, seolah tidak
pernah ada lubang merekah di atasnya satu detik lalu.
Norland segera memasukkan tiga buah batu mulia Emerald, Amethys, Onyx pada
Peti Kayu. Maka terbukalah Peti Kayu tersebut. Di dalamnya terdapat sebuah harta
karun rahasia. Sampai saat ini banyak orang memburu harta karun tersebut.
Sebelum menghilang, dia menyisakan semua petunjuk tentang harta karun tersebut
melalui tulisan dalam buku catatannya yang tersebar di penjuru dunia. "One Piece
does exist".

### SOLUSI
#### 4(a)
Berikut adalah program `4a.c`.
```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define LENGTH 2
#define WIDTH 4
#define WIDTH1 5
#define MAX_THREAD 4

int matrixA[WIDTH][LENGTH];
int matrixB[LENGTH][WIDTH1];
int (*matrixC)[WIDTH1] = 0;

void* multiplicationMatrix(void* arg){
    int i, j, k;
    for (i = 0 ; i < WIDTH; i++) {
        for (j = 0; j < WIDTH1; j++) {  
            for (k = 0; k < LENGTH; k++) {
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
}
int main(){
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int[WIDTH][WIDTH1]), IPC_CREAT | 0666);
    matrixC = shmat(shmid, NULL, 0);
    for (int i = 0; i < WIDTH; i++) { 
        for (int j = 0; j < LENGTH; j++) { 
            matrixA[i][j] = (j+1);
        } 
    }

    for (int i = 0; i < LENGTH; i++) { 
        for (int j = 0; j < WIDTH1; j++) { 
            matrixB[i][j] = (j+1);
        } 
    } 

    printf("Matrix A:\n");
    for (int i = 0; i < WIDTH; i++) { 
        for (int j = 0; j < LENGTH; j++)  
            printf("%d ",matrixA[i][j]);
        printf("\n");
    } 
  
    printf("Matrix B:\n");
    for (int i = 0; i < LENGTH; i++) { 
        for (int j = 0; j < WIDTH1; j++)  
            printf("%d ",matrixB[i][j]);
        printf("\n");
    }

    pthread_t threads;

    for (int i = 0; i < WIDTH; i++) {
        for(int j = 0;j < WIDTH1; j++) {
            matrixC[i][j] = 0;
        }
        pthread_create(&threads,NULL, multiplicationMatrix, NULL);
        pthread_join(threads, NULL); 
    }    

    printf("Hasil Perkalian:\n");
    for (int i = 0; i < WIDTH; i++) { 
        for (int j = 0; j < WIDTH1; j++)  
            printf("%d ",matrixC[i][j]);
        printf("\n");
    } 
    return 0;
}
```

Pada bagian (a), diminta untuk membuat sebuah program yang dapat melakukan
proses perkalian dua buah matriks, dimana matriks pertama berordo 4x2 dan
matriks kedua berordo 2x5. Nantinya, hasil dari perkalian matriks ini akan
digunakan juga pada program `4b.c`.

```c
key_t key = 1234;
int shmid = shmget(key, sizeof(int[WIDTH][WIDTH1]), IPC_CREAT | 0666);
matrixC = shmat(shmid, NULL, 0);
```

Pada bagian ini, dibuat uniqe key yaitu `1234`, dimana nantinya akan digunakan
sebagai key shared memory. Nantinya, hasil perkalian matriks akan disimpan pada
`matrixC` dan akan di share dengan program `4b.c`.

```c
for (int i = 0; i < WIDTH; i++) { 
  for (int j = 0; j < LENGTH; j++) { 
    matrixA[i][j] = (j+1);
  } 
}

for (int i = 0; i < LENGTH; i++) { 
  for (int j = 0; j < WIDTH1; j++) { 
    matrixB[i][j] = (j+1);
  } 
}
```

Bagian ini digunakan untuk mengisi nilai dari matriks yang dibuat. `matrixA` dan
`matrixB` akan diisi dengan nilai `j+1`.

```c
printf("Matrix A:\n");
for (int i = 0; i < WIDTH; i++) { 
  for (int j = 0; j < LENGTH; j++)  
    printf("%d ",matrixA[i][j]);
  printf("\n");
} 
  
printf("Matrix B:\n");
for (int i = 0; i < LENGTH; i++) { 
  for (int j = 0; j < WIDTH1; j++)  
    printf("%d ",matrixB[i][j]);
  printf("\n");
}
```

Bagian ini untuk menampilkan nilai dari `matrixA` dan `matrixB` yang dimana
pada bagian sebelumnya sudah diisi dengan nilai `j+1`.

```c
pthread_t threads;

for (int i = 0; i < WIDTH; i++) {
  for(int j = 0;j < WIDTH1; j++) {
    matrixC[i][j] = 0;
  }
  pthread_create(&threads,NULL, multiplicationMatrix, NULL);
  pthread_join(threads, NULL); 
} 
```

Pada bagian ini, dibuat thread untuk mengoperasikan perkalian dari matriks
yang sudah dibuat tadi. `matrixC` yang digunakan sebagai hasil dari perkalian
`matrixA` dan `matrixB` dideklarasikan terlebih dahulu dengan nilai nol.

```c
void* multiplicationMatrix(void* arg){
    int i, j, k;
    for (i = 0 ; i < WIDTH; i++) {
        for (j = 0; j < WIDTH1; j++) {  
            for (k = 0; k < LENGTH; k++) {
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
}
```

Thread dengan nama `multiplicationMatrix` ini akan melakukan proses perkalian
matriks, yang dimana hasil dari perkalian tersebut yaitu `matrixC` dengan ordo 4x5.

```c
printf("Hasil Perkalian:\n");
  for (int i = 0; i < WIDTH; i++) { 
    for (int j = 0; j < WIDTH1; j++)  
      printf("%d ",matrixC[i][j]);
    printf("\n");
} 
```

Hasil dari perkalian matriks akan ditampilkan melalui proses iterasi tersebut.

#### 4(b)
Berikut adalah program `4b.c`.
```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define LENGTH 2
#define WIDTH 4
#define WIDTH1 5
#define MAX_THREAD 4
typedef unsigned long long int ull;

int (*matrixC)[WIDTH1];
int reslt[WIDTH][WIDTH1];

void* count(void *arg) {
    int temp;
    int ops = 0;
    int i,j,k;
    for (i = 0; i < WIDTH; i++) { 
        for (j = 0; j < WIDTH1; j++) { 
            temp = matrixC[i][j];
            for(k = temp; k > 0; k--) {
                ops+=k;
            }
            reslt[i][j] = ops;
            ops = 0;
        }
    }
}

int main()
{
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int[WIDTH][WIDTH1]), IPC_CREAT | 0666);
    matrixC = shmat(shmid, 0, 0);
    pthread_t thread;
    printf("Hasil dari program 4a.c:\n");
    for (int i = 0; i < WIDTH; i++) { 
        for (int j = 0; j < WIDTH1; j++) { 
            printf("%d ", matrixC[i][j]);
        }
        printf("\n");
    }

    pthread_create(&thread, NULL, count, NULL);
    pthread_join(thread, NULL);

    printf("Hasil operasi pernjumlahan dari matriks 4a.c:\n");
    for (int i = 0; i < WIDTH; i++) { 
        for (int j = 0; j < WIDTH1; j++) { 
            printf("%d ", reslt[i][j]);
        }
        printf("\n");
    }
 
    return 0;
}
```

Pada bagian (b), diminta untuk menampilkan hasil perkalian matriks dari
program `4a.c`, kemudian menjumlahkan setiap angka dari matriks tersebut
dari n sampai 1.

```c
key_t key = 1234;
int shmid = shmget(key, sizeof(int[WIDTH][WIDTH1]), IPC_CREAT | 0666);
matrixC = shmat(shmid, 0, 0);
```

Dibuat unique key untuk digunakan sebagai key shared memory dengan program
`4a.c` yang sudah dibuat sebelumnya. Nilai dari `matrixC` ini yang akan 
saling berbagi antara program `4a.c` dengan `4b.c`.

```c
printf("Hasil dari program 4a.c:\n");
  for (int i = 0; i < WIDTH; i++) { 
    for (int j = 0; j < WIDTH1; j++) { 
      printf("%d ", matrixC[i][j]);
    }
  printf("\n");
}

pthread_create(&thread, NULL, count, NULL);
pthread_join(thread, NULL);

```

Bagian ini akan menampilkan nilai dari `matrixC` yang dimana tadinya pada
program `4a.c` melakukan share memory dengan program `4b.c`.

```c
void* count(void *arg) {
    int temp;
    int ops = 0;
    int i,j,k;
    for (i = 0; i < WIDTH; i++) { 
        for (j = 0; j < WIDTH1; j++) { 
            temp = matrixC[i][j];
            for(k = temp; k > 0; k--) {
                ops+=k;
            }
            reslt[i][j] = ops;
            ops = 0;
        }
    }
}
```

Penjumlahan dari angka yang terdapat pada matriks `matrixC` dilakukan dengan
thread dimana setiap angka akan disimpan terlebih dahulu di variabel `temp` dan
akan di jumlahkan secara iterasi. Hasilnya akan dikembalikan ke tiap ordo masing-masing.

```c
printf("Hasil operasi pernjumlahan dari matriks 4a.c:\n");
  for (int i = 0; i < WIDTH; i++) { 
    for (int j = 0; j < WIDTH1; j++) { 
      printf("%d ", reslt[i][j]);
     }
  printf("\n");
  }
```

Hasil dari penjumlahan akan di tampilkan dengan bagian diatas

#### 4(c)
Berikut adalah program `4c.c`.

```c
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
 
int pid;
int pipe1[2];
 
int main()
{
    if (pipe(pipe1) == -1)
    exit(1);
 
    if ((fork()) == 0) {
        dup2(pipe1[1], 1);
        close(pipe1[0]);
        close(pipe1[1]);
        char *argv1[] = {"ls", NULL};
        execv("/bin/ls", argv1);
    }
    else {
        dup2(pipe1[0],0);
        close(pipe1[0]);
        close(pipe1[1]);
        char *argv1[] = {"wc", "-l", NULL};
        execv("/usr/bin/wc", argv1);
    }
}
```

Pada bagian (c), kita diminta untuk menghitung jumlah file dan direktori yang
terdapat di dalam direktori saat ini dengan command `ls | wc -l`. Menggunakan 
pipe untuk membuat grep line disini, digunakan `dup2()` untuk write dan read 
dari pipe yang dibuat. `0` berarti read, dan `1` berarti write.
