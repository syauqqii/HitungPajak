/*
 * ◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆
 *   HITUNG PAJAK  ·  v2.0.0  ·  2026
 *   Kalkulator Pajak Penghasilan (PPh Pasal 21)
 *   Cross-platform · C11 · MIT License
 * ◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>

#ifdef _WIN32
#  include <windows.h>
#  define CLEAR_CMD "cls"
   static void platform_init(void) {
       SetConsoleOutputCP(CP_UTF8);
       HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
       DWORD  m = 0;
       if (GetConsoleMode(h, &m))
           SetConsoleMode(h, m | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
       SetConsoleTitleA("Hitung Pajak v2.0");
   }
#else
#  include <unistd.h>
#  define CLEAR_CMD "clear"
   static void platform_init(void) {}
#endif

/* ── ANSI Colors ──────────────────────────────────────── */
#define RST  "\033[0m"
#define BLD  "\033[1m"
#define DIM  "\033[2m"
#define CYN  "\033[36m"
#define YLW  "\033[33m"
#define GRN  "\033[32m"
#define RED  "\033[31m"
#define WHT  "\033[97m"

/* ── Tax Constants (IDR) ──────────────────────────────── */
#define PTKP_BASE        54000000LL   /* TK/0              */
#define PTKP_MARRIED      4500000LL   /* Tambahan menikah  */
#define PTKP_DEPENDENT    4500000LL   /* Per tanggungan    */
#define PTKP_SPOUSE      54000000LL   /* Penghasilan isteri digabung */
#define INCOME_MAX   999999999999LL   /* ~1 Triliun IDR    */

/* Progressive tax bracket table (PMK-101/PMK.010/2016) */
static const long long LIMIT[5] = {
    60000000LL,
    250000000LL,
    500000000LL,
    5000000000LL,
    LLONG_MAX
};
static const double RATE[5] = { 0.05, 0.15, 0.25, 0.30, 0.35 };

/* ── Types ────────────────────────────────────────────── */
typedef enum { TK = 0, K, KI } Status;

typedef struct {
    Status    status;
    int       dep;        /* tanggungan 0–3 */
    long long income;
    long long ptkp;
    long long pkp;
    long long tax;
    long long netto;
} TaxResult;

/* ── Screen ───────────────────────────────────────────── */
static void clrscr(void) {
    int r = system(CLEAR_CMD);
    (void)r;
}

/* ── Number Formatting ────────────────────────────────── */
static void fmt_idr(char *buf, size_t sz, long long v) {
    char num[32];
    snprintf(num, sizeof(num), "%lld", v < 0 ? -v : v);
    size_t nlen = strlen(num);

    char out[64];
    size_t oi = 0;
    if (v < 0) out[oi++] = '-';
    out[oi++] = 'R'; out[oi++] = 'p'; out[oi++] = ' ';

    for (size_t i = 0; i < nlen; i++) {
        if (i > 0 && (nlen - i) % 3 == 0) out[oi++] = '.';
        out[oi++] = num[i];
    }
    out[oi] = '\0';
    snprintf(buf, sz, "%s", out);
}

/* ── Input Helpers ────────────────────────────────────── */
static void flush_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void read_line(char *buf, size_t sz) {
    if (!fgets(buf, (int)sz, stdin)) { printf("\n"); exit(0); }
    size_t len = strlen(buf);
    if (len && buf[len - 1] == '\n') buf[--len] = '\0';
}

static bool parse_ll(const char *s, long long *out, long long lo, long long hi) {
    if (!s || !*s) return false;
    char *end;
    errno = 0;
    long long v = strtoll(s, &end, 10);
    if (errno || end == s || *end) return false;
    if (v < lo || v > hi)         return false;
    *out = v;
    return true;
}

static void ask_ll(long long *out, const char *prompt, long long lo, long long hi) {
    char buf[64];
    while (true) {
        printf("  " CYN "▷ " RST "%s", prompt);
        read_line(buf, sizeof(buf));
        if (parse_ll(buf, out, lo, hi)) return;
        printf("  " RED "× " RST "Input tidak valid, coba lagi.\n");
    }
}

