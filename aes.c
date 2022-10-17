//201951155---Sumit Prajapati
#include <stdio.h>
#include <memory.h>
typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
int gAes_keybits=128; 
int gAes_rounds = 10;
int gAes_nk=4;
int gAes_nb=4;
static const uint32_t g_aes_rcon[] = {
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000,
    0x1b000000, 0x36000000, 0x6c000000, 0xd8000000, 0xab000000, 0xed000000, 0x9a000000
};
static const uint8_t g_aes_sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};
static const uint8_t g_inv_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};
uint8_t aes_sub_sbox(uint8_t val)
{
    return g_aes_sbox[val];
}
uint32_t aes_sub_dword(uint32_t val)
{
    uint32_t tmp = 0;
   tmp |= ((uint32_t)aes_sub_sbox((uint8_t)((val>>0) &0xFF)))<<0;
   tmp |= ((uint32_t)aes_sub_sbox((uint8_t)((val>>8) &0xFF)))<<8;
   tmp |= ((uint32_t)aes_sub_sbox((uint8_t)((val>>16) &0xFF)))<<16;
   tmp |= ((uint32_t)aes_sub_sbox((uint8_t)((val>>24) &0xFF)))<<24;
   return tmp;
}
uint32_t aes_rot_dword(uint32_t val)
{
    uint32_t tmp = val;
   return (val >> 8) | ((tmp & 0xFF) << 24);
}
uint32_t aes_swap_dword(uint32_t val)
{
    return (((val & 0x000000FF)<<24) |
            ((val & 0x0000FF00)<<8) |
            ((val & 0x00FF0000)>>8) |
            ((val & 0xFF000000)>>24) );
}
void aes_key_expansion( uint8_t *key, uint8_t *round)
{
    uint32_t *w = (uint32_t *)round;
    uint32_t t;
    int i = 0;
    printf("Key Expansion:\n");
    do {
        w[i]=*((uint32_t *)&key[i*4+0]);
        printf("%2.2d:rs: %8.8x\n", i, aes_swap_dword(w[i]));
    } while (++i < gAes_nk);
   do {
        printf("    %2.2d: ", i);
        if ((i%gAes_nk)==0) {
            t=aes_rot_dword(w[i - 1]);
            printf(" rot: %8.8x", aes_swap_dword(t));
            t=aes_sub_dword(t);
            printf(" sub: %8.8x", aes_swap_dword(t));
            printf(" rcon: %8.8x", g_aes_rcon[i/gAes_nk - 1]);
            t = t ^ aes_swap_dword(g_aes_rcon[i/gAes_nk - 1]);
            printf("xor: %8.8x",t);
        } else if (gAes_nk>6 && (i%gAes_nk)==4) {
            t=aes_sub_dword(w[i - 1]);
            printf(" sub: %8.8x", aes_swap_dword(t));
        } else {
            t = w[i - 1];
            printf(" equ: %8.8x", aes_swap_dword(t));
        }
        w[i] = w[i - gAes_nk] ^ t;
        printf(" rs: %8.8x\n", aes_swap_dword(w[i]));
    } while (++i < gAes_nb  * (gAes_rounds  + 1));
}
void aes_add_round_key(  uint8_t *state, uint8_t *round, int nr)
{
    uint32_t *w = (uint32_t *)round;
    uint32_t *s = (uint32_t *)state;
    int i;
    for (i = 0; i < gAes_nb ; i++) {
        s[i] ^= w[nr * gAes_nb  + i];
    }
}
void aes_sub_bytes(  uint8_t *state)
{
    int i, j;
     for (i = 0; i < gAes_nb ; i++) {
        for (j = 0; j < 4; j++) {
            state[i * 4 + j] = aes_sub_sbox(state[i * 4 + j]);
        }
    }
}
void aes_shift_rows(  uint8_t *state)
{
    uint8_t *s = (uint8_t *)state;
    int i, j, r;
   for (i = 1; i < gAes_nb ; i++) {
        for (j = 0; j < i; j++) {
            uint8_t tmp = s[i];
            for (r = 0; r < gAes_nb ; r++) {
                s[i + r * 4] = s[i + (r + 1) * 4];
            }
            s[i + (gAes_nb  - 1) * 4] = tmp;
        }
    }
}
uint8_t aes_xtime(uint8_t x)
{
    return ((x << 1)^(((x >> 7) & 1) * 0x1b));
}
uint8_t aes_xtimes(uint8_t x, int ts)
{
    while (ts-->0) {
        x = aes_xtime(x);
    }
   return x;
}
uint8_t aes_mul(uint8_t x, uint8_t y)
{
    return ((((y >> 0) & 1) * aes_xtimes(x, 0)) ^
            (((y >> 1) & 1) * aes_xtimes(x, 1)) ^
            (((y >> 2) & 1) * aes_xtimes(x, 2)) ^
            (((y >> 3) & 1) * aes_xtimes(x, 3)) ^
            (((y >> 4) & 1) * aes_xtimes(x, 4)) ^
            (((y >> 5) & 1) * aes_xtimes(x, 5)) ^
            (((y >> 6) & 1) * aes_xtimes(x, 6)) ^
            (((y >> 7) & 1) * aes_xtimes(x, 7)) );
}
void aes_mix_columns(  uint8_t *state)
{
    uint8_t y[16] = { 2, 3, 1, 1,  1, 2, 3, 1,  1, 1, 2, 3,  3, 1, 1, 2};
    uint8_t s[4];
    int i, j, r;
   for (i = 0; i < gAes_nb ; i++) {
        for (r = 0; r < 4; r++) {
            s[r] = 0;
            for (j = 0; j < 4; j++) {
                s[r] = s[r] ^ aes_mul(state[i * 4 + j], y[r * 4 + j]);
            }
        }
        for (r = 0; r < 4; r++) {
            state[i * 4 + r] = s[r];
        }
    }
}
void aes_dump(char *msg, uint8_t *data, int len)
{
    int i;
   printf("%8.8s: ", msg);
   for (i = 0; i < len; i++) {
        printf(" %2.2x", data[i]);
    }
    printf("\n");
}
int aes_encrypt(  uint8_t *data, int len, uint8_t *key)
{
    uint8_t w[4 * 4 * 15] = {0};
    uint8_t s[4 * 4] = {0};
     int nr, i, j;
    aes_key_expansion(  key, w);
    for (i = 0; i < len; i += 4 * gAes_nb ) {
       printf("Encrypting block at %u ...\n", i);
        for (j = 0; j < 4 * gAes_nb ; j++)
            s[j] = data[i + j];
        for (nr = 0; nr <= gAes_rounds ; nr++) {
           printf(" Round %d:\n", nr);
            aes_dump("input", s, 4 * gAes_nb );
           if (nr > 0) {
                aes_sub_bytes(  s);
                aes_dump("  sub", s, 4 * gAes_nb );
                aes_shift_rows(  s);
                aes_dump("  shift", s, 4 * gAes_nb );
               if (nr < gAes_rounds ) {
                    aes_mix_columns(  s);
                    aes_dump("  mix", s, 4 * gAes_nb );
                }
            }
            aes_add_round_key(  s, w, nr);
            aes_dump("  round", &w[nr * 4 * gAes_nb ], 4 * gAes_nb );
            aes_dump("  state", s, 4 * gAes_nb );
        }
        for (j = 0; j < 4 * gAes_nb ; j++)
            data[i + j] = s[j];
        printf("Output:\n");
        aes_dump("cypher", &data[i], 4 * gAes_nb );
    }
   return 0;
}
int aes_encrypt_ecb(  uint8_t *data, int len, uint8_t *key)
{
    return aes_encrypt(  data, len, key);
}
int aes_encrypt_cbc(  uint8_t *data, int len, uint8_t *key, uint8_t *iv)
{
    uint8_t w[4 * 4 * 15] = {0}; 
    uint8_t s[4 * 4] = {0}; 
    uint8_t v[4 * 4] = {0};
   int nr, i, j;
    aes_key_expansion(  key, w);
    memcpy(v, iv, sizeof(v));
    for (i = 0; i < len; i += 4 * gAes_nb ) {
        for (j = 0; j < 4 * gAes_nb ; j++)
            s[j] = data[i + j] ^ v[j];
        for (nr = 0; nr <= gAes_rounds ; nr++) {
           aes_dump("input", s, 4 * gAes_nb );
           if (nr > 0) {
                aes_sub_bytes(  s);
                aes_dump("  sub", s, 4 * gAes_nb );
               aes_shift_rows(  s);
                aes_dump("  shift", s, 4 * gAes_nb );
               if (nr < gAes_rounds ) {
                    aes_mix_columns(  s);
                    aes_dump("  mix", s, 4 * gAes_nb );
                }
            }
            aes_add_round_key(  s, w, nr);
            aes_dump("  round", &w[nr * 4 * gAes_nb ], 4 * gAes_nb );
            aes_dump("  state", s, 4 * gAes_nb );
        }
        for (j = 0; j < 4 * gAes_nb ; j++)
            data[i + j] = v[j] = s[j];
    }
   
    return 0;
}
void inv_shift_rows(  uint8_t *state)
{
    uint8_t *s = (uint8_t *)state;
    int i, j, r;
   for (i = 1; i < gAes_nb ; i++) {
        for (j = 0; j < gAes_nb  - i; j++) {
            uint8_t tmp = s[i];
            for (r = 0; r < gAes_nb ; r++) {
                s[i + r * 4] = s[i + (r + 1) * 4];
            }
            s[i + (gAes_nb  - 1) * 4] = tmp;
        }
    }
}
uint8_t inv_sub_sbox(uint8_t val)
{
    return g_inv_sbox[val];
}void inv_sub_bytes(  uint8_t *state)
{
    int i, j;
   for (i = 0; i < gAes_nb ; i++) {
        for (j = 0; j < 4; j++) {
            state[i * 4 + j] = inv_sub_sbox(state[i * 4 + j]);
        }
    }
}
void inv_mix_columns(  uint8_t *state)
{
    uint8_t y[16] = { 0x0e, 0x0b, 0x0d, 0x09,  0x09, 0x0e, 0x0b, 0x0d,
                      0x0d, 0x09, 0x0e, 0x0b,  0x0b, 0x0d, 0x09, 0x0e};
    uint8_t s[4];
    int i, j, r;
   for (i = 0; i < gAes_nb ; i++) {
        for (r = 0; r < 4; r++) {
            s[r] = 0;
            for (j = 0; j < 4; j++) {
                s[r] = s[r] ^ aes_mul(state[i * 4 + j], y[r * 4 + j]);
            }
        }
        for (r = 0; r < 4; r++) {
            state[i * 4 + r] = s[r];
        }
    }
}
int aes_decrypt(  uint8_t *data, int len, uint8_t *key)
{
    uint8_t w[4 * 4 * 15] = {0};
    uint8_t s[4 * 4] = {0};
   
    int nr, i, j;
    aes_key_expansion(  key, w);
    for (i = 0; i < len; i += 4 * gAes_nb ) {
       
        printf("Decrypting block at %u ...\n", i);
        for (j = 0; j < 4 * gAes_nb ; j++)
            s[j] = data[i + j];
        for (nr = gAes_rounds ; nr >= 0; nr--) {
           
            printf(" Round %d:\n", nr);
            aes_dump("input", s, 4 * gAes_nb );
            aes_add_round_key(  s, w, nr);
            aes_dump("  round", &w[nr * 4 * gAes_nb ], 4 * gAes_nb );
            if (nr > 0) {
              if (nr < gAes_rounds ) {
                    aes_dump("  mix", s, 4 * gAes_nb );
                    inv_mix_columns(  s);
                }
                aes_dump("  shift", s, 4 * gAes_nb );
                inv_shift_rows(  s);
                aes_dump("  sub", s, 4 * gAes_nb );
                inv_sub_bytes(  s);
            }
           
            aes_dump("  state", s, 4 * gAes_nb );
        }
        for (j = 0; j < 4 * gAes_nb ; j++)
            data[i + j] = s[j];
        printf("Output:\n");
        aes_dump("plain", &data[i], 4 * gAes_nb );
    }
   return 0;
}
int aes_decrypt_ecb(  uint8_t *data, int len, uint8_t *key)
{
    return aes_decrypt(  data, len, key);
}
int aes_decrypt_cbc(  uint8_t *data, int len, uint8_t *key, uint8_t *iv)
{
    uint8_t w[4 * 4 * 15] = {0};
    uint8_t s[4 * 4] = {0}; 
    uint8_t v[4 * 4] = {0};
   int nr, i, j;
    aes_key_expansion(  key, w);
    memcpy(v, iv, sizeof(v));
    for (i = 0; i < len; i += 4 * gAes_nb ) {
        for (j = 0; j < 4 * gAes_nb ; j++)
            s[j] = data[i + j];
        for (nr = gAes_rounds ; nr >= 0; nr--) {
           aes_dump("input", s, 4 * gAes_nb );
            aes_add_round_key(  s, w, nr);
            aes_dump("  round", &w[nr * 4 * gAes_nb ], 4 * gAes_nb );
              if (nr > 0) {
                 if (nr < gAes_rounds ) {
                    aes_dump("  mix", s, 4 * gAes_nb );
                    inv_mix_columns(  s);
                }
                aes_dump("  shift", s, 4 * gAes_nb );
                inv_shift_rows(  s);
                aes_dump("  sub", s, 4 * gAes_nb );
                inv_sub_bytes(  s);
            }
           aes_dump("  state", s, 4 * gAes_nb );
        }
        for (j = 0; j < 4 * gAes_nb ; j++) {
            uint8_t p = s[j] ^ v[j];
            v[j] = data[i + j];
            data[i + j] = p;
            }
    }
   return 0;
}
void main()
{
    char plain[17];
    uint8_t plaintext[16];
    printf("enter plaintex\n");
    scanf(" %[^\n]",plain);
    for(int i=0;i<16;i++)
    {
        plaintext[i]=(uint8_t)plain[i];
    }
    char ke1[17];
    uint8_t key1[16];
    printf("enter key\n");
    scanf(" %[^\n]",ke1);
    for(int i=0;i<16;i++)
    {
       key1[i]=(uint8_t)ke1[i];
    }
    uint8_t key2[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
printf("\nAES_CYPHER_128 encrypt test case:\n");
    printf("Input:\n");
    aes_dump("plaintext", plaintext, sizeof(plaintext));
    aes_dump("key ",  key1, sizeof(key1));
    aes_encrypt( plaintext, sizeof(plaintext), key1);
   printf("\nAES_CYPHER_128 decrypt test case:\n");
    printf("Input:\n");
    aes_dump("cipertext", plaintext, sizeof(plaintext));
    aes_dump("key2 ",  key2, sizeof(key2));
    aes_decrypt( plaintext, sizeof(plaintext), key2);
    printf("\nAES_CYPHER_128 encrypt test case:\n");
    printf("Input:\n");
    aes_dump("2nd_image", plaintext, sizeof(plaintext));
    aes_dump("key2 ",  key2, sizeof(key2));
    aes_encrypt( plaintext, sizeof(plaintext), key2);

}
