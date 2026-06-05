/*
 * ============================================================
 *  MINESWEEPER++
 *  Project 4 - Console-Based Minesweeper
 * ============================================================
 */
 
#include <iostream>
#include <cstdlib>
#include <ctime>
 
// ============================================================
//  Class Minesweeper
//  Mengelola seluruh state dan logika permainan
// ============================================================
class Minesweeper {
private:
    static const int MAX_SIZE = 10;
 
    // --- State papan ---
    int  board[MAX_SIZE][MAX_SIZE];   // nilai: -1 = bom, 0-8 = jumlah bom sekitar
    bool opened[MAX_SIZE][MAX_SIZE];  // sudah dibuka?
    bool flagged[MAX_SIZE][MAX_SIZE]; // sudah ditandai?
 
    int  N;           // ukuran papan N x N
    int  totalBombs;  // total bom
    int  flagCount;   // berapa tanda sudah dipasang
    int  openCount;   // berapa kotak aman sudah dibuka
 
    bool gameOver;
    bool gameWon;
 
    time_t startTime;
 
    // --- Helpers internal ---
 
    // Tanam bom secara acak
    void plantBombs() {
        int placed = 0;
        while (placed < totalBombs) {
            int r = rand() % N;
            int c = rand() % N;
            if (board[r][c] != -1) {
                board[r][c] = -1;
                ++placed;
            }
        }
    }
 
    // Hitung angka di setiap kotak bukan bom
    void calculateNumbers() {
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                if (board[r][c] == -1) continue;
                int count = 0;
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        if (dr == 0 && dc == 0) continue;
                        int nr = r + dr, nc = c + dc;
                        if (nr >= 0 && nr < N && nc >= 0 && nc < N && board[nr][nc] == -1)
                            ++count;
                    }
                }
                board[r][c] = count;
            }
        }
    }
 
    // Flood fill rekursif untuk kotak bernilai 0
    void floodFill(int r, int c) {
        if (r < 0 || r >= N || c < 0 || c >= N) return;
        if (opened[r][c]) return;
        if (board[r][c] == -1) return;
 
        opened[r][c] = true;
        ++openCount;
 
        if (board[r][c] == 0) {
            for (int dr = -1; dr <= 1; ++dr)
                for (int dc = -1; dc <= 1; ++dc)
                    if (!(dr == 0 && dc == 0))
                        floodFill(r + dr, c + dc);
        }
    }
 
    // Inisialisasi ulang semua array
    void resetArrays() {
        for (int r = 0; r < MAX_SIZE; ++r)
            for (int c = 0; c < MAX_SIZE; ++c) {
                board[r][c]   = 0;
                opened[r][c]  = false;
                flagged[r][c] = false;
            }
    }
 
    // Cek kondisi menang:
    // Menang jika SEMUA bom ditandai DAN tidak ada tanda di kotak aman
    bool checkWin() {
        for (int r = 0; r < N; ++r)
            for (int c = 0; c < N; ++c) {
                if (board[r][c] == -1 && !flagged[r][c]) return false; // bom belum ditandai
                if (board[r][c] != -1 && flagged[r][c]) return false;  // tanda salah
            }
        return true;
    }
 
    // Buka semua bom saat game over
    void revealBombs() {
        for (int r = 0; r < N; ++r)
            for (int c = 0; c < N; ++c)
                if (board[r][c] == -1)
                    opened[r][c] = true;
    }
 
    // Tampilkan satu sel
    void printCell(int r, int c, bool revealAll) const {
        if (revealAll && board[r][c] == -1) {
            if (flagged[r][c])
                std::cout << " F "; // bom + ditandai (benar)
            else
                std::cout << " * "; // bom meledak / terungkap
            return;
        }
        if (flagged[r][c]) { std::cout << " F "; return; }
        if (!opened[r][c]) { std::cout << " . "; return; }
        if (board[r][c] == 0) { std::cout << "   "; return; }
        std::cout << " " << board[r][c] << " ";
    }
 
    // Hitung elapsed time
    int elapsedSeconds() const {
        return static_cast<int>(time(nullptr) - startTime);
    }
 
