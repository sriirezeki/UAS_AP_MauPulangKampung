// Guard agar header ini tidak di proses lebih dari sekali
#ifndef TEBAKKATA_H
#define TEBAKKATA_H

#include <iostream>   // untuk cout, cin
#include <string>     // untuk tipe data string
#include <cstdlib>    // untuk rand(), srand()
#include <ctime>      // untuk time() sebagai seed acak
#include <cctype>     // untuk toupper(), isalpha()
#include <algorithm>  // untuk swap() digunakan saat sorting leaderboard

using namespace std;

const int JUMLAH_KATA     = 15; // Total kata yang tersedia di bank kata
const int MAX_LEADERBOARD = 5;  // Maksimal entri yang disimpan di leaderboard

// Bank kata berisi daftar kata rahasia yang bisa muncul dalam permainan
// Dipilih secara acak setiap ronde menggunakan rand()
const string bankKata[JUMLAH_KATA] = {
    "BUKU", "MEJA", "KOPI", "PANTAI", "SEPEDA",
    "KOMPUTER", "PERPUSTAKAAN", "TEKNOLOGI", "PETUALANGAN",
    "KESEHATAN", "KONSTELASI", "PARADOKSAL",
    "METAMORFOSIS", "ELEKTROMAGNETIK", "INTERPRETATIF"
};

// Struct untuk menyimpan semua data yang berubah selama satu ronde bermain.
//  Dikirim ke fungsi melalui pointer agar bisa diubah langsung.
// ============================================================
struct KataGame {
    string kataAsli;         // Kata rahasia yang harus ditebak pemain
    string statusTebakan;    // Tampilan kata saat ini,menggunakan '_' untuk huruf yang belum terbuka
    int    sisaNyawa;        // Jumlah nyawa yang masih tersisa
    char   tebakanSalah[26]; // Array untuk menyimpan huruf-huruf yang salah ditebak
    int    jumlahSalah;      // Jumlah kesalahan yang sudah dilakukan
    bool   sudahDitebak[26]; // Penanda apakah huruf sudah pernah ditebak
    bool   hintSudahDipakai; // Untuk memberi tau hint sudah dipakai di ronde ini, tidak bisa dipakai lagi
};

// Struct untuk menyimpan satu baris data di leaderboard
struct Skor {
    string nama;       // Nama pemain
    int    nilai;      // Skor yang didapat
    string kesulitan;  // Level yang dipilih
};

// Variabel global untuk menyimpan leaderboard
static Skor leaderboard[MAX_LEADERBOARD]; // Array 5 skor tertinggi
static int  jumlahSkorTersimpan = 0;      // Berapa entri yang sudah terisi

// Fungsi untuk membersihkan layar terminal
void bersihkanLayar() {
    system("cls");
}

// Mencetak judul game dengan garis pembatas di atas dan bawah
void tampilkanJudul() {
    cout << string(50, '=') << "\n";
    string judul = "GAME TEBAK KATA";
    // Hitung spasi agar judul tampil di tengah (lebar 50 karakter)
    cout << string((50 - (int)judul.length()) / 2, ' ') << judul << "\n";
    cout << string(50, '=') << "\n";
}

// Meminta pemain input namanya
// Jika tidak diisi (langsung Enter), nama default menjadi "Pemain"
string inputNama() {
    string nama;
    cout << "  Masukkan nama Anda: ";
    getline(cin, nama); // getline agar bisa baca nama dengan spasi
    if (nama.empty()) nama = "Pemain";
    return nama;
}

// Membaca satu karakter dari input pemain
// Otomatis diubah ke huruf kapital (A-Z)
// Mengembalikan '\0' jika pemain hanya tekan Enter tanpa mengetik apapun
char inputHuruf() {
    string input;
    getline(cin, input);
    if (!input.empty()) return toupper((unsigned char)input[0]);
    return '\0';
}

// Menampilkan menu pilihan level dan meminta pemain memilih
// Mengembalikan jumlah nyawa sesuai level yang dipilih
//   Easy = 8 nyawa, Medium = 6 nyawa, Hard = 4 nyawa
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

    // Ulangi terus selama input tidak valid (bukan angka 1, 2, atau 3)
    while (cin.fail() || pilihan < 1 || pilihan > 3) {
        cin.clear();             // Reset flag error cin
        cin.ignore(1000, '\n');  // Buang karakter yang tersisa di buffer
        cout << "  Pilihan tidak valid! Masukkan angka 1-3: ";
        cin >> pilihan;
    }
    cin.ignore(1000, '\n'); // Buang sisa newline agar getline berikutnya tidak terganggu

    if (pilihan == 1) return 8;
    if (pilihan == 2) return 6;
    return 4; // pilihan == 3 maka Hard
}

// Mengubah jumlah nyawa menjadi nama level yang mudah dibaca
string namaLevel(int maxNyawa) {
    if (maxNyawa == 8) return "Easy";
    if (maxNyawa == 6) return "Medium";
    return "Hard";
}


