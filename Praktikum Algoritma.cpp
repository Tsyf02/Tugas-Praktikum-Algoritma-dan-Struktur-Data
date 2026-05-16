#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

// ── Nama file penyimpanan ────────────────────────────────────
const string FILE_DATA   = "inventory_data.txt";
const string FILE_LOG    = "inventory_log.txt";
const string FILE_EXPORT = "inventory_export.csv";

int     totalBarang = 0;
int     kapasitas   = 0;
int     nextId      = 1;

string* arrNama      = nullptr;
string* arrKategori  = nullptr;
string* arrLokasi    = nullptr;
string* arrKondisi   = nullptr;
string* arrTanggal   = nullptr;
string* arrCatatan   = nullptr;
int*    arrId        = nullptr;
int*    arrQty       = nullptr;
double* arrHarga     = nullptr;

void resizeArrays(int kapBaru) {
    string* tNama     = new string[kapBaru];
    string* tKategori = new string[kapBaru];
    string* tLokasi   = new string[kapBaru];
    string* tKondisi  = new string[kapBaru];
    string* tTanggal  = new string[kapBaru];
    string* tCatatan  = new string[kapBaru];
    int*    tId       = new int[kapBaru];
    int*    tQty      = new int[kapBaru];
    double* tHarga    = new double[kapBaru];

    int salin = min(totalBarang, kapBaru);
    for (int i = 0; i < salin; i++) {
        tNama[i]     = arrNama[i];
        tKategori[i] = arrKategori[i];
        tLokasi[i]   = arrLokasi[i];
        tKondisi[i]  = arrKondisi[i];
        tTanggal[i]  = arrTanggal[i];
        tCatatan[i]  = arrCatatan[i];
        tId[i]       = arrId[i];
        tQty[i]      = arrQty[i];
        tHarga[i]    = arrHarga[i];
    }

    delete[] arrNama;     delete[] arrKategori; delete[] arrLokasi;
    delete[] arrKondisi;  delete[] arrTanggal;  delete[] arrCatatan;
    delete[] arrId;       delete[] arrQty;      delete[] arrHarga;

    arrNama = tNama; arrKategori = tKategori; arrLokasi = tLokasi;
    arrKondisi = tKondisi; arrTanggal = tTanggal; arrCatatan = tCatatan;
    arrId = tId; arrQty = tQty; arrHarga = tHarga;
    kapasitas = kapBaru;
}

void pastikanKapasitas() {
    if (totalBarang >= kapasitas)
        resizeArrays(kapasitas == 0 ? 8 : kapasitas * 2);
}

void bebaskanMemori() {
    delete[] arrNama;    delete[] arrKategori; delete[] arrLokasi;
    delete[] arrKondisi; delete[] arrTanggal;  delete[] arrCatatan;
    delete[] arrId;      delete[] arrQty;      delete[] arrHarga;
}

void bersihkanLayar() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string timestamp() {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&now));
    return string(buf);
}

string inputString(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    size_t a = s.find_first_not_of(" \t");
    size_t b = s.find_last_not_of(" \t");
    if (a == string::npos) return "";
    return s.substr(a, b - a + 1);
}

int inputInt(const string& prompt, int minVal = 0) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= minVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Input tidak valid. Masukkan angka >= " << minVal << "\n";
    }
}

double inputDouble(const string& prompt) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Input tidak valid.\n";
    }
}

string toLower(string s) {
    for (char& c : s) c = (char)tolower(c);
    return s;
}

string rupiah(double val) {
    if (val == 0) return "-";
    long long v = (long long)val;
    string s = to_string(v);
    int pos = (int)s.length() - 3;
    while (pos > 0) { s.insert(pos, "."); pos -= 3; }
    return "Rp " + s;
}

string potong(const string& s, int n) {
    if ((int)s.length() <= n) return s;
    return s.substr(0, n - 2) + "..";
}

string escPipe(const string& s) {
    string r = s;
    for (char& c : r) if (c == '|') c = '\x01';
    return r;
}

string unescPipe(const string& s) {
    string r = s;
    for (char& c : r) if (c == '\x01') c = '|';
    return r;
}

void garis(char k = '-', int n = 64) {
    for (int i = 0; i < n; i++) cout << k;
    cout << "\n";
}

