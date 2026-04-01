```
  ◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆
  ║                                                  ║
  ║    ▲  H I T U N G   P A J A K                  ║
  ║    ◆  Indonesian Income Tax Calculator           ║
  ║    ◇  v2.0.0 · 2026                             ║
  ║                                                  ║
  ◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆
```

![Language](https://img.shields.io/badge/language-C11-blue?style=flat-square)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey?style=flat-square)
![License](https://img.shields.io/badge/license-MIT-green?style=flat-square)

Kalkulator **Pajak Penghasilan Pasal 21** berbasis terminal. Menghitung PPh secara progresif berdasarkan status pernikahan, jumlah tanggungan, dan penghasilan bruto tahunan.

---

## Fitur

- Tarif progresif 5 lapisan sesuai PMK-101/PMK.010/2016
- Perhitungan PTKP otomatis (TK / K / K+I)
- Rincian per lapisan pajak
- Validasi input yang aman — tidak ada buffer overflow
- UI terminal dengan desain geometris + warna ANSI
- Cross-platform: **Linux · macOS · Windows**
- Zero dependency — hanya C stdlib

---

## Build & Run

**Linux / macOS**
```sh
make
./hitung_pajak
```

**Windows (MinGW / MSYS2)**
```sh
make
./hitung_pajak.exe
```

**Manual (semua platform)**
```sh
gcc -std=c11 -Wall -O2 -o hitung_pajak main.c
```

---

## Tarif PPh Pasal 21

| Lapisan | PKP                        | Tarif |
|---------|----------------------------|-------|
| 1       | Rp 0 – 60 juta             | 5%    |
| 2       | Rp 60 juta – 250 juta      | 15%   |
| 3       | Rp 250 juta – 500 juta     | 25%   |
| 4       | Rp 500 juta – 5 miliar     | 30%   |
| 5       | > Rp 5 miliar              | 35%   |

---

## Tabel PTKP

| Status         | +0 tanggungan  | +1             | +2             | +3             |
|----------------|----------------|----------------|----------------|----------------|
| TK (Tidak Kawin) | Rp 54.000.000 | Rp 58.500.000 | Rp 63.000.000 | Rp 67.500.000 |
| K (Kawin)        | Rp 58.500.000 | Rp 63.000.000 | Rp 67.500.000 | Rp 72.000.000 |
| K/I (Gabung)     | Rp 112.500.000| Rp 117.000.000| Rp 121.500.000| Rp 126.000.000|

> Dasar hukum: PMK Nomor 101/PMK.010/2016

---

## Lisensi

MIT — bebas digunakan untuk keperluan edukasi dan pribadi.