static void ask_int(int *out, const char *prompt, int lo, int hi) {
    long long v;
    ask_ll(&v, prompt, (long long)lo, (long long)hi);
    *out = (int)v;
}

/* ── UI Components ────────────────────────────────────── */
static void print_header(void) {
    printf("\n");
    printf(CYN BLD
        "  ◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆\n"
        "  ║" RST "                                                  " CYN BLD "║\n"
        "  ║" RST "    " BLD WHT "▲  H I T U N G   P A J A K" RST
            "                    " CYN BLD "║\n"
        "  ║" RST "    " DIM WHT "Indonesian Income Tax Calculator" RST
            "               " CYN BLD "║\n"
        "  ║" RST "    " DIM CYN "v2.0.0 · 2026" RST
            "                                  " CYN BLD "║\n"
        "  ║" RST "                                                  " CYN BLD "║\n"
        "  ◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆\n" RST
    );
    printf("\n");
}

static void print_section(const char *title) {
    printf(CYN BLD "  ◆ " BLD WHT "%s" RST "\n", title);
    printf(DIM CYN "  ├──────────────────────────────────────────────────\n" RST);
}

static void print_row(const char *label, const char *val, const char *col) {
    printf("  │  " DIM WHT "%-30s" RST "%s%s" RST "\n", label, col, val);
}

static void print_div(void) {
    printf(DIM CYN "  ├──────────────────────────────────────────────────\n" RST);
}

static void print_close(void) {
    printf(DIM CYN "  └──────────────────────────────────────────────────\n" RST);
}

/* ── Tax Logic ────────────────────────────────────────── */
static long long calc_ptkp(Status s, int dep) {
    long long p = PTKP_BASE;
    if (s == K || s == KI) p += PTKP_MARRIED;
    if (s == KI)            p += PTKP_SPOUSE;
    p += dep * PTKP_DEPENDENT;
    return p;
}

static long long calc_tax(long long pkp) {
    if (pkp <= 0) return 0;
    long long tax = 0, prev = 0;
    for (int i = 0; i < 5; i++) {
        long long cap   = (LIMIT[i] < pkp) ? LIMIT[i] : pkp;
        long long slice = cap - prev;
        if (slice <= 0) break;
        tax += (long long)(slice * RATE[i]);
        prev = LIMIT[i];
        if (pkp <= LIMIT[i]) break;
    }
    return tax;
}

/* ── Result Display ───────────────────────────────────── */
static void show_result(const TaxResult *r) {
    static const char *STATUS_LABEL[] = {
        "Tidak Kawin (TK)",
        "Kawin (K)",
        "Kawin + Isteri Bekerja (K/I)"
    };
    static const char *RATE_LABEL[] = {
        " 5%  (0 – Rp 60 jt)",
        "15%  (Rp 60 – 250 jt)",
        "25%  (Rp 250 – 500 jt)",
        "30%  (Rp 500 jt – 5 M)",
        "35%  (> Rp 5 M)"
    };

    char b1[32], b2[32], b3[32], b4[32], b5[32];
    fmt_idr(b1, sizeof(b1), r->income);
    fmt_idr(b2, sizeof(b2), r->ptkp);
    fmt_idr(b3, sizeof(b3), r->pkp > 0 ? r->pkp : 0);
    fmt_idr(b4, sizeof(b4), r->tax);
    fmt_idr(b5, sizeof(b5), r->netto);

    clrscr();
    print_header();

    /* ── Taxpayer Info ── */
    print_section("WAJIB PAJAK");
    {
        char dep_s[16];
        snprintf(dep_s, sizeof(dep_s), "%d orang", r->dep);
        print_row("Status Pernikahan",  STATUS_LABEL[r->status], CYN);
        print_row("Jumlah Tanggungan",  dep_s, CYN);
    }
    print_close();

    printf("\n");

    /* ── Tax Breakdown ── */
    print_section("PERHITUNGAN PPh PASAL 21");
    print_row("Penghasilan Bruto",   b1, WHT);
    print_row("PTKP (Pengurangan)", b2, WHT);
    print_div();
    print_row("PKP (Kena Pajak)",   b3, YLW);
    print_div();

    /* Per-bracket detail */
    long long pkp  = r->pkp > 0 ? r->pkp : 0;
    long long prev = 0;
    for (int i = 0; i < 5 && pkp > 0; i++) {
        long long cap   = (LIMIT[i] < pkp) ? LIMIT[i] : pkp;
        long long slice = cap - prev;
        if (slice <= 0) break;
        long long ts = (long long)(slice * RATE[i]);
        char sb[32];
        fmt_idr(sb, sizeof(sb), ts);
        printf("  │    " DIM WHT "Lapisan %-26s" RST YLW "%s" RST "\n",
               RATE_LABEL[i], sb);
        prev = LIMIT[i];
        if (pkp <= LIMIT[i]) break;
    }

    print_div();
    print_row("Total PPh Terutang", b4, RED BLD);
    print_div();
    print_row("Penghasilan Netto",  b5, GRN BLD);
    print_close();
    printf("\n");
}