void cetakHeader() {
    cout << "\n";
    garis('=', 64);
    cout << "       WhereIsIt? Implement Personal Inventory Management System\n";
    cout << "           		   Kamar Kos / Pribadi\n";
    garis('=', 64);
}

void cetakMenu() {
    cout << "\n";
    garis('-', 42);
    cout << "  MENU UTAMA\n";
    garis('-', 42);
    cout << "  [ 1]  Tambah Barang\n";
    cout << "  [ 2]  Lihat Semua Barang\n";
    cout << "  [ 3]  Cari Barang\n";
    cout << "  [ 4]  Edit Barang\n";
    cout << "  [ 5]  Hapus Barang\n";
    cout << "  [ 6]  Filter per Kategori\n";
    cout << "  [ 7]  Statistik Inventaris\n";
    cout << "  [ 8]  Update Stok Cepat\n";
    cout << "  [ 9]  Detail Barang\n";
    cout << "  [10]  Urutkan Barang\n";
    cout << "  [11]  Export ke CSV\n";
    cout << "  [12]  Riwayat Aktivitas\n";
    cout << "  [13]  Barang Bermasalah\n";
    cout << "  [14]  Hapus Semua Data\n";
    cout << "  [ 0]  Keluar\n";
    garis('-', 42);
}

void headerTabel() {
    garis('-', 78);
    cout << left
         << setw(5)  << "ID"
         << setw(22) << "Nama Barang"
         << setw(14) << "Kategori"
         << setw(6)  << "Qty"
         << setw(16) << "Harga Satuan"
         << setw(15) << "Kondisi"
         << "\n";
    garis('-', 78);
}

void cetakBaris(int i) {
    cout << left
         << setw(5)  << arrId[i]
         << setw(22) << potong(arrNama[i], 21)
         << setw(14) << potong(arrKategori[i], 13)
         << setw(6)  << arrQty[i]
         << setw(16) << rupiah(arrHarga[i])
         << setw(15) << arrKondisi[i]
         << "\n";
}

void simpanData() {
    ofstream f(FILE_DATA);
    if (!f) { cout << "  Gagal menyimpan file!\n"; return; }
    f << nextId << "\n" << totalBarang << "\n";
    for (int i = 0; i < totalBarang; i++) {
        f << arrId[i]              << "|"
          << escPipe(arrNama[i])   << "|"
          << escPipe(arrKategori[i]) << "|"
          << arrQty[i]             << "|"
          << fixed << setprecision(2) << arrHarga[i] << "|"
          << escPipe(arrLokasi[i]) << "|"
          << escPipe(arrKondisi[i])<< "|"
          << escPipe(arrTanggal[i])<< "|"
          << escPipe(arrCatatan[i])<< "\n";
    }
}

void muatData() {
    ifstream f(FILE_DATA);
    if (!f) return;
    int total;
    f >> nextId >> total;
    f.ignore();
    resizeArrays(total + 8);
    string baris;
    while (getline(f, baris) && totalBarang < total) {
        if (baris.empty()) continue;
        istringstream ss(baris);
        string tok;
        getline(ss, tok, '|'); arrId[totalBarang]       = stoi(tok);
        getline(ss, tok, '|'); arrNama[totalBarang]      = unescPipe(tok);
        getline(ss, tok, '|'); arrKategori[totalBarang]  = unescPipe(tok);
        getline(ss, tok, '|'); arrQty[totalBarang]       = stoi(tok);
        getline(ss, tok, '|'); arrHarga[totalBarang]     = stod(tok);
        getline(ss, tok, '|'); arrLokasi[totalBarang]    = unescPipe(tok);
        getline(ss, tok, '|'); arrKondisi[totalBarang]   = unescPipe(tok);
        getline(ss, tok, '|'); arrTanggal[totalBarang]   = unescPipe(tok);
        getline(ss, tok);      arrCatatan[totalBarang]   = unescPipe(tok);
        totalBarang++;
    }
}

void tulisLog(const string& aksi) {
    ofstream lf(FILE_LOG, ios::app);
    lf << "[" << timestamp() << "] " << aksi << "\n";
}

int cariIndeks(int id) {
    for (int i = 0; i < totalBarang; i++)
        if (arrId[i] == id) return i;
    return -1;
}

