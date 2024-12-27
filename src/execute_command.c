/*
GRUP ÜYELERİ:
G221210383 Metin Aydın
G221210071 Bilal avcı
G22121008G Furkan Ay
G221210047 Furkan Kesicioğlu
G221210045 Mehmet Bosdancı
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> // Dosya açmak için gerekli
#include "parse_command.h"   // Argümanları ayrıştırmak için parse_command fonksiyonu
#include "execute_command.h" // execute_command fonksiyonu için
#include "background_process_count.h"

void execute_command(char *command) {
	//printf("Komut çalıştırılıyor: %s\n", command);  // Burada komutun geldiğini kontrol edin
		
    char *args[20];
    char *input_file = NULL;
    char *output_file = NULL;
    int background = 0;

    // Giriş ve çıkış yönlendirme kontrolü
    char *redirect_pos_in = strchr(command, '<');
    char *redirect_pos_out = strchr(command, '>');

    // Giriş yönlendirme ( < )
    if (redirect_pos_in != NULL) {
        *redirect_pos_in = '\0';
        input_file = redirect_pos_in + 1;
        while (*input_file == ' ') input_file++;
    }

    // Çıkış yönlendirme ( > )
    if (redirect_pos_out != NULL) {
        *redirect_pos_out = '\0';
        output_file = redirect_pos_out + 1;
        while (*output_file == ' ') output_file++;
    }

    // Arka plan kontrolü ( & )
    char *ampersand = strchr(command, '&');
    if (ampersand != NULL) {
        background = 1;
        *ampersand = '\0';
    }


    // Argümanları ayrıştır
    parse_command(command, args);


    pid_t pid = fork();
    if (pid < 0) {
        perror("fork başarısız");
        return;
    }

    if (pid == 0) {
        // Çocuk süreç

        // Giriş dosyasına yönlendirme
        if (input_file != NULL) {
            int fd_in = open(input_file, O_RDONLY);
            if (fd_in < 0) {
                perror("Giriş dosyası açılamadı");
                exit(1);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }

        // Çıkış dosyasına yönlendirme
        if (output_file != NULL) {
            int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out < 0) {
                perror("Çıkış dosyası açılamadı");
                exit(1);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }

        if (execvp(args[0], args) == -1) {
            perror("Komut çalıştırılamadı");
            exit(1);
        }
    } else {
        if (!background) {
            int status;
            waitpid(pid, &status, 0);
        } else {
            printf("[PID %d] arka planda çalışıyor\n", pid);
            //background_process_count++;
			increment_background_process_count();
        }
    }
}