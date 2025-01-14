#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>       
#include <unistd.h>
#include <string.h>
#include <time.h>

uint8_t maps[][16] = {{0xa, 0x1, 0xb, 0x6, 0x3, 0xe, 0x5, 0xd, 0x4, 0x7, 0x0, 0x9, 0x8, 0xc, 0xf, 0x2},
                      {0x2, 0x1, 0x7, 0xb, 0xc, 0x4, 0xa, 0x6, 0x5, 0x3, 0x9, 0xe, 0x8, 0xf, 0x0, 0xd},
                      {0xe, 0xf, 0x8, 0x3, 0xb, 0x9, 0xc, 0x1, 0x5, 0xa, 0x7, 0x2, 0x0, 0xd, 0x6, 0x4},
                      {0xf, 0x9, 0xe, 0xd, 0xc, 0x5, 0x6, 0x4, 0x0, 0x2, 0x8, 0x7, 0x1, 0x3, 0xa, 0xb},
                      {0x9, 0x7, 0x2, 0x0, 0x1, 0xa, 0xc, 0xe, 0x4, 0xb, 0xf, 0x6, 0x5, 0x8, 0x3, 0xd},
                      {0x9, 0x4, 0x8, 0xb, 0x2, 0xc, 0xe, 0x0, 0x5, 0x6, 0x3, 0xa, 0xf, 0x1, 0xd, 0x7},
                      {0x6, 0x1, 0x2, 0xd, 0x7, 0x0, 0x5, 0x9, 0xf, 0xc, 0xa, 0xb, 0xe, 0x3, 0x4, 0x8},
                      {0x5, 0xa, 0xf, 0x2, 0x6, 0x7, 0xd, 0xe, 0xc, 0x1, 0x8, 0x9, 0x0, 0x3, 0x4, 0xb},};

uint8_t maps_rev[][16] = {{0xa, 0x1, 0xf, 0x4, 0x8, 0x6, 0x3, 0x9, 0xc, 0xb, 0x0, 0x2, 0xd, 0x7, 0x5, 0xe},
                          {0xe, 0x1, 0x0, 0x9, 0x5, 0x8, 0x7, 0x2, 0xc, 0xa, 0x6, 0x3, 0x4, 0xf, 0xb, 0xd},
                          {0xc, 0x7, 0xb, 0x3, 0xf, 0x8, 0xe, 0xa, 0x2, 0x5, 0x9, 0x4, 0x6, 0xd, 0x0, 0x1},
                          {0x8, 0xc, 0x9, 0xd, 0x7, 0x5, 0x6, 0xb, 0xa, 0x1, 0xe, 0xf, 0x4, 0x3, 0x2, 0x0},
                          {0x3, 0x4, 0x2, 0xe, 0x8, 0xc, 0xb, 0x1, 0xd, 0x0, 0x5, 0x9, 0x6, 0xf, 0x7, 0xa},
                          {0x7, 0xd, 0x4, 0xa, 0x1, 0x8, 0x9, 0xf, 0x2, 0x0, 0xb, 0x3, 0x5, 0xe, 0x6, 0xc},
                          {0x5, 0x1, 0x2, 0xd, 0xe, 0x6, 0x0, 0x4, 0xf, 0x7, 0xa, 0xb, 0x9, 0x3, 0xc, 0x8},
                          {0xc, 0x9, 0x3, 0xd, 0xe, 0x0, 0x4, 0x5, 0xa, 0xb, 0x1, 0xf, 0x8, 0x6, 0x7, 0x2}};

uint8_t rmap[16] = {13, 11, 14, 8, 9, 15, 12, 10, 3, 4, 7, 1, 6, 0, 2, 5};

uint8_t rotate(uint8_t c, uint64_t cnt, uint64_t hash) {
    int i = 0;
    
    for (; i < 8; i++) {
        uint64_t shifted = (cnt >> (4*i));
        uint8_t hash_offset = (hash >> 4*i) & 0xff; 
        uint64_t offset = shifted + hash_offset;
        c = (maps[i][(c+offset)&0xf]-offset)&0xf;
    }
    c = rmap[c&0xf];
    for (--i; i >= 0; i--) {
        uint64_t shifted = (cnt >> (4*i));
        uint8_t hash_offset = (hash >> 4*i) & 0xff; 
        uint64_t offset = shifted + hash_offset;
        c = (maps_rev[i][(c+offset)&0xf]-offset)&0xf;
    }
    return c;
}

void rotgen() {
    srand((unsigned)time(NULL));
    int8_t buf[16] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int8_t rbuf[16] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    uint8_t x;
 
    for(int i = 0; i < 16; ) {
        x = rand()%16;
        int free = 1;

        for (int j = 0; j < i; j++)
            if (buf[j] == x) {
                free = 0;
                break;
            }

        if (free) {
            buf[i] = x;
            rbuf[x] = i;
            i++;
            continue;
        }
   }

    
    printf("{0x%x", buf[0]);
    for (int i = 1; i < 16; i++)
        printf(", 0x%x", buf[i]); 
    puts("}");

    printf("{0x%x", rbuf[0]);
    for (int i = 1; i < 16; i++)
        printf(", 0x%x", rbuf[i]); 
    puts("}");
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

uint64_t djb2(uint8_t buf[static 16]) {
    uint64_t hash = 5381;
    int c;
    for (int i = 0; i < 8; i++)
        hash = ((hash << 5) + hash) + (buf[2*i] | (buf[2*i+1] << 4)); /* hash * 33 + c */
    return hash;
}

int main(int argc, const char **argv) {
    uint8_t buf[2];
    uint8_t bytes;
    uint8_t m[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    int hex_input = 0, hex_output = 0;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-g")) {
            keygen();
            exit(0);
        }
        if (!strcmp(argv[i], "-r")) {
            rotgen();
            exit(0);
        }
        if (argv[i][0] == '-') {
            hex_input  = strchr(argv[i], 'x') != 0;
            hex_output = strchr(argv[i], 'h') != 0;
            continue;
        }

        int keylen = strlen(argv[i]);
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
    uint64_t hash = djb2(m);
    uint8_t  last_nibble;

    while ((bytes = read(STDIN_FILENO, buf, 1)) && bytes) {
            uint8_t n1;
            uint8_t n2;

            if (hex_input) {            
                uint8_t x = buf[0] >= 'a' &&  buf[0] <= 'f' ? 10 + buf[0]-'a' :
                            buf[0] >= '0' &&  buf[0] <= '9' ? buf[0] -    '0' :
                            -1;
                if (x == -1) {
                    continue;
                }

                n1 = m[rotate(m[x], cnt++, hash)];
            } else {
                n1 = m[rotate(m[buf[0] & 0xF], cnt++, hash)];
                n2 = m[rotate(m[buf[0] >> 4] , cnt++, hash)];
            }

            if (hex_output) {
                printf("%x", n1);
                if (!hex_input)
                    printf("%x", n2); 
            } else {
                if (hex_input) {
                    if ((cnt&1) == 0) {
                        putc(last_nibble | (n1 << 4), stdout);
                    } else {
                        last_nibble = n1;
                    }
                    continue;
                }  

                putc(n1 | (n2 << 4), stdout);
            }
        }
}