// ── Fitur 1: Tambah Barang ───────────────────────────────────
void tambahBarang() {
    bersihkanLayar(); cetakHeader();
    cout << "\n  [ TAMBAH BARANG BARU ]\n\n";

    string nama = inputString("  Nama barang          : ");
    if (nama.empty()) { cout << "  Nama tidak boleh kosong.\n"; return; }

    pastikanKapasitas();
    int i = totalBarang;

    arrId[i]       = nextId++;
    arrNama[i]     = nama;
    arrKategori[i] = inputString("  Kategori              : ");
    arrQty[i]      = inputInt   ("  Jumlah                : ", 0);
    arrHarga[i]    = inputDouble("  Harga satuan (0=skip) : ");
    arrLokasi[i]   = inputString("  Lokasi di kamar       : ");

    cout << "  Kondisi [1]Bagus  [2]Rusak  [3]Perlu Perbaikan : ";
    int pil; cin >> pil; cin.ignore();
    if      (pil == 2) arrKondisi[i] = "Rusak";
    else if (pil == 3) arrKondisi[i] = "Perlu Perbaikan";
    else               arrKondisi[i] = "Bagus";

    arrCatatan[i] = inputString("  Catatan (opsional)    : ");
    arrTanggal[i] = timestamp();
    totalBarang++;

    simpanData();
    tulisLog("TAMBAH [" + to_string(arrId[i]) + "] " + arrNama[i]);
    cout << "\n  Berhasil ditambahkan! (ID: " << arrId[i] << ")\n";
}

// ── Fitur 2: Lihat Semua Barang ──────────────────────────────
void lihatSemuaBarang() {
    bersihkanLayar(); cetakHeader();
    cout << "\n  [ SEMUA BARANG ] - Total: " << totalBarang << " item\n\n";
    if (totalBarang == 0) { cout << "  (belum ada data)\n"; return; }
    headerTabel();
    for (int i = 0; i < totalBarang; i++) cetakBaris(i);
    garis('-', 78);
}

// ── Fitur 3: Cari Barang ─────────────────────────────────────
void cariBarang() {
    string kw = inputString("\n  Kata kunci : ");
    string kwL = toLower(kw);
    bersihkanLayar(); cetakHeader();
    cout << "\n  [ HASIL CARI: \"" << kw << "\" ]\n\n";
    headerTabel();
    int cnt = 0;
    for (int i = 0; i < totalBarang; i++) {
        string gabung = toLower(arrNama[i]+arrKategori[i]+arrLokasi[i]+arrCatatan[i]);
        if (gabung.find(kwL) != string::npos) { cetakBaris(i); cnt++; }
    }
    garis('-', 78);
    cout << "  Ditemukan: " << cnt << " barang\n";
}

// ── Fitur 4: Edit Barang ─────────────────────────────────────
void editBarang() {
    if (totalBarang == 0) { cout << "  Inventaris kosong.\n"; return; }
    int id = inputInt("\n  ID barang yang diedit : ", 1);
    int idx = cariIndeks(id);
    if (idx < 0) { cout << "  ID tidak ditemukan.\n"; return; }

    cout << "\n  (Tekan Enter untuk mempertahankan nilai lama)\n\n";

    auto tanya = [](const string& p, const string& lama) -> string {
        cout << "  " << p << " [" << lama << "]: ";
        string v; getline(cin, v);
        return v.empty() ? lama : v;
    };

    arrNama[idx]     = tanya("Nama    ", arrNama[idx]);
    arrKategori[idx] = tanya("Kategori", arrKategori[idx]);
    arrLokasi[idx]   = tanya("Lokasi  ", arrLokasi[idx]);
    arrCatatan[idx]  = tanya("Catatan ", arrCatatan[idx]);

    cout << "  Jumlah baru (Enter=skip, sekarang=" << arrQty[idx] << "): ";
    string qs; getline(cin, qs);
    if (!qs.empty()) arrQty[idx] = stoi(qs);

    cout << "  Kondisi [1]Bagus [2]Rusak [3]Perlu Perbaikan (Enter=skip): ";
    string cs; getline(cin, cs);
    if      (cs == "1") arrKondisi[idx] = "Bagus";
    else if (cs == "2") arrKondisi[idx] = "Rusak";
    else if (cs == "3") arrKondisi[idx] = "Perlu Perbaikan";

    simpanData();
    tulisLog("EDIT [" + to_string(id) + "] " + arrNama[idx]);
    cout << "\n  Barang berhasil diperbarui!\n";
}

