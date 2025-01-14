#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>       
#include <time.h>

int main() {    
    srand((unsigned)time(NULL));
    int8_t buf[16] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    for(int i = 0; i < 16; i++) {
        if (buf[i] >= 0) {
            continue;
        }

        uint8_t x;
        do {
            x = rand()&0xF;
        } while(buf[x] >= 0);

        buf[i] = x;
        buf[x] = i;
    }

    for(int i = 0; i < 16; i++) printf("%x", buf[i]);
    puts("");
}