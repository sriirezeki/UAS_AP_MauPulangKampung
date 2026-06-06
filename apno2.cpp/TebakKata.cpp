#include <iostream>
#include "TebakKata.h"

int main() {
    // Inisialisasi seed acak berdasarkan waktu saat ini
    srand(static_cast<unsigned int>(time(nullptr)));

    bool mainLagi = true;
    string namaPemain = "";

    while (mainLagi) {
        bersihkanLayar();
        tampilkanJudul();

        // Input nama pemain di setiap ronde baru
        namaPemain = inputNama();
        cout << "\n  Halo, " << namaPemain << "! Selamat datang di ronde baru.\n\n";

        // Pemain memilih level (menentukan jumlah nyawa)
        int maxNyawa = pilihLevel();

        // Pilih kata secara acak dari bank kata
        int    idxKata = rand() % JUMLAH_KATA;
        string kata    = bankKata[idxKata];

        // Inisialisasi state permainan via pointer
        KataGame game;
        inisialisasiGame(&game, kata, maxNyawa);

        bool menang = false;

        // Loop terus sampai pemain menang atau nyawa habis
        while (game.sisaNyawa > 0) {
            tampilkanStatus(&game);

            char huruf = inputHuruf();

            // Cek hint sebbelum validasi isalpha, karena '!' bukan huruf
            if (huruf == '!') {
                gunakanHint(&game);
                cout << "  Tekan Enter untuk melanjutkan...";
                cin.ignore(1000, '\n');
                // Cek menang setelah hint (kata bisa langsung terbuka)
                if (cekKemenangan(&game)) { menang = true; break; }
                continue;
            }

            // Cek input huruf atau bukan
            if (huruf == '\0' || !isalpha((unsigned char)huruf)) {
                cout << "\n  [!] Input tidak valid! Masukkan satu huruf A-Z atau '!' untuk hint.\n";
                cout << "  Tekan Enter untuk melanjutkan...";
                cin.ignore(1000, '\n');
                continue;
            }

            // Cek apakah huruf sudah pernah ditebak?
            if (cekHurufSudahDitebak(&game, huruf)) {
                cout << "\n  [!] Huruf '" << huruf << "' sudah pernah ditebak! Coba huruf lain.\n";
                cout << "  Tekan Enter untuk melanjutkan...";
                cin.ignore(1000, '\n');
                continue; // Nyawa TIDAK berkurang
            }

            // Proses tebakan: benar -> buka huruf, salah -> kurangi nyawa
            bool benar = prosesTebakan(&game, huruf);

            if (benar) {
                cout << "\n Benar! Huruf '" << huruf << "' ada dalam kata.\n";
            } else {
                cout << "\n Salah! Huruf '" << huruf << "' tidak ada. Nyawa berkurang!\n";
            }

            // Cek kondisi menang setelah tebakan
            if (cekKemenangan(&game)) { menang = true; break; }

            cout << "  Tekan Enter untuk melanjutkan...";
            cin.ignore(1000, '\n');
        }

        // Tampilkan hasil akhir ronde
        tampilkanHasil(&game, menang, namaPemain);

        // Simpan skor hanya jika menang (ketentuan soal)
        if (menang) {
            int skor = hitungSkor(&game, maxNyawa);
            simpanSkor(namaPemain, skor, maxNyawa);
            cout << "  Skor Anda: " << skor << " poin\n\n";
        } else {
            cout << "  Skor: 0 (tidak menang)\n\n";
        }

        // Tampilkan leaderboard setelah setiap ronde
        tampilkanLeaderboard();

        // Tanya apakah ingin main lagi
        cout << "\n  Apakah Anda ingin bermain lagi? (Y/N): ";
        char pilihanMainLagi;
        cin >> pilihanMainLagi;
        cin.ignore(1000, '\n');
        mainLagi = (toupper((unsigned char)pilihanMainLagi) == 'Y');
    }

    cout << "\n  Sampai jumpa, " << namaPemain << "! Terima kasih sudah bermain.\n\n";
    return 0;
}