// ── Fitur 5: Hapus Barang ────────────────────────────────────
//  Elemen dihapus dengan menggeser semua elemen di sebelah
//  kanannya satu posisi ke kiri, lalu totalBarang dikurangi.
void hapusBarang() {
    if (totalBarang == 0) { cout << "  Inventaris kosong.\n"; return; }
    int id = inputInt("\n  ID barang yang dihapus : ", 1);
    int idx = cariIndeks(id);
    if (idx < 0) { cout << "  ID tidak ditemukan.\n"; return; }

    cout << "  Hapus \"" << arrNama[idx] << "\"? [y/N]: ";
    char c; cin >> c; cin.ignore();
    if (c != 'y' && c != 'Y') { cout << "  Dibatalkan.\n"; return; }

    string nm = arrNama[idx];
    for (int i = idx; i < totalBarang - 1; i++) {
        arrId[i]       = arrId[i+1];
        arrNama[i]     = arrNama[i+1];
        arrKategori[i] = arrKategori[i+1];
        arrQty[i]      = arrQty[i+1];
        arrHarga[i]    = arrHarga[i+1];
        arrLokasi[i]   = arrLokasi[i+1];
        arrKondisi[i]  = arrKondisi[i+1];
        arrTanggal[i]  = arrTanggal[i+1];
        arrCatatan[i]  = arrCatatan[i+1];
    }
    totalBarang--;
    simpanData();
    tulisLog("HAPUS [" + to_string(id) + "] " + nm);
    cout << "\n  \"" << nm << "\" berhasil dihapus!\n";
}

// ── Fitur 6: Filter Kategori ─────────────────────────────────
void filterKategori() {
    if (totalBarang == 0) { cout << "  Inventaris kosong.\n"; return; }

    // Kumpulkan nama kategori unik ke array string sementara
    int     jmlKat = 0;
    string* arrKat = new string[totalBarang];

    for (int i = 0; i < totalBarang; i++) {
        bool ada = false;
        for (int j = 0; j < jmlKat; j++)
            if (arrKat[j] == arrKategori[i]) { ada = true; break; }
        if (!ada) arrKat[jmlKat++] = arrKategori[i];
    }

    cout << "\n  Daftar Kategori:\n";
    for (int i = 0; i < jmlKat; i++)
        cout << "  [" << (i+1) << "] " << arrKat[i] << "\n";

    int pil = inputInt("  Pilih : ", 1);
    if (pil < 1 || pil > jmlKat) { delete[] arrKat; return; }
    string pilKat = arrKat[pil-1];
    delete[] arrKat;

    bersihkanLayar(); cetakHeader();
    cout << "\n  [ KATEGORI: " << pilKat << " ]\n\n";
    headerTabel();
    int cnt = 0;
    for (int i = 0; i < totalBarang; i++)
        if (arrKategori[i] == pilKat) { cetakBaris(i); cnt++; }
    garis('-', 78);
    cout << "  Total: " << cnt << " barang\n";
}

// ── Fitur 7: Statistik ───────────────────────────────────────
void statistik() {
    bersihkanLayar(); cetakHeader();
    cout << "\n  [ STATISTIK INVENTARIS ]\n\n";
    if (totalBarang == 0) { cout << "  (belum ada data)\n"; return; }

    int    totalQty   = 0;
    double totalNilai = 0;
    int    cBagus = 0, cRusak = 0, cPerlu = 0;
    double hMax = 0;
    string nmMax = "-";

    for (int i = 0; i < totalBarang; i++) {
        totalQty   += arrQty[i];
        totalNilai += arrHarga[i] * arrQty[i];
        if      (arrKondisi[i] == "Bagus")           cBagus++;
        else if (arrKondisi[i] == "Rusak")           cRusak++;
        else                                          cPerlu++;
        if (arrHarga[i] > hMax) { hMax = arrHarga[i]; nmMax = arrNama[i]; }
    }

    garis('=', 50);
    cout << left << setw(30) << "  Total jenis barang"   << ": " << totalBarang << "\n";
    cout << setw(30)         << "  Total stok (qty)"     << ": " << totalQty    << "\n";
    cout << setw(30)         << "  Estimasi total nilai" << ": " << rupiah(totalNilai) << "\n";
    cout << setw(30)         << "  Barang termahal"      << ": " << nmMax << " (" << rupiah(hMax) << ")\n";
    garis('-', 50);
    cout << "  KONDISI:\n";
    cout << "    Bagus           : " << cBagus << " barang\n";
    cout << "    Perlu Perbaikan : " << cPerlu << " barang\n";
    cout << "    Rusak           : " << cRusak << " barang\n";
    garis('=', 50);
}

