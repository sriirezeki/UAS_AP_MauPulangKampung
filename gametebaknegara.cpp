#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cctype>

using namespace std;

vector<string> daftarNegara = {
    "indonesia", "malaysia", "singapura", "thailand", "filipina",
    "vietnam", "myanmar", "kamboja", "laos", "brunei", "timor leste",
    "china", "jepang", "korea selatan", "korea utara", "mongolia", "taiwan",
    "india", "pakistan", "bangladesh", "nepal", "srilanka",
    "afghanistan", "bhutan", "maladewa",
    "kazakhstan", "uzbekistan", "turkmenistan", "kirgistan", "tajikistan",
    "arab saudi", "iran", "irak", "turki", "israel",
    "yordania", "suriah", "lebanon", "yaman", "oman",
    "uni emirat arab", "kuwait", "qatar", "bahrain",
    "azerbaijan", "georgia", "armenia", "siprus",
    "inggris", "prancis", "jerman", "italia", "spanyol",
    "portugal", "belanda", "belgia", "swiss", "austria",
    "swedia", "norwegia", "denmark", "finlandia", "islandia",
    "irlandia", "estonia", "latvia", "lituania", "luksemburg",
    "yunani", "kroasia", "serbia", "albania", "slovenia",
    "makedonia utara", "bosnia herzegovina", "montenegro",
    "rusia", "ukraina", "polandia", "ceko", "slowakia",
    "hungaria", "rumania", "bulgaria", "moldova", "belarus",
    "monako", "liechtenstein", "andorra", "san marino", "vatikan",
    "mesir", "libya", "tunisia", "aljazair", "maroko",
    "sudan", "mauritania",
    "nigeria", "ghana", "senegal", "mali", "burkina faso",
    "pantai gading", "guinea", "guinea bissau", "gambia", "sierra leone",
    "liberia", "togo", "benin", "niger", "tanjung verde", "mauritius",
    "kamerun", "chad", "republik afrika tengah", "gabon",
    "republik kongo", "republik demokratik kongo", "guinea khatulistiwa", "sao tome dan principe",
    "ethiopia", "kenya", "tanzania", "uganda", "rwanda",
    "burundi", "somalia", "djibouti", "eritrea", "sudan selatan",
    "mozambik", "madagaskar", "komoro", "seychelles",
    "afrika selatan", "zimbabwe", "zambia", "angola", "namibia",
    "botswana", "lesotho", "eswatini", "malawi",
    "amerika serikat", "kanada", "meksiko", "kuba", "jamaika",
    "haiti", "republik dominika", "panama", "kosta rika", "guatemala",
    "honduras", "nikaragua", "el salvador", "belize",
    "trinidad dan tobago", "barbados", "bahama", "grenada",
    "saint kitts dan nevis", "saint lucia",
    "antigua dan barbuda", "dominika", "saint vincent dan grenadines", "kepulauan cook",
    "brasil", "argentina", "chile", "kolombia", "peru",
    "venezuela", "ekuador", "bolivia", "paraguay", "uruguay",
    "guyana", "suriname",
    "australia", "selandia baru", "papua nugini", "fiji",
    "kepulauan solomon", "vanuatu", "samoa", "kiribati",
    "tonga", "mikronesia", "palau", "nauru", "tuvalu", "kepulauan marshall"
};

// Fungsi: Sembunyikan huruf secara random
string sembunyikanHuruf(const string& negara) {
    string hasil = negara;
    int panjang = negara.size();

    vector<int> indeksHuruf;
    for (int i = 0; i < panjang; i++) {
        if (negara[i] != ' ') {
            indeksHuruf.push_back(i);
        }
    }

    int totalHuruf = indeksHuruf.size();
    if (totalHuruf == 0) return hasil;

    int maxSembunyi = max(1, totalHuruf / 2);
    int jumlahSembunyi = (rand() % maxSembunyi) + 1;

    // Fisher-Yates shuffle
    for (int i = totalHuruf - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(indeksHuruf[i], indeksHuruf[j]);
    }

    for (int i = 0; i < jumlahSembunyi; i++) {
        hasil[indeksHuruf[i]] = '_';
    }

    return hasil;
}

// Fungsi: Ubah string ke huruf kecil semua
string toLowerStr(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// MAIN PROGRAM
int main() {
    srand(time(0));

    int skor = 0;
    char pilihan;

    cout << "============================================" << endl;
    cout << "   Selamat Datang di Game Tebak Negara!    " << endl;
    cout << "============================================" << endl;
    cout << "Total negara: " << daftarNegara.size() << " negara" << endl;
    cout << "Tebak nama negara dari huruf yang tersedia!" << endl;
    cout << "============================================" << endl << endl;

    do {
        // Pilih negara secara random
        int indexNegara = rand() % daftarNegara.size();
        string negara   = daftarNegara[indexNegara];
        string soal     = sembunyikanHuruf(negara);

        cout << "Tebak negara: " << soal << endl;
        cout << "Negara apakah yang dimaksud? ";

        string jawaban;
        getline(cin, jawaban);

        if (toLowerStr(jawaban) == toLowerStr(negara)) {
            cout << "Selamat! Anda benar." << endl;
            skor++;
        } else {
            cout << "Maaf, jawaban Anda salah. Coba lagi." << endl;
        }

        cout << "Skor Anda: " << skor << endl;
        cout << "Ingin bermain lagi? (y/n): ";
        cin >> pilihan;
        cin.ignore();
        cout << endl;

    } while (pilihan == 'y' || pilihan == 'Y');

    cout << "Game Selesai :D" << endl;
    cout << "Skor Akhir Anda: " << skor << endl;

    return 0;
}