# Proyek-Akhir
Sistem Kendali Irigasi Tanaman Cabai Berbasis Arduino

Proyek akhir yang telah diselesaikan dimaksudkan sebagai solusi dari 
masalah yang dialami para petani khususnya dalam menggunakan pengatur waktu 
(timer) dalam upaya mengatur penjadwalan irigasi tetes. Cara ini masih kurang 
efektif dalam memberi irigasi yang sesuai dengan kebutuhan air tanaman karena 
hanya mampu mengatur pemberian irigasi berdasarkan interval waktu yang telah 
ditentukan saja, sehingga kelebihan maupun kekurangan air tidak dapat 
dikendalikan.  
Purwarupa Kendali Irigasi Tanaman Cabai Berbasis Arduino menggunakan 
cabai sebagai objek implementasi sistem irigasi. Purwarupa terdiri dari 
mikrokontroler Arduino Uno sebagai pengendali katup solenoid valve dan pengolah 
data soil moisture sensor YL-69 untuk mendeteksi resistansi kelembaban tanah. 
Data akan disimpan dalam bentuk file pada SD card dengan format ekstensi text 
document (*.txt) dan ditampilkan ke LCD. Katup solenoid valve akan terbuka 
otomatis ketika kadar air tanah berada pada titik krisis dan tertutup ketika 
kandungan air lebih tinggi dari kapasitas lapang. Sebagai penandaan status proses 
yang dijalankan oleh Arduino Uno, modul SIM800L V2 akan mengirimkan 
informasi status ke nomor pengguna. 
Berdasarkan hasil implementasi dan pengujian purwarupa kendali irigasi, 
maka sistem berjalan sesuai rancangan dan mampu mengatur pemberiaan air irigasi 
sesuai dengan kebutuhan tanaman (efisiensi terhadap penggunaan air irigasi) dan 
infomasi status akan dikirim melalui SMS. 

 

Kata Kunci: Irigasi Tetes, Arduino Uno, SMS, Sensor Kelembaban YL-69, SD 
Card. 
