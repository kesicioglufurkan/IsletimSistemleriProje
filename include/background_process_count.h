#ifndef BACKGROUND_PROCESS_COUNT_H
#define BACKGROUND_PROCESS_COUNT_H

#include <signal.h>

extern volatile sig_atomic_t background_process_count;

// Fonksiyon bildirimleri
void increment_background_process_count();
void decrement_background_process_count();
sig_atomic_t get_background_process_count();

#endif
