#ifndef HEADER_H // Mencegah  isi file diproses  lwbih dari sekali
#define HEADER_H // Jika HEADER_H sudah didefinisikan, maka isi file ini tidak akan diproses lagi.

#include <iostream>   // Untuk cin dan cout
#include <string>     // Untuk tipe data string
#include <cstdlib>    // Untuk fungsi rand(), srand(), system()
#include <ctime>      // Untuk time() untuk seed angka acak
#include <cctype>     // Untuk toupper(), isalpha()
#include <algorithm>  // Untuk swap() dipakai saat sorting leaderboard

using namespace std; // Agar tidak perlu menulis "std::" berulang kali

const int jumlahKata     = 15; // Total kata yang tersedia di bank kata
const int maxLeaderboard = 5;  // Maksimal entri yang disimpan di leaderboard

// Bank kata  yang berisi :
// Daftar kata yang akan ditebak berbentuk array
// Program akan memilih satu secara acak setiap ronde.
const string bankKata[jumlahKata] = {
    "BUKU", "MEJA", "KOPI", "PANTAI", "SEPEDA", "KOMPUTER", "PERPUSTAKAAN",
    "TEKNOLOGI", "PETUALANGAN", "KESEHATAN", "KONSTELASI", "PARADOKSAL",
    "METAMORFOSIS", "ELEKTROMAGNETIK", "INTERPRETATIF"
};


// Struct untuk menyimpan semua data saat ini dari satu sesi permainan.
struct KataGame {
    string kataAsli;        // Kata yang harus ditebak 
    string statusTebakan;   // Tampilan kata saat ini (dengan '_' untuk huruf tersembunyi)
    int sisaNyawa;          // Jumlah kesempatan tebak tersisa
    char tebakanSalah[26];  // Array untuk menyimpan huruf-huruf yang salah ditebak
    int jumlahSalah;        // Jumlah total tebakan yang salah
    bool sudahDitebak[26];  // Index 0-25 untuk abjad.
};

// Sruct untuk menyimpan data skor seorang pemain untuk ditampilkan di leaderboard.
struct Skor {
    string nama;        // Nama pemain
    int    nilai;       // Nilai skor yang diperoleh
    string kesulitan;   // Level yang dipilih ("Easy", "Medium", atau "Hard")
};

// Variabel Global
Skor leaderboard[maxLeaderboard]; // Array menyimpan top 5 skor tertinggi
int  jumlahSkorTersimpan = 0;     // Counter: sudah berapa skor yang tersimpan

// Fungsi untuk membersihkan layar
void bersihkanLayar() {
    system("cls");
}

Fungsi untuk menampilkan Header judul 
void tampilkanJudul() {
    cout << string(50, '=') << "\n"; 

    string judul = "GAME TEBAK KATA";
    // Hitung spasi agar teks tampil di tengah (50 karakter lebar layar)
    cout << string((50 - judul.length()) / 2, ' ') << judul << "\n";

    cout << string(50, '=') << "\n";
}

// Fungsi untuk membaca satu karakter huruf dari pemain.
char inputHuruf() {
    string input;
    getline(cin, input);                      // Baca seluruh baris input
    if (!input.empty()) return toupper(input[0]); // Ambil karakter pertama, ubah jadi huruf besar
    return '\0';                              // Kembalikan null jika input kosong
}

// Fungsi untuk memminta pemain memasukkan namanya di awal game.
string inputNama() {
    string nama;
    cout << "  Masukkan nama Anda: ";
    getline(cin, nama);               // Baca nama (bisa pakai spasi)
    if (nama.empty()) nama = "Pemain"; // Fallback jika kosong
    return nama;
}