public:
    // ---- Constructor ----
    Minesweeper() {
        N          = 0;
        totalBombs = 0;
        flagCount  = 0;
        openCount  = 0;
        gameOver   = false;
        gameWon    = false;
        startTime  = 0;
        resetArrays();
    }
 
    // ---- Setup permainan baru ----
    void setup() {
        std::cout << "\n========================================\n";
        std::cout << "        MINESWEEPER++ - PENGATURAN\n";
        std::cout << "========================================\n";
 
        // Pilih ukuran
        do {
            std::cout << "Masukkan ukuran papan N (4 - 10): ";
            std::cin >> N;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                N = 0;
            }
            if (N < 4 || N > 10)
                std::cout << "[!] Ukuran harus antara 4 dan 10.\n";
        } while (N < 4 || N > 10);
 
        // Pilih jumlah bom
        int maxBombs = N * N - 1;
        do {
            std::cout << "Masukkan jumlah bom (1 - " << maxBombs << "): ";
            std::cin >> totalBombs;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                totalBombs = 0;
            }
            if (totalBombs < 1 || totalBombs > maxBombs)
                std::cout << "[!] Jumlah bom harus antara 1 dan " << maxBombs << ".\n";
        } while (totalBombs < 1 || totalBombs > maxBombs);
 
        // Reset state
        resetArrays();
        flagCount = 0;
        openCount = 0;
        gameOver  = false;
        gameWon   = false;
 
        // Tanam bom & hitung angka
        plantBombs();
        calculateNumbers();
 
        startTime = time(nullptr);
 
        std::cout << "\nPermainan dimulai! Semoga beruntung!\n";
    }
 
    // ---- Tampilkan papan ----
    void display(bool revealAll = false) const {
        int elapsed = static_cast<int>(time(nullptr) - startTime);
        int remaining = totalBombs - flagCount;
 
        std::cout << "\n";
        std::cout << "  Bom tersisa: " << remaining
                  << "   |   Waktu: " << elapsed << " detik\n";
 
        // Header kolom
        std::cout << "    ";
        for (int c = 0; c < N; ++c)
            std::cout << " " << (c + 1) << "  ";
        std::cout << "\n";
 
        // Garis atas
        std::cout << "    ";
        for (int c = 0; c < N; ++c)
            std::cout << "----";
        std::cout << "-\n";
 
        for (int r = 0; r < N; ++r) {
            // Nomor baris (1..N)
            if (r + 1 < 10)
                std::cout << " " << (r + 1) << " |";
            else
                std::cout << (r + 1) << " |";
 
            for (int c = 0; c < N; ++c) {
                printCell(r, c, revealAll);
                std::cout << "|";
            }
            std::cout << "\n";
 
            // Garis pemisah
            std::cout << "    ";
            for (int c = 0; c < N; ++c)
                std::cout << "----";
            std::cout << "-\n";
        }
 
        std::cout << "\n  Legenda: [.] Tertutup  [F] Ditandai  [ ] Kosong  [1-8] Angka  [*] Bom\n";
    }
 
    // ---- Proses aksi pemain ----
    void playerAction() {
        std::cout << "\nPilih aksi:\n";
        std::cout << "  1. Buka kotak\n";
        std::cout << "  2. Tandai / Hapus tanda kotak\n";
        std::cout << "Pilihan: ";
 
        int action;
        std::cin >> action;
        if (std::cin.fail() || (action != 1 && action != 2)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "[!] Pilihan tidak valid.\n";
            return;
        }
 
        int row, col;
        std::cout << "Masukkan baris (1-" << N << "): ";
        std::cin >> row;
        std::cout << "Masukkan kolom (1-" << N << "): ";
        std::cin >> col;
 
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "[!] Input tidak valid.\n";
            return;
        }
 
        if (row < 1 || row > N || col < 1 || col > N) {
            std::cout << "[!] Koordinat di luar papan.\n";
            return;
        }
 
        int r = row - 1, c = col - 1;
 
        if (action == 1) { // --- BUKA ---
            if (opened[r][c]) {
                std::cout << "[!] Kotak sudah terbuka.\n";
                return;
            }
            if (flagged[r][c]) {
                std::cout << "[!] Kotak ditandai. Hapus tanda dulu jika ingin membuka.\n";
                return;
            }
 
            if (board[r][c] == -1) {
                // GAME OVER
                opened[r][c] = true;
                revealBombs();
                gameOver = true;
                return;
            }
 
            floodFill(r, c);
 
        } else { // --- TANDAI / HAPUS TANDA ---
            if (opened[r][c]) {
                std::cout << "[!] Kotak sudah terbuka, tidak bisa ditandai.\n";
                return;
            }
            if (flagged[r][c]) {
                flagged[r][c] = false;
                --flagCount;
                std::cout << "[-] Tanda dihapus dari (" << row << "," << col << ").\n";
            } else {
                flagged[r][c] = true;
                ++flagCount;
                std::cout << "[+] Tanda dipasang di (" << row << "," << col << ").\n";
            }
 
            // Cek menang setelah menandai
            if (checkWin()) {
                gameWon = true;
            }
        }
    }
 
    // ---- Loop utama permainan ----
    void play() {
        setup();
 
        while (!gameOver && !gameWon) {
            display(false);
            playerAction();
        }
 
        // Tampilkan hasil akhir
        int elapsed = elapsedSeconds();
        display(true);
 
        if (gameOver) {
            std::cout << "\n";
            std::cout << "  +---------------------------------+\n";
            std::cout << "  |   BOOM! Kamu terkena bom!      |\n";
            std::cout << "  |   Game Over!                   |\n";
            std::cout << "  |   Waktu: " << elapsed << " detik";
            // Padding agar kotak rata
            int pad = 20 - (elapsed > 9 ? 2 : 1);
            for (int i = 0; i < pad; ++i) std::cout << " ";
            std::cout << "|\n";
            std::cout << "  +---------------------------------+\n";
        } else {
            std::cout << "\n";
            std::cout << "  +---------------------------------------+\n";
            std::cout << "  |   Selamat! Kamu berhasil!             |\n";
            std::cout << "  |   Semua bom berhasil ditandai!        |\n";
            std::cout << "  |   Waktu: " << elapsed << " detik";
            int pad = 27 - (elapsed > 9 ? 2 : 1);
            for (int i = 0; i < pad; ++i) std::cout << " ";
            std::cout << "|\n";
            std::cout << "  +---------------------------------------+\n";
        }
    }
};
 
// ============================================================
//  Main - Menu Utama
// ============================================================
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
 
    std::cout << "\n";
    std::cout << "  ************************************\n";
    std::cout << "  *         M I N E S W E E P E R   *\n";
    std::cout << "  *               + +                *\n";
    std::cout << "  ************************************\n";
 
    int choice = 0;
    while (true) {
        std::cout << "\n========== MENU UTAMA ==========\n";
        std::cout << "  1. Mulai Permainan Baru\n";
        std::cout << "  2. Keluar\n";
        std::cout << "Pilihan: ";
 
        std::cin >> choice;
 
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "[!] Input tidak valid.\n";
            continue;
        }
 
        if (choice == 1) {
            Minesweeper game;
            game.play();
        } else if (choice == 2) {
            std::cout << "\nTerima kasih sudah bermain Minesweeper++. Sampai jumpa!\n\n";
            break;
        } else {
            std::cout << "[!] Pilihan tidak valid. Masukkan 1 atau 2.\n";
        }
    }
 
    return 0;
}