// Fungsi untuk menyimpan satu ronde baru
// Mengisi kataAsli dengan kata yang dipilih
// Membuat statusTebakan berisi semua '_' (belum ada yang terbuka)
// Mengatur nyawa awal sesuai level
// Mereset semua riwayat tebakan
void inisialisasiGame(KataGame* game, const string& kata, int maxNyawa) {
    game->kataAsli        = kata;
    game->sisaNyawa       = maxNyawa;
    game->jumlahSalah     = 0;
    game->hintSudahDipakai = false;

    // Buat statusTebakan: setiap huruf jadi '_', spasi tetap spasi
    game->statusTebakan = "";
    for (char c : kata) {
        game->statusTebakan += (c == ' ') ? ' ' : '_';
    }

    // Reset semua slot tebakan salah dan flag sudahDitebak
    for (int i = 0; i < 26; i++) {
        game->tebakanSalah[i] = '\0';   // Kosongkan riwayat salah
        game->sudahDitebak[i] = false;  // Semua huruf belum pernah ditebak
    }
}

// Cek apakah huruf sudah pernah ditebak?
// Huruf 'A' = indeks 0, 'B' = indeks 1, dst (huruf - 'A')
// Mengembalikan true jika sudah pernah ditebak
bool cekHurufSudahDitebak(const KataGame* game, char huruf) {
    return game->sudahDitebak[huruf - 'A'];
}

// Memproses satu tebakan huruf dari pemain:
// Tandai huruf sebagai sudah ditebak
// Jika benar, buka semua posisi huruf itu di statusTebakan
// Jika salah, catat di tebakanSalah dan kurangi nyawa
// Mengembalikan true jika tebakan benar
bool prosesTebakan(KataGame* game, char huruf) {
    bool benar = false;

    // Catat bahwa huruf ini sudah pernah ditebak
    game->sudahDitebak[huruf - 'A'] = true;

    // Cari huruf di setiap posisi kata asli
    for (size_t i = 0; i < game->kataAsli.size(); i++) {
        if (game->kataAsli[i] == huruf) {
            game->statusTebakan[i] = huruf; // Ganti '_' dengan huruf yang benar
            benar = true;
        }
    }

    // Jika huruf tidak ditemukan di manapun → salah
    if (!benar) {
        game->tebakanSalah[game->jumlahSalah] = huruf; // Simpan ke riwayat salah
        game->jumlahSalah++;
        game->sisaNyawa--; // Kurangi nyawa
    }

    return benar;
}

// Cek apakah pemain sudah menang
// Jika tidak ada lagi '_' di statusTebakan, berarti semua huruf sudah terbuka berarti menang
// Mengembalikan true jika semua huruf sudah terbuka
bool cekKemenangan(const KataGame* game) {
    for (char c : game->statusTebakan) {
        if (c == '_') return false; // Masih ada yang belum terbuka
    }
    return true; // Semua sudah terbuka
}

// Fitur hint untuk membuka  satu huruf yang masih terrsembunyi di statusTebakan
void gunakanHint(KataGame* game) {
    // Cek apakah hint sudah pernah dipakai di ronde ini
    if (game->hintSudahDipakai) {
        cout << "\n  [!] Hint sudah digunakan di ronde ini!\n";
        return;
    }

    // Cek nyawa cukup atau tidak
    if (game->sisaNyawa <= 1) {
        cout << "\n  [!] Nyawa tidak cukup untuk menggunakan hint!\n";
        return;
    }

    // Cari posisi '_' pertama di statusTebakan
    for (size_t i = 0; i < game->kataAsli.size(); i++) {
        if (game->statusTebakan[i] == '_') {
            char hurufHint = game->kataAsli[i]; // Ambil huruf asli di posisi itu

            // Buka semua posisi yang memiliki huruf yang sama
            for (size_t j = 0; j < game->kataAsli.size(); j++) {
                if (game->kataAsli[j] == hurufHint) {
                    game->statusTebakan[j] = hurufHint;
                }
            }

            // Tandai huruf hint sebagai sudah ditebak (tidak bisa ditebak manual lagi)
            game->sudahDitebak[hurufHint - 'A'] = true;
            game->sisaNyawa--;          // Bayar 1 nyawa sebagai biaya hint
            game->hintSudahDipakai = true; // Kunci hint agar tidak bisa dipakai lagi

            cout << "\n  [HINT] Huruf '" << hurufHint << "' dibuka! (-1 nyawa)\n";
            return; // Selesai, tidak perlu cari posisi lagi
        }
    }

    // Sampai sini berarti tidak ada '_' tersisa → kata sudah hampir terbuka semua
    cout << "\n  [i] Semua huruf sudah terbuka, hint tidak diperlukan.\n";
}

