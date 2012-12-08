FileTelePortation
====================

`FooFS` adalah sebuah aplikasi FTP file sistem sederhana yang memanfaatkan shared memory untuk interaksi
antar proses. Aplikasi `FooFS` mengimplementasi fungsi-fungsi untuk sederhana untuk akses file, seperti
membuat direktori, delete direktori, copy file, dsb.

Untuk kompilasi project ini, gunakan perintah `make all`. File executable `main`, untuk run panggil 
`make run`.

Fungsi-fungsi yang dapat dipanggil dari sisi client adalah:

+   `format @[filepath] [size]`, untuk membuat sebuah device yang mengimplementasi filesistem kita

+   `mountfs @[filepath] [mount_point]`, untuk memasukkan device tersebut pada filesistem manager kita

+   `umount [mount_point]`, untuk mengeluarkan device tersebut dari filesistem manager kita

+   `pwd`, untuk mencetak current directory filesistem manager

+   `ls <path>`, untuk mencetak isi direktori pada pth relatif pada current directory

+   `cd [path]`, untuk masuk pada sebuah direktori dari current directory

+   `mkdir [path]`, buat direktori baru

+   `rm [path]`, menghapus direktori atau file dari filesistem kita

+   `cp [@][path1] [@][path2]`, mengcopy file dari path1 ke path2

+   `mv [@][path1] [@][path2]`, memindahkan file dari path1 ke path2

+   `cat [path]`, mencetak isi dari file pada path

+   `exit`, keluar dari filesistem

