#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

struct Barang {
    string kode_barang;
    string nama_barang;
    double harga;
    int stok;
};

vector<Barang> daftar_barang;

void bersihin_buffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool kode(string kode) {
    for (int i = 0; i < daftar_barang.size(); i++) {
        if (daftar_barang[i].kode_barang == kode) return true;
    }
    return false;
}

void tambah_barang() {
    Barang b;
    cout << "\n--- TAMBAH BARANG ---" << endl;
    cout << "Kode barang : ";
    cin >> b.kode_barang;
    bersihin_buffer();

    while (kode(b.kode_barang)) {
        cout << "Kode sudah dipakai! Masukkan kode lain: ";
        cin >> b.kode_barang;
        bersihin_buffer();
    }

    cout << "Nama Barang : ";
    getline(cin, b.nama_barang);
    cout << "Harga : ";
    cin >> b.harga;
    bersihin_buffer();

    while (b.harga < 0) {
        cout << "Harga tidak boleh negatif. Silakan masukkan lagi :D" << endl;
        cout << "Harga: ";
        cin >> b.harga;
        bersihin_buffer();
    }

    cout << "Stok : ";
    cin >> b.stok;
    bersihin_buffer();

    while (b.stok < 0) {
        cout << "Stok tidak boleh negatif. Silakan masukkan lagi :D" << endl;
        cout << "Stok : ";
        cin >> b.stok; 
        bersihin_buffer();
    }

    daftar_barang.push_back(b);
    cout << "Barang berhasil ditambahkan!" << endl;
}

void tampilkansemuabarang() {
    cout << "\n--- DAFTAR BARANG ---" << endl;

    if (daftar_barang.size() == 0) {
        cout << "Belum ada barang." << endl;
        return;
    }

    for (int i = 0; i < daftar_barang.size(); i++) {
        cout << i + 1 << ". [" << daftar_barang[i].kode_barang << "] " << daftar_barang[i].nama_barang << endl;
        cout << "Harga : Rp" << daftar_barang[i].harga << endl; 
        cout << "Stok  : " << daftar_barang[i].stok << endl << endl;
    }
}

void caribarangtermahal() {
    cout << "\n--- BARANG TERMAHAL ---" << endl;

    if (daftar_barang.size() == 0) {
        cout << "Belum ada barang." << endl;
        return;
    }

    int indeks_termahal = 0;
    for (int i = 1; i < daftar_barang.size(); i++) {
        if (daftar_barang[i].harga > daftar_barang[indeks_termahal].harga) {
            indeks_termahal = i;
        }
    }

    cout << "Kode  : " << daftar_barang[indeks_termahal].kode_barang << endl;
    cout << "Nama  : " << daftar_barang[indeks_termahal].nama_barang << endl;
    cout << "Harga : Rp" << daftar_barang[indeks_termahal].harga << endl;
    cout << "Stok  : " << daftar_barang[indeks_termahal].stok << endl;
}

void hitungtotalnilai() {
    cout << "\n--- TOTAL NILAI INVENTARIS ---" << endl;

    if (daftar_barang.size() == 0) {
        cout << "Belum ada barang." << endl;
        return;
    }

    double total = 0;
    for (int i = 0; i < daftar_barang.size(); i++) {
        double nilai_barang = daftar_barang[i].harga * daftar_barang[i].stok;
        cout << daftar_barang[i].nama_barang
             << " : Rp" << daftar_barang[i].harga
             << " x " << daftar_barang[i].stok
             << " = Rp" << nilai_barang << endl;
        total = total + nilai_barang;
    }

    cout << "\nTotal nilai : Rp" << total << endl;
}

