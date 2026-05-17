# Tugas-Praktikum-Algoritma-dan-Struktur-Data

          WhereIsIt? — Personal Inventory Management System

          
**Overview**

 Merupakan sistem manajemen inventaris pribadi berbasis baris perintah terminal antarmuka (CLI) yang membantu pengguna mencatat, mengelola, dan memantau barang-barang pribadi yang ditulis dalam Bahasa Pemrograman C++. Dirancang untuk penghuni kamar kos atau siapa saja yang ingin mencatat dan melacak barang-barang pribadi secara terstruktur, lengkap dengan lokasi, kondisi, stok, dan riwayat aktivitas.
 Program menyimpan data secara persisten dalam file teks berformat delimiter sehingga data tidak hilang saat program ditutup.
Program ditulis murni dalam C++ standar (C++11) tanpa library eksternal, mengimplementasikan manajemen memori heap secara manual, algoritma Bubble Sort pada parallel arrays, Shift-Left Deletion, Circular Buffer untuk log, serta File I/O dengan teknik delimiter escaping.


**Fitur**

 No,Fitur,Deskripsi
         
1,Tambah Barang,Menambah barang baru beserta nama, kategori, jumlah, harga, lokasi di kamar, kondisi, dan catatan opsional

2,Lihat Semua Barang,Menampilkan seluruh barang dalam format tabel yang terformat rapi

3,Cari Barang,Pencarian substring case-insensitive di seluruh field: nama, kategori, lokasi, dan catatan

4,Edit Barang,Mengubah detail barang; tekan Enter untuk mempertahankan nilai lama

5,Hapus Barang,Menghapus barang dengan konfirmasi; menggunakan algoritma Shift-Left Deletion

6,Filter per Kategori,Menampilkan daftar kategori unik lalu menyaring barang berdasarkan pilihan

7,Statistik Inventaris,Ringkasan total jenis barang, total stok, estimasi nilai, barang termahal, dan rekapitulasi kondisi

8,Update Stok Cepat,Tambah, kurangi, atau set langsung jumlah stok suatu barang tanpa masuk menu edit

9,Detail Barang,Menampilkan semua informasi lengkap satu barang termasuk total nilai (harga × qty)

10,Urutkan Barang,Mengurutkan berdasarkan Nama (A–Z), Jumlah (terbesar), atau Harga (terbesar) menggunakan Bubble Sort

11,Export ke CSV,Mengekspor seluruh data ke file `inventory_export.csv` yang dapat dibuka di Excel atau Google Sheets

12,Riwayat Aktivitas,Menampilkan 30 aktivitas terakhir (tambah, edit, hapus, dll.) beserta timestamp menggunakan Circular Buffer

13,Barang Bermasalah,Menyaring dan menampilkan barang dengan kondisi "Rusak" atau "Perlu Perbaikan"

14,Hapus Semua Data,Mereset seluruh inventaris; memerlukan konfirmasi ketik `HAPUS` → tidak dapat diurungkan


          **Anggota Kelompok:**
1. Talitha Syifa Al Fath nim 124250173
2. Jhosua Febrian Alexander Sibuea nim 124250179