// Fungsi untuk menampilkan menu pemilihan level dan memvalidasi input pemain.
int pilihLevel() {
    int pilihan = 0;
    cout << "  Pilih Level:\n";
    cout << string(50, '-') << "\n";
    cout << "  1. Easy   - 8 kali maksimal kesalahan\n";
    cout << "  2. Medium - 6 kali maksimal kesalahan\n";
    cout << "  3. Hard   - 4 kali maksimal kesalahan\n";
    cout << string(50, '-') << "\n";
    cout << "  Pilihan Anda (1-3): ";
    cin >> pilihan;

    // Validasi sampai pemain memasukkan angka 1, 2, atau 3
    while (pilihan < 1 || pilihan > 3) {
        cin.clear();                   // Reset error flag cin
        cin.ignore(1000, '\n');        // Buang karakter sisa di buffer
        cout << "  Pilihan tidak valid! Silakan masukkan angka 1-3: ";
        cin >> pilihan;
    }
    cin.ignore(1000, '\n'); // Bersihkan buffer setelah input valid

    // Kembalikan jumlah nyawa sesuai pilihan
    if (pilihan == 1) return 8;
    if (pilihan == 2) return 6;
    return 4;
}

// Fungsi untuk mengonversi jumlah nyawa menjadi nama level (string).
string namaLevel(int maxNyawa) {
    if (maxNyawa == 8) return "Easy";
    if (maxNyawa == 6) return "Medium";
    return "Hard";
}

// Fungsi untuk inisialisasi game
void inisialisasiGame(KataGame* game, const string& kata, int maxNyawa) {
    game->kataAsli    = kata;      // Simpan kata asli
    game->sisaNyawa   = maxNyawa;  // Set nyawa awal
    game->jumlahSalah = 0;         // Reset hitungan salah

    // Untuk buat semua huruf jadi '_', spasi tetap ' ' di statusTebakan
    game->statusTebakan = "";
    for (char c : kata) {
        game->statusTebakan += (c == ' ') ? ' ' : '_';
    }

    // Reset semua slot jika tidak ada huruf salah, tidak ada huruf yang ditebak
    for (int i = 0; i < 26; i++) {
        game->tebakanSalah[i] = '\0';   // Kosongkan huruf salah
        game->sudahDitebak[i] = false;  // Tandai semua huruf belum pernah ditebak
    }
}

// Fungsi untuk mengecek apakah huruf tertentu sudah pernah ditebak sebelumnya.
// Returntrue jika sudah pernah ditebak, false jika belum
// Dimana Huruf 'A' = index 0, 'B' = index 1, dst. ('huruf' - 'A')
// ============================================================
bool cekHurufSudahDitebak(KataGame* game, char huruf) {
    return game->sudahDitebak[huruf - 'A'];
}


// Fungsi untuk memproses tebakan huruf dari pemain.
// - Jika benar, buka posisi huruf di statusTebakan
// - Jika salah,  catat huruf salah, kurangi nyawa
bool prosesTebakan(KataGame* game, char huruf) {
    bool benar = false;

    // Tandai huruf ini sudah ditebak
    game->sudahDitebak[huruf - 'A'] = true;

    // Periksa setiap karakter dalam kata asli
    for (size_t i = 0; i < game->kataAsli.size(); i++) {
        if (game->kataAsli[i] == huruf) {
            game->statusTebakan[i] = huruf; // Buka huruf di posisi yang cocok
            benar = true;
        }
    }

    // Jika huruf tidak ditemukan di kata maka tebakan salah
    if (!benar) {
        game->tebakanSalah[game->jumlahSalah] = huruf; // Simpan ke daftar salah
        game->jumlahSalah++;                            // Tambah counter salah
        game->sisaNyawa--;                              // Kurangi nyawa
    }

    return benar;
}


// Fungsi untuk mengecek apakah pemain sudah menang (semua huruf terbuka).
// true jika tidak ada lagi '_' di statusTebakan, false jika masih ada berarti belum menang
bool cekKemenangan(KataGame* game) {
    for (char c : game->statusTebakan) {
        if (c == '_') return false; // Masih ada yang belum terbuka
    }
    return true; // Semua huruf sudah terbuka berarti MENANG
}