// Untuk menampilkan status permainan saat ini ke layar
void tampilkanStatus(const KataGame* game) {
    bersihkanLayar();
    tampilkanJudul();

    // Cetak kata dengan spasi antar huruf agar mudah dibaca
    cout << "  Kata  : ";
    for (char c : game->statusTebakan) cout << c << " ";
    cout << "\n\n";

    // Cetak simbol nyawa: setiap [*] = 1 nyawa tersisa
    cout << "  Nyawa : ";
    for (int i = 0; i < game->sisaNyawa; i++) cout << "[*] ";
    cout << "\n";

    // Cetak huruf-huruf yang sudah salah ditebak
    cout << "  Salah : ";
    if (game->jumlahSalah == 0) {
        cout << "(belum ada)"; // Belum ada kesalahan
    } else {
        for (int i = 0; i < game->jumlahSalah; i++) {
            cout << game->tebakanSalah[i];
            if (i < game->jumlahSalah - 1) cout << ", "; // Pisahkan dengan koma
        }
    }
    cout << "\n\n";

    // Tampilkan info hint sesuai kondisi
    if (!game->hintSudahDipakai) {
        cout << "  [!] = Gunakan Hint (-1 nyawa, sekali per ronde)\n";
    } else {
        cout << "  [Hint sudah digunakan]\n";
    }
    cout << "  Masukkan huruf tebakan (atau '!' untuk hint) > ";
}

// Menampilkan layar hasil akhir setelah ronde selesai
// Menampilkan pesan yang berbeda untuk menang atau kalah
void tampilkanHasil(const KataGame* game, bool menang, const string& nama) {
    bersihkanLayar();
    tampilkanJudul();

    if (menang) {
        // Pemain berhasil menebak semua huruf sebelum nyawa habis
        cout << "  *** SELAMAT, " << nama << "! ANDA MENANG! ***\n\n";
        cout << "  Kata yang benar : [ " << game->kataAsli << " ]\n";
        cout << "  Sisa nyawa      : " << game->sisaNyawa << "\n";
    } else {
        // Nyawa habis sebelum kata berhasil ditebak
        cout << "  === GAME OVER! Nyawa habis. ===\n\n";
        cout << "  Kata sebenarnya : [ " << game->kataAsli << " ]\n"; // Ungkap jawabannya
    }
    cout << "\n  Terima kasih sudah bermain!\n\n";
}

// Fungsi untuk menghitung skor pemain setelah menang
// Rumusnya: sisaNyawa × multiplier × 10
// Multiplier makin besar untuk level yang lebih sulit
int hitungSkor(const KataGame* game, int maxNyawa) {
    int multiplier = 1;
    if      (maxNyawa == 8) multiplier = 1; // Easy
    else if (maxNyawa == 6) multiplier = 2; // Medium
    else                    multiplier = 3; // Hard
    return game->sisaNyawa * multiplier * 10;
}

// Fungsi untuk menyimpan skor baru ke dalam array leaderboard
void simpanSkor(const string& nama, int skor, int maxNyawa) {
    string namaKesulitan = namaLevel(maxNyawa);

    if (jumlahSkorTersimpan < MAX_LEADERBOARD) {
        // Leaderboard masih ada tempat maka langsung masukkan
        leaderboard[jumlahSkorTersimpan++] = {nama, skor, namaKesulitan};
    } else {
        // Leaderboard sudah penuh maka cari posisi skor terendah
        int idxTerkecil = 0;
        for (int i = 1; i < jumlahSkorTersimpan; i++) {
            if (leaderboard[i].nilai < leaderboard[idxTerkecil].nilai)
                idxTerkecil = i;
        }
        // Ganti hanya jika skor baru lebih tinggi dari skor terendah
        if (skor > leaderboard[idxTerkecil].nilai)
            leaderboard[idxTerkecil] = {nama, skor, namaKesulitan};
    }

    // Urutkan leaderboard dari skor terbesar ke terkecil menggunakan bubble sort 
    for (int i = 0; i < jumlahSkorTersimpan - 1; i++) {
        for (int j = 0; j < jumlahSkorTersimpan - i - 1; j++) {
            if (leaderboard[j].nilai < leaderboard[j + 1].nilai)
                swap(leaderboard[j], leaderboard[j + 1]); // Tukar posisi
        }
    }
}

// Mencetak tabel leaderboard 5 besar ke layar
// Format tabel: No | Nama | Skor | Level
void tampilkanLeaderboard() {
    cout << string(50, '=') << "\n";
    string judulBoard = "LEADERBOARD TOP 5";
    cout << string((50 - (int)judulBoard.length()) / 2, ' ') << judulBoard << "\n";
    cout << string(50, '-') << "\n";
    printf("  %-3s | %-15s | %5s | %-8s\n", "No", "Nama", "Skor", "Level");
    cout << string(50, '-') << "\n";

    if (jumlahSkorTersimpan == 0) {
        cout << "  (Belum ada skor tersimpan)\n"; // Belum ada yang pernah menang
    } else {
        // Cetak tiap baris leaderboard
        for (int i = 0; i < jumlahSkorTersimpan; i++) {
            string no   = to_string(i + 1) + ".";
            string nama = leaderboard[i].nama;
            // Potong nama jika terlalu panjang agar tabel tetap rapi
            if ((int)nama.length() > 15) nama = nama.substr(0, 12) + "...";

            printf("  %-3s | %-15s | %5d | %-8s\n",
                no.c_str(),
                nama.c_str(),
                leaderboard[i].nilai,
                leaderboard[i].kesulitan.c_str());
        }
    }
    cout << string(50, '=') << "\n";
}

#endif 
