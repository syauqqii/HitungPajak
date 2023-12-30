// =========================================
// Import Library
// =========================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

// =========================================
// Import Library with Case
// =========================================
#ifdef _WIN32
#include <windows.h>
#define OS_Windows
#else
#include <unistd.h>
#endif

// =========================================
// Declaration Function
// =========================================
void clear_screen();
void format_angka();
void menu();

// =========================================
// Main Function
// =========================================
int main(){
	#ifdef OS_Windows
	system("title App Hitung Pajak");
	#endif
	
	// Panggil function clear_screen
	clear_screen();
	
	// Panggil function menu
	menu(1);

	return 0;
}

// =========================================
// clear_screen() function
// =========================================
void clear_screen(){
	// Untuk bersihkan konsol
	// Command cmd / terminal
	system("@cls||clear");
}

// =========================================
// format_angka function
// @params:
// 	- number(angka) represent integer
// =========================================
void format_angka(long long int angka){
	// Jika parameter angka < 0 / negatif, maka tampilkan negatif
	if(angka < 0){
		printf("-");
		format_angka(-angka);
		return;
	} // JIka parameter kurang dari 1000, atau bisa di sebut > 0 - 99,
	// Maka tampilkan langusung tanpa di beri format . (titik)
	else if(angka < 1000){
		printf("%lld", angka);
		return;
	}
	
	// Rekrusif mengembalikan nilai angka dengan di bagi 1000
	format_angka(angka / 1000);
	
	// Print hasil dengan format
	printf(".%03lld", angka % 1000);
}

