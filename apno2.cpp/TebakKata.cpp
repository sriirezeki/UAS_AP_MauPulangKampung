#include <iostream>   // Untuk cout dan cin
#include "TebakKata.h" // Untuk memanggil fungsi dan struktur data
int main() {
    
    srand(static_cast<unsigned int>(time(nullptr)));
    // srand() mengatur "titik awal" generator angka acak.
    // time(nullptr) mengembalikan waktu saat ini 
    // Hasilnya: setiap kali program dijalankan, kata yang muncul berbeda.
    bersihkanLayar(); // Fungsi untuk membersihkan layar
    tampilkanJudul(); // Fungsi untuk menampilkan judul game
    string namaPemain = inputNama(); // Fungsi untuk menyimpan nama pemain dan mengembalikannya sebagai string

    bool mainLagi = true;
    // Loop utama untuk mengulang permainan jika pemain memilih "main lagi"
    while (mainLagi) {
        // Bersihkan layar dan tampilkan judul di setiap ronde baru
        bersihkanLayar();
        tampilkanJudul();
        cout << "  Halo, " << namaPemain << "! Selamat datang di ronde baru.\n\n";

        // Pemain memilih level dan dapat jumlah nyawa sesuai level yang dipilih
        int maxNyawa = pilihLevel();

        // Pilih kata secara acak dari bankKata
        int    idxKata = rand() % jumlahKata; // rand() untuk menghasilkan angka acak
        string kata    = bankKata[idxKata]; // Pilih kata dari bankKata berdasarkan indeks acak

        KataGame game; // Struct untuk menyimpan permainan saat ini
        inisialisasiGame(&game, kata, maxNyawa); // Fungsi untuk mengisi struct game dengan data awal (kata, nyawa, dll)

        // Flag untuk menandai apakah pemain menang pada ronde ini
        bool menang = false;

        // Pemain terus menebak huruf selama masih punya nyawa.
        // Loop berhenti jika: nyawa habis ATAU pemain menang.
        while (game.sisaNyawa > 0) {

            // Tampilkan kondisi game terkini ke layar
            tampilkanStatus(&game);

            // Baca input huruf dari pemain
            char huruf = inputHuruf();

            // Cek apakah input valid (adalah huruf A-Z)
            if (huruf == '\0' || !isalpha(huruf)) {
                cout << "\n  [!] Input tidak valid! Masukkan huruf A-Z.\n";
                cout << "  Tekan Enter untuk melanjutkan...";
                cin.ignore(1000, '\n'); // Tunggu pemain tekan Enter
                continue;              // Kembali ke atas loop (minta input ulang)
            }

            // Cek apakah huruf sudah pernah ditebak
            if (cekHurufSudahDitebak(&game, huruf)) {
                cout << "\n  [!] Huruf '" << huruf << "' sudah pernah ditebak! Coba huruf lain.\n";
                cout << "  Tekan Enter untuk melanjutkan...";
                cin.ignore(1000, '\n');
                continue;
            }

            // FITUR HINT: Jika pemain menekan 'H'
            // Hint membuka satu huruf tersembunyi, tapi memotong 1 nyawa.
            if (huruf == 'H') {
                gunakanHint(&game);
                cout << "  Tekan Enter untuk melanjutkan...";
                cin.ignore(1000, '\n');

                // ek apakah kata sudah lengkap (menang karena hint)
                if (cekKemenangan(&game)) { menang = true; break; }
                continue;
            }

            // Fungsi untuk mengisi huruf yang benar atau mengurangi nyawa
            prosesTebakan(&game, huruf);

            // Cek apakah semua huruf sudah terbuka?
            if (cekKemenangan(&game)) { menang = true; break; }

        }

        tampilkanHasil(&game, menang, namaPemain); // fungsi untuk menampilkan hasil dari permainan
        
        int skor = hitungSkor(&game, maxNyawa);// Hitung skor berdasarkan sisa nyawa dan level yang dipilih
        
        simpanSkor(namaPemain, skor, maxNyawa);// Simpan skor ke leaderboard (akan diurutkan otomatis di dalam fungsi)

        tampilkanLeaderboard();// Tampilkan leaderboard yang sudah diperbarui

        // Tanya pemain ingin bermain lagi atau tidak
        // Input 'Y' (Ya) maka loop utama diulang (mainLagi = true)
        // Input selain 'N' maka loop berhenti (mainLagi = false)
        cout << "\n  Main lagi? (Y/N): ";
        char pilihanMainLagi;
        cin >> pilihanMainLagi;
        cin.ignore(1000, '\n'); // Bersihkan sisa karakter di buffer input

        mainLagi = (toupper(pilihanMainLagi) == 'Y');

    } 

    // Pesan ketika pemain tidak ingin bermain lagi
    cout << "\n  Sampai jumpa, " << namaPemain << "!\n";

    return 0; // Program selesai dengan status sukses
}