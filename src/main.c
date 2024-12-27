/*
GRUP ÜYELERİ:
G221210383 Metin Aydın
G221210071 Bilal Avcı
G221210089 Furkan Ay
G221210047 Furkan Kesicioğlu
G221210045 Mehmet Bosdancı
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> // Dosya açmak için gerekli
#include <signal.h> // Arka plan işlemleri için gerekli
#include "handle_sigchld.h"
#include "ardisik_komutlar.h"
#include "background_process_count.h"

//volatile sig_atomic_t background_process_count = 0; // Arka plan işlem sayısı

int main() {
	
	//char command[] = "echo Hello World";
    //execute_command(command);  // Komut burada çalıştırılacak
	
    char command[256]; // Kullanıcı komutlarını saklar
    char *args[20];     // Argüman dizisi

    // SIGCHLD sinyalini yakala
    struct sigaction sa;
    sa.sa_handler = handle_sigchld;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);

	
    while (1) {
        printf("> ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL) {
            printf("\n");
            break;
        }

        // Komuttaki newline karakterini kaldır
        command[strcspn(command, "\n")] = 0;

		// Quit komutunu kontrol et tekli komut için
        if (strcmp(command, "quit") == 0) {
            printf("Çıkış yapılıyor, arka plan işlemleri bekleniyor...\n");
            // Arka plan işlemleri tamamlanana kadar bekle
            while (get_background_process_count() > 0) {
                pause(); // Arka plan süreçlerinin bitmesini bekle
            }
            printf("Tüm arka plan işlemleri tamamlandı. Çıkılıyor.\n");
            break;
        }
		
		int a ;
		a = ardisik_komutlar(command,args) ;
		if(a==0){return 0;}
		
    }

    return 0;
}
