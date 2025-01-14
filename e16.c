#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>       
#include <unistd.h>
#include <string.h>
#include <time.h>

uint8_t map1[16] = {0, 8, 6, 5, 2, 9, 3, 7, 11, 15, 13, 12, 4, 1, 14, 10};
uint8_t map2[16] = {0, 13, 9, 3, 15, 7, 12, 4, 2, 8, 5, 6, 14, 1, 10, 11};
uint8_t map3[16] = {0, 8, 7, 14, 5, 3, 2, 12, 9, 10, 11, 15, 1, 13, 4, 6};

uint8_t map1r[16] = {0, 13, 4, 6, 12, 3, 2, 7, 1, 5, 15, 8, 11, 10, 14, 9};
uint8_t map2r[16] = {0, 13, 8, 3, 7, 10, 11, 5, 9, 2, 14, 15, 6, 1, 12, 4};
uint8_t map3r[16] = {0, 12, 6, 5, 14, 4, 15, 2, 1, 8, 9, 10, 7, 13, 3, 11};

uint8_t rmap[16] = {13, 11, 14, 8, 9, 15, 12, 10, 3, 4, 7, 1, 6, 0, 2, 5};

uint8_t rotate(uint8_t c, uint64_t cnt) {
    uint8_t c_in = c; 
    uint8_t c_rotated;
    uint8_t c_out;

    uint64_t cnt1 = cnt;
    uint64_t cnt2 = cnt>>2;
    uint64_t cnt3 = cnt>>4;

    c = (map1[(c + cnt1)&0xf] - cnt1)&0xf;
    c = (map2[(c + cnt2)&0xf] - cnt2)&0xf;
    c = (map3[(c + cnt3)&0xf] - cnt3)&0xf;
    c = rmap[c&0xf];
    c = (map3r[(c + cnt3)&0xf] - cnt3)&0xf;
    c = (map2r[(c + cnt2)&0xf] - cnt2)&0xf;
    c = (map1r[(c + cnt1)&0xf] - cnt1)&0xf;

    return c;
}

void keygen() {
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

int main(int argc, const char **argv) {
    uint8_t buf[256];
    uint8_t bytes;
    uint8_t m[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    
    if (argc == 2) {
        if (!strcmp(argv[1], "-g")) {
            keygen();
            exit(0);
        }
    
        int keylen = strlen(argv[1]);

        for (int i = 0; i < 16; i++) {
            if (i >= keylen) {
                m[i] = i;
                continue;
            } 

            char c = argv[1][i];
            uint8_t x = c >= 'a' && c <= 'f' ? 10+c-'a' :
                        c >= '0' && c <= '9' ? c-'0' :
                        -1;

            if (x == -1) {
                fprintf(stderr, "invalid character in key\n");
                exit(0);
            }

            if (x < i && m[x] != i) {
                fprintf(stderr, "non-reflective key %x<->%x %x\n", m[x], i, x);
                exit(0);
            }

            m[i] = x;
        }
    }    

    uint64_t cnt = 0;

    while ((bytes = read(STDIN_FILENO, buf, 1)) && bytes) {
        for (int i = 0; i < bytes; i++) {
            uint8_t n1 = m[rotate(m[buf[i] & 0xF], cnt++)];
            uint8_t n2 = m[rotate(m[buf[i] >> 4] , cnt++)];

            buf[i] = n1 | (n2 << 4);
        }

        write(STDOUT_FILENO, buf, bytes);
    }
}