// ── Fitur 8: Update Stok Cepat ───────────────────────────────
void updateStok() {
    if (totalBarang == 0) { cout << "  Inventaris kosong.\n"; return; }
    int id = inputInt("\n  ID barang : ", 1);
    int idx = cariIndeks(id);
    if (idx < 0) { cout << "  ID tidak ditemukan.\n"; return; }

    cout << "  \"" << arrNama[idx] << "\" -- stok: " << arrQty[idx] << "\n";
    cout << "  [1]Tambah  [2]Kurangi  [3]Set : ";
    int op; cin >> op; cin.ignore();
    int val = inputInt("  Jumlah : ", 0);

    if      (op == 1) arrQty[idx] += val;
    else if (op == 2) arrQty[idx] = max(0, arrQty[idx] - val);
    else              arrQty[idx] = val;

    simpanData();
    tulisLog("STOK [" + to_string(id) + "] " + arrNama[idx] + " -> " + to_string(arrQty[idx]));
    cout << "\n  Stok diperbarui: " << arrQty[idx] << "\n";
}

// ── Fitur 9: Detail Barang ───────────────────────────────────
void detailBarang() {
    int id = inputInt("\n  ID barang : ", 1);
    int idx = cariIndeks(id);
    if (idx < 0) { cout << "  ID tidak ditemukan.\n"; return; }

    garis('=', 50);
    cout << "  Detail Barang #" << arrId[idx] << "\n";
    garis('-', 50);
    auto r = [](const string& k, const string& v) {
        cout << "  " << left << setw(18) << k << ": " << v << "\n";
    };
    r("Nama",           arrNama[idx]);
    r("Kategori",       arrKategori[idx]);
    r("Jumlah",         to_string(arrQty[idx]));
    r("Harga Satuan",   rupiah(arrHarga[idx]));
    r("Total Nilai",    rupiah(arrHarga[idx] * arrQty[idx]));
    r("Lokasi",         arrLokasi[idx]);
    r("Kondisi",        arrKondisi[idx]);
    r("Tanggal Tambah", arrTanggal[idx]);
    r("Catatan",        arrCatatan[idx].empty() ? "-" : arrCatatan[idx]);
    garis('=', 50);
}

// ── Fitur 10: Urutkan (Bubble Sort pada array paralel) ───────
//  Bubble sort diterapkan langsung pada semua array secara
//  bersamaan, sehingga data setiap barang tetap konsisten
//  meskipun urutan indeks berubah setelah pengurutan.
void urutkanBarang() {
    cout << "\n  Urutkan berdasarkan:\n"
         << "  [1] Nama (A-Z)\n"
         << "  [2] Jumlah (terbesar)\n"
         << "  [3] Harga (terbesar)\n";
    int pil = inputInt("  Pilihan : ", 1);

    for (int i = 0; i < totalBarang - 1; i++) {
        for (int j = 0; j < totalBarang - i - 1; j++) {
            bool tukar = false;
            if      (pil == 1) tukar = toLower(arrNama[j]) > toLower(arrNama[j+1]);
            else if (pil == 2) tukar = arrQty[j]   < arrQty[j+1];
            else               tukar = arrHarga[j]  < arrHarga[j+1];

            if (tukar) {
                swap(arrId[j],       arrId[j+1]);
                swap(arrNama[j],     arrNama[j+1]);
                swap(arrKategori[j], arrKategori[j+1]);
                swap(arrQty[j],      arrQty[j+1]);
                swap(arrHarga[j],    arrHarga[j+1]);
                swap(arrLokasi[j],   arrLokasi[j+1]);
                swap(arrKondisi[j],  arrKondisi[j+1]);
                swap(arrTanggal[j],  arrTanggal[j+1]);
                swap(arrCatatan[j],  arrCatatan[j+1]);
            }
        }
    }
    simpanData();
    lihatSemuaBarang();
}