// Fungsi untuk membuka satu huruf tersembunyi secara otomatis sebagai bantuan.
//  Menggunakan 1 nyawa. yang tidak bisa digunakan jika nyawa hanya 1 maka akan langsung kalah
void gunakanHint(KataGame* game) {
    // Tolak jika nyawa hanya 1 (menggunakan hint akan langsung kalah)
    if (game->sisaNyawa <= 1) {
        cout << "\n  [!] Nyawa tidak cukup untuk menggunakan hint!\n";
        return;
    }

    // Cari huruf pertama yang belum terbuka
    for (size_t i = 0; i < game->kataAsli.size(); i++) {
        if (game->statusTebakan[i] == '_') {
            char hurufHint = game->kataAsli[i]; // Ambil huruf asli di posisi tersebut

            // Buka semua posisi dengan huruf yang sama (satu hint bisa membuka beberapa posisi)
            for (size_t j = 0; j < game->kataAsli.size(); j++) {
                if (game->kataAsli[j] == hurufHint) {
                    game->statusTebakan[j] = hurufHint;
                }
            }

            game->sudahDitebak[hurufHint - 'A'] = true; // Tandai sudah ditebak
            game->sisaNyawa--;                           // Potong 1 nyawa

            cout << "\n  [HINT] Huruf '" << hurufHint << "' dibuka! (-1 nyawa)\n";
            return; // Selesai, hanya buka satu jenis huruf per hint
        }
    }

    // Jika tidak ada huruf yang bisa dibuka (seharusnya tidak terjadi)
    cout << "\n  [i] Tidak ada huruf yang bisa dibuka.\n";
}


// Fungsi untuk menampilkan kondisi game saat ini ke layar:
// - Kata yang sedang ditebak (dengan '_' untuk huruf tersembunyi)
// - Sisa nyawa, ditampilkan sebagai (*)
// - Daftar huruf yang sudah salah ditebak
// - input untuk pemain
void tampilkanStatus(KataGame* game) {
    bersihkanLayar();
    tampilkanJudul();

    // Tampilkan statusTebakan dengan spasi antar huruf agar mudah dibaca
    cout << "  Kata  : ";
    for (char c : game->statusTebakan) cout << c << " ";
    cout << "\n\n";

    // Tampilkan sisa nyawa dalam bentuk visual (*)
    cout << "  Nyawa : ";
    for (int i = 0; i < game->sisaNyawa; i++) cout << "[*] ";
    cout << "\n";

    // Tampilkan huruf-huruf yang sudah salah ditebak
    cout << "  Salah : ";
    if (game->jumlahSalah == 0) {
        cout << "(belum ada)";
    } else {
        for (int i = 0; i < game->jumlahSalah; i++) {
            cout << game->tebakanSalah[i];
            if (i < game->jumlahSalah - 1) cout << ", "; // Pemisah antar huruf salah
        }
    }
    cout << "\n\n";

    // Informasi hint dan prompt input
    cout << "  [H] = Gunakan Hint (-1 nyawa)\n";
    cout << "  Masukkan huruf tebakan > ";
}


// Fungsi untuk menampilkan layar akhir permainan.
// - Jika menang akan tampilkan pesan selamat dan kata yang benar
// - Jika kalah akan tampilkan pesan game over dan kata yang benar
void tampilkanHasil(KataGame* game, bool menang, const string& nama) {
    bersihkanLayar();
    tampilkanJudul();

    if (menang) {
        cout << "  === SELAMAT " << nama << "! ANDA MENANG! ===\n";
        cout << "  Kata yang benar: [ " << game->kataAsli << " ]\n";
    } else {
        cout << "  === GAME OVER! Nyawa habis. ===\n";
        cout << "  Kata sebenarnya: [ " << game->kataAsli << " ]\n";
    }
    cout << "\n  Terima kasih sudah bermain!\n\n";
}