void cek_restock() {
    cout << "\n--- BARANG PERLU RESTOCK (stok < 5) ---" << endl;

    if (daftar_barang.size() == 0) {
        cout << "Belum ada barang." << endl;
        return;
    }

    bool kritis = false;
    for (int i = 0; i < daftar_barang.size(); i++) {
        if (daftar_barang[i].stok < 5) {
            cout << daftar_barang[i].nama_barang
                 << " (stok : " << daftar_barang[i].stok << ")" << endl;
            kritis = true;
        }
    }
    if (!kritis) cout << "Semua stok aman." << endl;
}

void update_stok() {
    cout << "\n--- UPDATE STOK ---" << endl;

    if (daftar_barang.size() == 0) {
        cout << "Belum ada barang." << endl;
        return;
    }

    string input_kode;
    cout << "Masukkan kode barang : ";
    cin >> input_kode;
    bersihin_buffer();

    int indeksditemukan = -1;
    for (int i = 0; i < daftar_barang.size(); i++) {
        if (daftar_barang[i].kode_barang == input_kode) {
            indeksditemukan = i; 
            break;
        }
    }

    if (indeksditemukan == -1) {
        cout << "Barang tidak ditemukan." << endl;
        return;
    }

    int* pstok = &daftar_barang[indeksditemukan].stok;
    cout << "Barang       : " << daftar_barang[indeksditemukan].nama_barang << endl;
    cout << "Stok sekarang: " << *pstok << endl;

    int pilihan;
    cout << "1. Tambah stok" << endl;
    cout << "2. Kurangi stok" << endl;
    cout << "Pilih: ";
    cin >> pilihan;
    bersihin_buffer();

    int jumlah;
    if (pilihan == 1) {
        cout << "Tambah berapa : ";
        cin >> jumlah;
        bersihin_buffer();
        while (jumlah <= 0) {
            cout << "Jumlah harus lebih dari 0" << endl;
            cout << "Tambah berapa : ";
            cin >> jumlah;
            bersihin_buffer();
        }
        *pstok += jumlah;
        cout << "Stok berhasil ditambah." << endl;
        cout << "Stok sekarang: " << *pstok << endl;

    } else if (pilihan == 2) {
        cout << "Kurangi berapa : ";
        cin >> jumlah;
        bersihin_buffer();
        while (jumlah <= 0 || jumlah > *pstok) {
            if (jumlah <= 0)
                cout << "Jumlah harus lebih dari 0" << endl;
            else
                cout << "Stok tidak cukup! Tersedia " << *pstok << "." << endl;
            cout << "Masukkan lagi : ";
            cin >> jumlah;
            bersihin_buffer();
        }
        *pstok -= jumlah;
        cout << "Stok berhasil dikurangi." << endl;
        cout << "Stok sekarang : " << *pstok << endl;

    } else {
        cout << "Pilihan tidak valid." << endl;
    }
} 

int main() {
    system ("cls");
    int pilihan;
    do {
        cout << "\n===========================================================" << endl;
        cout << "          SISTEM MANAJEMEN INVENTARIS TOKO                  " << endl;
        cout << "===========================================================" << endl;
        cout << "1. Tambah Barang" << endl;
        cout << "2. Tampilkan Semua Barang" << endl;
        cout << "3. Cari Barang Termahal" << endl;
        cout << "4. Hitung Total Nilai Inventaris" << endl;
        cout << "5. Cek Restock (Stok < 5)" << endl;
        cout << "6. Update Stok" << endl;
        cout << "7. Keluar" << endl;
        cout << "\nPilih menu : ";
        cin >> pilihan;
        bersihin_buffer();

        if (pilihan == 1) tambah_barang();
        else if (pilihan == 2) tampilkansemuabarang();
        else if (pilihan == 3) caribarangtermahal();
        else if (pilihan == 4) hitungtotalnilai();
        else if (pilihan == 5) cek_restock();
        else if (pilihan == 6) update_stok();
        else if (pilihan == 7) cout << "Keluar dari program." << endl;
        else cout << "Pilihan tidak valid." << endl;

    } while (pilihan != 7);

    return 0;
}