// ── Fitur 11: Export CSV ─────────────────────────────────────
void exportCSV() {
    ofstream f(FILE_EXPORT);
    if (!f) { cout << "  Gagal membuat file CSV!\n"; return; }
    f << "ID,Nama,Kategori,Qty,Harga Satuan,Total Nilai,Lokasi,Kondisi,Tanggal,Catatan\n";
    for (int i = 0; i < totalBarang; i++) {
        f << arrId[i] << ","
          << "\"" << arrNama[i]     << "\","
          << "\"" << arrKategori[i] << "\","
          << arrQty[i]   << ","
          << arrHarga[i] << ","
          << (arrHarga[i] * arrQty[i]) << ","
          << "\"" << arrLokasi[i]   << "\","
          << "\"" << arrKondisi[i]  << "\","
          << "\"" << arrTanggal[i]  << "\","
          << "\"" << arrCatatan[i]  << "\"\n";
    }
    tulisLog("EXPORT CSV: " + FILE_EXPORT);
    cout << "\n  Data diekspor ke \"" << FILE_EXPORT << "\"\n";
}

// ── Fitur 12: Riwayat Log ────────────────────────────────────
void riwayatLog() {
    ifstream lf(FILE_LOG);
    if (!lf) { cout << "\n  (belum ada log)\n"; return; }
    bersihkanLayar(); cetakHeader();
    cout << "\n  [ RIWAYAT AKTIVITAS (30 terakhir) ]\n\n";
    garis('-', 64);

    // Gunakan array string sementara untuk buffer 30 baris terakhir
    string buffer[30];
    int cnt = 0, totalLog = 0;
    string baris;
    while (getline(lf, baris)) {
        buffer[cnt % 30] = baris;
        cnt++; totalLog++;
    }
    int start  = (totalLog >= 30) ? cnt % 30 : 0;
    int tampil = min(totalLog, 30);
    for (int i = 0; i < tampil; i++)
        cout << "  " << buffer[(start + i) % 30] << "\n";
    garis('-', 64);
}

// ── Fitur 13: Barang Bermasalah ──────────────────────────────
void barangBermasalah() {
    bersihkanLayar(); cetakHeader();
    cout << "\n  [ BARANG BERMASALAH ]\n\n";
    headerTabel();
    int cnt = 0;
    for (int i = 0; i < totalBarang; i++)
        if (arrKondisi[i] != "Bagus") { cetakBaris(i); cnt++; }
    garis('-', 78);
    if (cnt == 0) cout << "  Semua barang dalam kondisi baik!\n";
    else          cout << "  Total bermasalah: " << cnt << " barang\n";
}

// ── Fitur 14: Reset Semua Data ───────────────────────────────
void resetSemua() {
    cout << "\n  HAPUS SEMUA DATA? Tidak bisa diurungkan!\n";
    cout << "  Ketik HAPUS untuk konfirmasi: ";
    string konfirmasi; getline(cin, konfirmasi);
    if (konfirmasi != "HAPUS") { cout << "  Dibatalkan.\n"; return; }
    totalBarang = 0;
    nextId      = 1;
    simpanData();
    tulisLog("RESET ALL");
    cout << "  Semua data telah dihapus.\n";
}

// ── Main ─────────────────────────────────────────────────────
int main() {
    muatData();
    tulisLog("PROGRAM MULAI");

    while (true) {
        bersihkanLayar();
        cetakHeader();
        cout << "  File: " << FILE_DATA
             << "  |  Total: " << totalBarang << " barang\n";
        cetakMenu();

        int pilihan = inputInt("  Pilih menu : ", 0);

        switch (pilihan) {
            case  1: tambahBarang();     break;
            case  2: lihatSemuaBarang(); break;
            case  3: cariBarang();       break;
            case  4: editBarang();       break;
            case  5: hapusBarang();      break;
            case  6: filterKategori();   break;
            case  7: statistik();        break;
            case  8: updateStok();       break;
            case  9: detailBarang();     break;
            case 10: urutkanBarang();    break;
            case 11: exportCSV();        break;
            case 12: riwayatLog();       break;
            case 13: barangBermasalah(); break;
            case 14: resetSemua();       break;
            case  0:
                tulisLog("PROGRAM SELESAI");
                bebaskanMemori();
                cout << "\n  Sampai jumpa! Data tersimpan di " << FILE_DATA << "\n\n";
                return 0;
            default:
                cout << "\n  Pilihan tidak valid.\n";
        }

        cout << "\n  [ Tekan Enter untuk kembali ke menu... ]";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