// Fungsi untuk menghitung skor pemain berdasarkan sisa nyawa dan level.
// Skor = sisaNyawa × multiplier × 10
// Multiplier: Easy=1, Medium=2, Hard=3 (level sulit maka poin lebih besar)
// Akan menampilkan skor pemain
int hitungSkor(KataGame* game, int maxNyawa) {
    int multiplier = 1;
    if      (maxNyawa == 8) multiplier = 1; // Easy, multiplier kecil
    else if (maxNyawa == 6) multiplier = 2; // Medium, multiplier sedang
    else if (maxNyawa == 4) multiplier = 3; // Hard,multiplier besar

    return game->sisaNyawa * multiplier * 10;
}


// Fungsi untuk menyimpan skor baru ke array leaderboard dan mengurutkannya
// - Jika leaderboard belum penuh (< 5 entri) maka langsung tambahkan
// - Jika sudah penuh: cari skor terkecil, ganti jika skor baru lebih tinggi
// - Setelah itu, urutkan leaderboard dari skor tertinggi ke terendah menggunakan algoritma Bubble Sort

void simpanSkor(const string& nama, int skor, int maxNyawa) {
    string namaKesulitan = namaLevel(maxNyawa);

    // Jika Leaderboard masih ada tempat
    if (jumlahSkorTersimpan < maxLeaderboard) {
        leaderboard[jumlahSkorTersimpan++] = {nama, skor, namaKesulitan};
    }
    // JikaLeaderboard sudah penuh maka cek apakah skor baru layak masuk
    else {
        // Cari index skor terkecil di leaderboard
        int idxTerkecil = 0;
        for (int i = 1; i < jumlahSkorTersimpan; i++) {
            if (leaderboard[i].nilai < leaderboard[idxTerkecil].nilai)
                idxTerkecil = i;
        }
        // Ganti entri terkecil jika skor baru lebih tinggi
        if (skor > leaderboard[idxTerkecil].nilai)
            leaderboard[idxTerkecil] = {nama, skor, namaKesulitan};
    }

    // Urutkan leaderboard: skor tertinggi di atas (Bubble Sort)
    // Setiap iterasi, elemen terkecil "naik" ke atas (nilai besar = posisi atas)
    for (int i = 0; i < jumlahSkorTersimpan - 1; i++) {
        for (int j = 0; j < jumlahSkorTersimpan - i - 1; j++) {
            if (leaderboard[j].nilai < leaderboard[j + 1].nilai)
                swap(leaderboard[j], leaderboard[j + 1]); // Tukar posisi
        }
    }
}


// Fungsi untuk menampilkan tabel top skor ke layar.
// Menggunakan printf() untuk format kolom yang rapi.
void tampilkanLeaderboard() {
    cout << string(50, '=') << "\n";

    string judulBoard = "LEADERBOARD";
    cout << string((50 - judulBoard.length()) / 2, ' ') << judulBoard << "\n";

    cout << string(50, '-') << "\n";
    // Format header tabel: No | Nama | Skor | Level
    printf("  %-3s | %-15s | %5s | %-6s\n", "No", "Nama", "Skor", "Level");
    cout << string(50, '-') << "\n";

    if (jumlahSkorTersimpan == 0) {
        cout << "  (Belum ada skor)\n";
    } else {
        // Tampilkan setiap entri leaderboard
        for (int i = 0; i < jumlahSkorTersimpan; i++) {
            string no   = to_string(i + 1) + ".";
            string nama = leaderboard[i].nama;

            // Potong nama jika lebih dari 15 karakter agar tabel tetap rapi
            if ((int)nama.length() > 15) nama = nama.substr(0, 15);

            printf("  %-3s | %-15s | %5d | %-6s\n",
                no.c_str(),
                nama.c_str(),
                leaderboard[i].nilai,
                leaderboard[i].kesulitan.c_str());
        }
    }

    cout << string(50, '=') << "\n";
}

#endif // Akhir dari header guard HEADER_H