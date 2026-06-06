#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;

// ==================== STRUCT DEFINISI ====================

struct Kendaraan {
    string platMobil;
    string merkMobil;
    string warnaMobil;
    time_t waktuMasuk;
};

struct Petugas {
    string nama;
    string id;
};

// ==================== FUNGSI UTILITAS ====================

void cetakGaris(char karakter = '=', int panjang = 60) {
    for (int i = 0; i < panjang; i++) cout << karakter;
    cout << endl;
}

void cetakJudul() {
    cetakGaris();
    cetakGaris('-');
    cout << setw(35) << "S E L A M A T   D A T A N G" << endl;
    cout << setw(38) << "DI PORTAL PARKIR KHUSUS MOBIL" << endl;
    cout << setw(28) << "MALL PRIENAL" << endl;
    cetakGaris();
}

void hitungWaktuParkir(time_t waktuMasuk, int &jam, int &menit, int &detik) {
    time_t sekarang = time(nullptr);
    double selisih = difftime(sekarang, waktuMasuk);
    jam   = (int)selisih / 3600;
    menit = ((int)selisih % 3600) / 60;
    detik = (int)selisih % 60;
}

long long hitungBiaya(time_t waktuMasuk) {
    time_t sekarang = time(nullptr);
    double selisih  = difftime(sekarang, waktuMasuk); // dalam detik
    // Rp2 per detik (sesuai contoh output: ~488 detik = Rp488)
    long long biaya = (long long)selisih * 2;
    return biaya;
}

// ==================== FUNGSI INPUT PETUGAS ====================

Petugas* inputPetugas() {
    Petugas* p = new Petugas();
    cout << endl;
    cout << "Kami senang Anda telah hadir." << endl;
    cout << "Sebelum melanjutkan, silahkan masukkan informasi Anda yang akan bertugas hari ini." << endl;
    cout << endl;
    cout << "Masukkan nama : ";
    getline(cin, p->nama);
    cout << "Masukkan ID   : ";
    getline(cin, p->id);
    return p;
}

void tampilkanPetugas(const Petugas* p) {
    cetakGaris();
    cout << "Nama Petugas : " << p->nama << endl;
    cout << "ID Petugas   : " << p->id   << endl;
    cout << endl;
    cout << setw(38) << "TERIMA KASIH ATAS KERJASAMANYA !" << endl;
    cetakGaris('-');
    cout << setw(32) << "S E L A M A T   B E R T U G A S" << endl;
    cetakGaris('-');
    cetakGaris();
}

// ==================== FUNGSI MENU ====================

int tampilkanMenu() {
    int opsi;
    cout << endl;
    cout << "Silahkan input opsi berikut dalam bentuk angka." << endl;
    cout << "1. Tambahkan Daftar Kendaraan" << endl;
    cout << "2. Tampilkan Daftar Kendaraan" << endl;
    cout << "3. Pembayaran" << endl;
    cout << "4. Keluar Program" << endl;
    cout << "Masukkan opsi : ";
    cin  >> opsi;
    cin.ignore();
    return opsi;
}

// ==================== OPSI 1: TAMBAH KENDARAAN ====================

void tambahKendaraan(vector<Kendaraan*>& daftar, int kapasitas) {
    if ((int)daftar.size() >= kapasitas) {
        cout << endl;
        cetakGaris('=');
        cout << "  MAAF, PARKIRAN PENUH! Kapasitas maksimal " << kapasitas << " kendaraan." << endl;
        cetakGaris('=');
        return;
    }

    int jumlah;
    cout << "Masukan jumlah mobil yang akan di parkirkan : ";
    cin  >> jumlah;
    cin.ignore();

    int sisa = kapasitas - (int)daftar.size();
    if (jumlah > sisa) {
        cout << "Kapasitas tersisa hanya " << sisa << ". Hanya " << sisa << " mobil yang dapat ditambahkan." << endl;
        jumlah = sisa;
    }

    for (int i = 0; i < jumlah; i++) {
        Kendaraan* k = new Kendaraan();
        cout << endl << "Mobil ke " << (i + 1) << endl;
        cout << "Masukkan plat mobil  : ";
        getline(cin, k->platMobil);
        cout << "Masukkan merk mobil  : ";
        getline(cin, k->merkMobil);
        cout << "Masukkan warna mobil : ";
        getline(cin, k->warnaMobil);
        k->waktuMasuk = time(nullptr);
        daftar.push_back(k);
    }
}

