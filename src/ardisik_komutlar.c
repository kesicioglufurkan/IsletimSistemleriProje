/*
GRUP ÜYELERİ:
G221210383 Metin Aydın
G221210071 Bilal Avcı
G221210089 Furkan Ay
G221210047 Furkan Kesicioğlu
G221210045 Mehmet Bosdancı
*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "ardisik_komutlar.h"
#include "execute_command.h"
#include "pipe.h"
#include "background_process_count.h"

int ardisik_komutlar(char command[],char *args[]){
	
	
	
		// Noktalı virgül işlemi ( ; )
        char *commands[10];
        int command_count = 0;
        char *token = strtok(command, ";");
        while (token != NULL) {
            commands[command_count++] = token;
            token = strtok(NULL, ";");
        }

        // Noktalı virgül kullanımı (ardışık komutlar)
        for (int i = 0; i < command_count; i++) {
            // Giriş ve çıkış yönlendirme kontrolü
            char *input_file = NULL;
            char *output_file = NULL;
            char *redirect_pos_in = strchr(commands[i], '<');
            char *redirect_pos_out = strchr(commands[i], '>');
            int background = 0;

			//Birden fazla komut için quit kısmı
			if(command_count>1){
				// Quit komutunu kontrol et
				for (int i = 0; i < command_count; i++) {
					// Quit komutunu algıla ve işle
					if (strcmp(commands[i], "quit") == 0) {
						printf("Çıkış yapılıyor, arka plan işlemleri bekleniyor...\n");
						while (get_background_process_count() > 0) {
							pause();
						}
						printf("Tüm arka plan işlemleri tamamlandı. Çıkılıyor.\n");
						return 0;
					}
					
						execute_command(commands[i]);
				}
			}
			
		
			
			
            // Giriş yönlendirme ( < )
            if (redirect_pos_in != NULL) {
                *redirect_pos_in = '\0'; // Komutu ayır
                input_file = redirect_pos_in + 1;
                while (*input_file == ' ') input_file++; // Boşlukları geç
            }

            // Çıkış yönlendirme ( > )
            if (redirect_pos_out != NULL) {
                *redirect_pos_out = '\0'; // Komutu ayır
                output_file = redirect_pos_out + 1;
                while (*output_file == ' ') output_file++; // Boşlukları geç
            }

            // Arka plan kontrolü ( & )
            char *ampersand = strchr(commands[i], '&');
            if (ampersand != NULL) {
                background = 1;
                *ampersand = '\0'; // Komuttan '&' sembolünü çıkar
            }

            my_pipe(commands,args,input_file,output_file,background,i);

        }
		return 1; // Fonksiyonun sonunda return ekledim
}
		
