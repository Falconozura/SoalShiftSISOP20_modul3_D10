# SoalShiftSISOP20_modul3_D10
## Soal 1 (Belum Selesai)
## Soal 2
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
## Soal 4