/* ── Calculator Flow ──────────────────────────────────── */
static void run_calculator(void) {
    TaxResult r = {0};
    int choice;

    /* Step 1: Marital status */
    clrscr();
    print_header();
    print_section("STATUS PERNIKAHAN");
    printf("  │  " GRN "[1]" RST "  Tidak Kawin (TK)\n");
    printf("  │  " GRN "[2]" RST "  Kawin (K)\n");
    printf("  │  " GRN "[3]" RST "  Kawin + Isteri Bekerja (K/I)\n");
    print_close();
    printf("\n");
    ask_int(&choice, "Pilih [1-3]: ", 1, 3);
    r.status = (Status)(choice - 1);

    /* Step 2: Dependents */
    clrscr();
    print_header();
    print_section("JUMLAH TANGGUNGAN");
    printf("  │  " DIM WHT "Anggota keluarga yang menjadi tanggungan (0–3)\n" RST);
    print_close();
    printf("\n");
    ask_int(&r.dep, "Tanggungan [0-3]: ", 0, 3);

    /* Step 3: Gross income */
    clrscr();
    print_header();
    print_section("PENGHASILAN BRUTO TAHUNAN");
    printf("  │  " DIM WHT "Total penghasilan bruto per tahun (Rupiah)\n" RST);
    printf("  │  " DIM CYN "Contoh: 120000000  →  Rp 120.000.000\n" RST);
    print_close();
    printf("\n");
    ask_ll(&r.income, "Penghasilan (Rp): ", 1LL, INCOME_MAX);

    /* Compute */
    r.ptkp  = calc_ptkp(r.status, r.dep);
    r.pkp   = r.income - r.ptkp;
    r.tax   = calc_tax(r.pkp > 0 ? r.pkp : 0);
    r.netto = r.income - r.tax;

    show_result(&r);

    printf("  " DIM WHT "Tekan " CYN "[Enter]" WHT " untuk kembali ke menu utama..." RST " ");
    flush_stdin();
}

/* ── Main Menu ────────────────────────────────────────── */
static void show_menu(void) {
    clrscr();
    print_header();
    print_section("MENU UTAMA");
    printf("  │  " GRN "[1]" RST "  Hitung Pajak Penghasilan\n");
    printf("  │  " RED "[0]" RST "  Keluar\n");
    print_close();
    printf("\n");
}

/* ── Entry Point ──────────────────────────────────────── */
int main(void) {
    platform_init();

    int choice;
    for (;;) {
        show_menu();
        ask_int(&choice, "Pilih [0-1]: ", 0, 1);
        if (choice == 1) run_calculator();
        else             break;
    }

    clrscr();
    printf("\n  " CYN BLD "◆" RST "  Terima kasih telah menggunakan Hitung Pajak.\n");
    printf("  " DIM CYN "◇  v2.0.0 · 2026\n\n" RST);
    return 0;
}
