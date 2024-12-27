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
#include <sys/wait.h>
#include <unistd.h>
#include "handle_sigchld.h" 
#include "background_process_count.h"
#include <errno.h>

void handle_sigchld(int sig) {
	
    int status;
    pid_t pid;

    // Tüm çocuk süreçlerin durumunu kontrol et
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("[PID %d] retval: %d\n", pid, WEXITSTATUS(status));
        //background_process_count--; // Arka plan işlem sayısını azalt
		decrement_background_process_count();
    }
	
}