// ==================== OPSI 2: TAMPILKAN KENDARAAN ====================

void tampilkanKendaraan(const vector<Kendaraan*>& daftar) {
    if (daftar.empty()) {
        cout << endl << "Belum ada kendaraan yang terparkir." << endl;
        return;
    }

    cout << endl;
    cout << "DAFTAR MOBIL YANG SUDAH TERPARKIR :" << endl;
    cetakGaris();

    for (int i = 0; i < (int)daftar.size(); i++) {
        int jam, menit, detik;
        hitungWaktuParkir(daftar[i]->waktuMasuk, jam, menit, detik);

        cout << endl;
        cout << "Mobil ke-" << (i + 1) << endl;
        cout << "Plat mobil  : " << daftar[i]->platMobil  << endl;
        cout << "Merk mobil  : " << daftar[i]->merkMobil  << endl;
        cout << "Warna mobil : " << daftar[i]->warnaMobil << endl;
        cout << "Waktu parkir: " << jam << " jam " << menit << " menit " << detik << " detik" << endl;
        cetakGaris('=');
    }
}

// ==================== OPSI 3: PEMBAYARAN ====================

void pembayaran(vector<Kendaraan*>& daftar) {
    if (daftar.empty()) {
        cout << endl << "Tidak ada kendaraan yang terparkir." << endl;
        return;
    }

    // Tampilkan daftar terlebih dahulu
    tampilkanKendaraan(daftar);

    string platCari;
    cout << endl;
    cout << "Masukkan plat mobil yang ingin dibayar : ";
    getline(cin, platCari);

    // Ubah ke huruf besar untuk perbandingan
    string platUpper = platCari;
    transform(platUpper.begin(), platUpper.end(), platUpper.begin(), ::toupper);

    auto it = daftar.end();
    for (auto iter = daftar.begin(); iter != daftar.end(); ++iter) {
        string platKendaraan = (*iter)->platMobil;
        transform(platKendaraan.begin(), platKendaraan.end(), platKendaraan.begin(), ::toupper);
        if (platKendaraan == platUpper) {
            it = iter;
            break;
        }
    }

    if (it == daftar.end()) {
        cout << "PLAT TIDAK DITEMUKAN !" << endl;

        cout << endl;
        cout << "Masukkan plat mobil yang ingin dibayar : ";
        getline(cin, platCari);

        platUpper = platCari;
        transform(platUpper.begin(), platUpper.end(), platUpper.begin(), ::toupper);

        for (auto iter = daftar.begin(); iter != daftar.end(); ++iter) {
            string platKendaraan = (*iter)->platMobil;
            transform(platKendaraan.begin(), platKendaraan.end(), platKendaraan.begin(), ::toupper);
            if (platKendaraan == platUpper) {
                it = iter;
                break;
            }
        }

        if (it == daftar.end()) {
            cout << "PLAT TIDAK DITEMUKAN ! Kembali ke menu." << endl;
            return;
        }
    }

    long long biaya = hitungBiaya((*it)->waktuMasuk);
    cout << endl;
    cout << "B I A Y A   P A R K I R : Rp" << biaya << endl;
    cetakGaris();

    // Hapus kendaraan dari daftar
    delete *it;
    daftar.erase(it);
}

// ==================== FUNGSI BERSIHKAN MEMORI ====================

void bersihkanMemori(vector<Kendaraan*>& daftar, Petugas* petugas) {
    for (auto k : daftar) delete k;
    daftar.clear();
    delete petugas;
}

// ==================== MAIN ====================

int main() {
    const int KAPASITAS = 10;
    vector<Kendaraan*> daftarKendaraan;

    cetakJudul();

    Petugas* petugas = inputPetugas();
    tampilkanPetugas(petugas);

    bool running = true;
    while (running) {
        int opsi = tampilkanMenu();
        switch (opsi) {
            case 1:
                tambahKendaraan(daftarKendaraan, KAPASITAS);
                break;
            case 2:
                tampilkanKendaraan(daftarKendaraan);
                break;
            case 3:
                pembayaran(daftarKendaraan);
                break;
            case 4:
                cout << endl;
                cetakGaris();
                cout << "Terima kasih, " << petugas->nama << ". Sampai jumpa!" << endl;
                cetakGaris();
                running = false;
                break;
            default:
                cout << "Opsi tidak valid. Silahkan masukkan angka 1-4." << endl;
        }
    }

    bersihkanMemori(daftarKendaraan, petugas);
    return 0;
}