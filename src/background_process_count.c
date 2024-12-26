#include "background_process_count.h"

volatile sig_atomic_t background_process_count = 0;

void increment_background_process_count() {
    background_process_count++;
}

void decrement_background_process_count() {
    if (background_process_count > 0) {
        background_process_count--;
    }
}

sig_atomic_t get_background_process_count() {
    return background_process_count;
}