// =========================================
// menu() function
// @params:
// 	- number(option) represent integer
// =========================================
void menu(int option){
	// Deklarasi input menu awal
	int input_pilihan_menu; // Pilih menu diawal
	
	// Deklarasi hitung ulang / exit
	char menu_exit;
	
	// Deklarasi variabel penting
	long long int penghasilan, pajak; // Untuk input jumlah penghasilan, pajak
	long long int lapis_1, lapis_2, lapis_3, lapis_4, lapis_5; // Lapisan pajak
	long long int ptkp, pkp; // Untuk mengetahui berapa penghasilan tidak kena pajak dan kena pajak
	
	// Deklarasi untuk keperluan perhitungan PTKP
	char status_pernikahan; // Sudah nikah atau belum
	char status_penghasilan; // Jika sudah nikah -> penghasilan digabung?
	int jumlah_tanggungan, ulangi; // Jumlah yang ditanggung
	char temp_pernikahan[500];
	char temp_penghasilan[500];
	
	for(ulangi = 0; ulangi < 500; ulangi++){
		temp_pernikahan[ulangi]  = '\0';
		temp_penghasilan[ulangi] = '\0';
	}
	
	// Menu awal -> menu tidak tersedia (handle error)
	if(option == 0){
		clear_screen();
		
		printf("\n  [#] Error: Menu Tidak Tersedia!\n");
		
		sleep(1);
		exit(0);
	} // Menu awal -> halaman utama
	else if(option == 1){
		clear_screen();
		
		printf("\n [#] Program Menghitung Pajak\n\n");
		printf("  [1] Hitung Pajak\n");
		printf("  [2] Keluar dari Program\n\n");
		
		printf("  [>] Pilih Menu    : [1-2] ");
		scanf("%d", &input_pilihan_menu);
		
		if(input_pilihan_menu == 1 || input_pilihan_menu == 2){
			if(input_pilihan_menu == 1){
				menu(2);
			} else if(input_pilihan_menu == 2){
				printf("  [?] Keluar Program? [Y/T] ");
				scanf(" %c", &menu_exit);
				
				menu_exit = toupper(menu_exit);

				if(menu_exit == 'Y'){
					menu(9);
				} else if(menu_exit == 'T'){
					menu(1);
				} else {
					menu(0);
				}
			}else{
				menu(0);
			}
		} else {
			menu(0);
		}
	} // Menu awal -> proses penghitungan pajak
	else if(option == 2){
		clear_screen();
		
		printf("\n [#] Program Menghitung Pajak\n\n");
		
		printf("  [>] Apakah anda sudah nikah? [Y/T] ");
		scanf(" %c", &status_pernikahan);
		
		status_pernikahan = toupper(status_pernikahan);
		
		// Hitung PTKP -> nikah
		if(status_pernikahan == 'Y'){
			strcat(temp_pernikahan, "Iya");
			
			printf("  [>] Apakah penghasilan anda digabung? [Y/T] ");
			scanf(" %c", &status_penghasilan);
			
			status_penghasilan = toupper(status_penghasilan);
			
			// Hitung PTKP -> nikah -> digabung
			if(status_penghasilan == 'Y'){
				strcat(temp_penghasilan, "Iya");
				
				ptkp = 112500000;
				
				printf("  [>] Berapa jumlah tanggungan anda? [0-3] ");
				scanf(" %d", &jumlah_tanggungan);
				
				if(jumlah_tanggungan == 0 || jumlah_tanggungan == 1 || jumlah_tanggungan == 2 || jumlah_tanggungan == 3){
					for(ulangi = 0; ulangi < jumlah_tanggungan; ulangi++){
						ptkp += 4500000;
					}
				} else {
					clear_screen();
					menu(0);
				}
			} // Hitung PTKP -> nikah -> tidak digabung
			else if(status_penghasilan == 'T'){
				strcat(temp_penghasilan, "Tidak");
				
				ptkp = 58500000;
				
				printf("  [>] Berapa jumlah tanggungan anda? [0-3] ");
				scanf(" %d", &jumlah_tanggungan);
				
				if(jumlah_tanggungan == 0 || jumlah_tanggungan == 1 || jumlah_tanggungan == 2 || jumlah_tanggungan == 3){
					for(ulangi = 0; ulangi < jumlah_tanggungan; ulangi++){
						ptkp += 4500000;
					}
				} else {
					clear_screen();
					menu(0);
				}
			} // Hitung PTKP -> nikah -> handle error input
			else {
				menu(2);
			}
		} // Hitung PTKP -> tidak nikah
		else if(status_pernikahan == 'T'){
			strcat(temp_pernikahan, "Tidak");
			strcat(temp_penghasilan, "Tidak");
			
			ptkp = 54000000;
			
			printf("  [>] Berapa jumlah tanggungan anda? [0-3] ");
			scanf(" %d", &jumlah_tanggungan);
			
			if(jumlah_tanggungan == 0 || jumlah_tanggungan == 1 || jumlah_tanggungan == 2 || jumlah_tanggungan == 3){
					for(ulangi = 0; ulangi < jumlah_tanggungan; ulangi++){
						ptkp += 4500000;
					}
				} else {
					clear_screen();
					menu(0);
				}
		} // Hitung PTKP -> handle error input
		else {
			menu(2);
		}
		
		clear_screen();
		
		printf("\n [#] Program Menghitung Pajak\n\n");
		
		printf("  [>] PTKP anda            : Rp "); format_angka(ptkp);
		printf("\n  [>] Masukkan Penghasilan : Rp ");
		scanf("%lld", &penghasilan);
		
		pkp = penghasilan - ptkp;
		
		// Lapis 1 : 0 - 60jt
		if(pkp > 0 && pkp <= 60000000){
			lapis_1 = pkp * 0.05;
			pajak   = lapis_1;
		} // Lapis 2 : > 60jt - 250jt
		else if(pkp > 60000000 && pkp <= 250000000){
			lapis_1 = 60000000 * 0.05;
			
			lapis_2 = (pkp - 60000000) * 0.15;
			
			pajak   = lapis_1 + lapis_2;
		} // Lapis 3 : > 250jt - 500jt
		else if(pkp > 250000000 && pkp <= 500000000){
			lapis_1 = 60000000 * 0.05;
			lapis_2 = (250000000 - 60000000) * 0.15;
			
			lapis_3 = (pkp - 250000000) * 0.25;
			
			pajak   = lapis_1 + lapis_2 + lapis_3;
		} // Lapis 4 : > 500jt - 5m
		else if(pkp > 500000000 && pkp <= 5000000000){
			lapis_1 = 60000000 * 0.05;
			lapis_2 = (250000000 - 60000000) * 0.15;
			lapis_3 = (500000000 - 250000000) * 0.25;
			
			lapis_4 = (pkp - 500000000) * 0.3;
			
			pajak   = lapis_1 + lapis_2 + lapis_3 + lapis_4;
		} // Lapis 5 : > 5m
		else if(pkp > 5000000000){
			lapis_1 = 60000000 * 0.05;
			lapis_2 = (250000000 - 60000000) * 0.15;
			lapis_3 = (500000000 - 250000000) * 0.25;
			lapis_4 = (5000000000 - 500000000) * 0.3;
			
			lapis_5 = (pkp - 5000000000) * 0.35;
			
			pajak = lapis_1 + lapis_2 + lapis_3 + lapis_4 + lapis_5;
		} // Jika hasil minus, maka tidak wajib pajak
		else {
			clear_screen();
			
			printf("\n [>] Anda tidak wajib pajak!\n");
			
			sleep(3);
			
			clear_screen();
			
			printf("           [ LAPORAN PAJAK ]\n");
			printf(" ---------------------------------------\n");
			printf(" [>] Status Pajak : Tidak Wajib Pajak\n");
			printf(" ---------------------------------------\n");
			printf(" [>] Status Pernikahan     : %s\n", temp_pernikahan);
			printf(" [>] Penghasilan di Gabung : %s\n", temp_penghasilan);
			printf(" [>] Jumlah Tanggungan     : %d\n", jumlah_tanggungan);
			printf(" ---------------------------------------");
			printf("\n [>] Penghasilan : Rp "); format_angka(penghasilan);
			printf("\n ---------------------------------------");
			printf("\n [>] PTKP : Rp "); format_angka(ptkp);
			printf("\n [>] PKP  : Rp 0");
			printf("\n ---------------------------------------");
			printf("\n [>] Total Pajak       : Rp 0");
			printf("\n [>] Peghasilan Bersih : Rp "); format_angka(penghasilan);
			printf("\n ---------------------------------------");
			
			menu_exit = '\0';
			
			printf("\n [?] Keluar Program? [Y/T] ");
			scanf(" %c", &menu_exit);
			
			menu_exit = toupper(menu_exit);
			
			if(menu_exit == 'Y'){
				menu(9);
			} else if(menu_exit == 'T'){
				menu(1);
			} else {
				menu(0);
			}
		}
		
		clear_screen();
		
		printf("           [ LAPORAN PAJAK ]\n");
		printf(" ---------------------------------------\n");
		printf(" [>] Status Pajak : Wajib Pajak\n");
		printf(" ---------------------------------------\n");
		printf(" [>] Status Pernikahan     : %s\n", temp_pernikahan);
		printf(" [>] Penghasilan di Gabung : %s\n", temp_penghasilan);
		printf(" [>] Jumlah Tanggungan     : %d\n", jumlah_tanggungan);
		printf(" ---------------------------------------");
		printf("\n [>] Penghasilan : Rp "); format_angka(penghasilan);
		printf("\n ---------------------------------------");
		printf("\n [>] PTKP : Rp "); format_angka(ptkp);
		printf("\n [>] PKP  : Rp "); format_angka(pkp);
		printf("\n ---------------------------------------");
		printf("\n [>] Total Pajak       : Rp "); format_angka(pajak);
		printf("\n [>] Peghasilan Bersih : Rp "); format_angka(penghasilan - pajak);
		printf("\n ---------------------------------------");
		
		menu_exit = '\0';
		
		printf("\n [?] Keluar Program? [Y/T] ");
		scanf(" %c", &menu_exit);
		
		menu_exit = toupper(menu_exit);
		
		if(menu_exit == 'Y'){
			menu(9);
		} else if(menu_exit == 'T'){
			menu(1);
		} else {
			menu(0);
		}
	} // Menu awal -> keluar program
	else if(option == 9){
		
		clear_screen();
		
		printf("\n [#] Keluar Program\n");
		
		sleep(1);
		exit(0);
	}
}
