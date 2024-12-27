/*
GRUP ÜYELERİ:
G221210383 Metin Aydın
G221210071 Bilal avcı
G221210089 Furkan Ay
G221210047 Furkan Kesicioğlu
G221210045 Mehmet Bosdancı
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "parse_command.h"
#include "execute_command.h"
#include "pipe.h"
#include "background_process_count.h"

void my_pipe(char *commands[],char *args[],char *input_file,char *output_file,int background,int i){
				
			// Boru işlemi kontrolü ( | )
            char *subcommands[10];
            int subcommand_count = 0;
            char *subtoken = strtok(commands[i], "|");
            while (subtoken != NULL) {
                subcommands[subcommand_count++] = subtoken;
                subtoken = strtok(NULL, "|");
            }

            if (subcommand_count > 1) {
                // Boru işlemi
                int pipefd[2];
                int prev_pipe_read = -1;

                for (int j = 0; j < subcommand_count; j++) {
                    if (j < subcommand_count - 1) {
                        if (pipe(pipefd) == -1) {
                            perror("pipe başarısız");
                            exit(1);
                        }
                    }

                    pid_t pid = fork();
                    if (pid < 0) {
                        perror("fork başarısız");
                        exit(1);
                    }

                    if (pid == 0) {
                        // Çocuk süreç

                        // Giriş dosyasına yönlendirme
                        if (input_file != NULL && j == 0) { // İlk komut için giriş yönlendirme
                            int fd_in = open(input_file, O_RDONLY);
                            if (fd_in < 0) {
                                perror("Giriş dosyası açılamadı");
                                exit(1);
                            }
                            dup2(fd_in, STDIN_FILENO); // Standart girdiyi dosyaya yönlendir
                            close(fd_in);
                        }

                        if (prev_pipe_read != -1) {
                            dup2(prev_pipe_read, STDIN_FILENO);
                            close(prev_pipe_read);
                        }

                        if (j < subcommand_count - 1) {
                            close(pipefd[0]);
                            dup2(pipefd[1], STDOUT_FILENO);
                            close(pipefd[1]);
                        } else if (output_file != NULL) { // Son komut için çıkış yönlendirme
                            int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                            if (fd_out < 0) {
                                perror("Çıkış dosyası açılamadı");
                                exit(1);
                            }
                            dup2(fd_out, STDOUT_FILENO);
                            close(fd_out);
                        }

                        parse_command(subcommands[j], args);

                        if (execvp(args[0], args) == -1) {
                            perror("Komut çalıştırılamadı");
                            exit(1);
                        }
                    } else {
                        // Ebeveyn süreç
                        if (prev_pipe_read != -1) {
                            close(prev_pipe_read);
                        }
                        if (j < subcommand_count - 1) {
                            close(pipefd[1]);
                        }
                        prev_pipe_read = pipefd[0];
                    }
                }

                for (int j = 0; j < subcommand_count; j++) {
                    wait(NULL);
                }
            } else {
                // Tek komut için işlem
                pid_t pid = fork();
                if (pid < 0) {
                    perror("fork başarısız");
                    //continue;
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
                        dup2(fd_in, STDIN_FILENO); // Standart girdiyi dosyaya yönlendir
                        close(fd_in);
                    }

                    // Çıkış dosyasına yönlendirme
                    if (output_file != NULL) {
                        int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if (fd_out < 0) {
                            perror("Çıkış dosyası açılamadı");
                            exit(1);
                        }
                        dup2(fd_out, STDOUT_FILENO); // Standart çıktıyı dosyaya yönlendir
                        close(fd_out);
                    }

                    parse_command(commands[i], args);

                    if (execvp(args[0], args) == -1) {
                        perror("Komut çalıştırılamadı");
                        exit(1);
                    }
                } else {
                    if (!background) {
                        // Ön planda çalıştırılan işlemi bekle
                        int status;
                        waitpid(pid, &status, 0);
                    } else {
                        // Arka plan işlemde sadece PID göster ve işlem sayısını artır
                        printf("[PID %d] arka planda çalışıyor\n", pid);
                        //background_process_count++;
						increment_background_process_count();
                    }
                }
            }
			
}
