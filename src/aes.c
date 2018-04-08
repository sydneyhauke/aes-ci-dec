#include <aes.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static unsigned char s_box[256] =
{
   0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
   0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
   0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
   0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
   0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
   0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
   0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
   0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
   0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
   0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
   0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
   0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
   0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
   0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
   0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
   0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

static unsigned char rcon[256] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
    0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
    0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
    0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
    0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
    0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
    0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
    0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
    0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
    0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
    0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
    0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
};

static void
key_expansions(enum AES_VERSION v, uint8_t **expended_key, const uint8_t *aes_cipher_key)
{
    uint8_t b,n;
    uint8_t rcon_i;

    switch (v) {
        case AES128:
            n = 16;
            b =176;
            break;
        case AES192:
            n = 24;
            b = 208;
            break;
        case AES256:
            n = 32;
            b = 240;
            break;
        default:
            fprintf(stderr, "AES version unrecognized\n");
            exit(EXIT_FAILURE);
    }

    /* Allocation has to be freed later with the according function */
    *expended_key = calloc(b, sizeof(uint8_t));
    if(*expended_key == NULL) {
        perror("Unable to allocate memory for round keys");
        exit(EXIT_FAILURE);
    }

    uint8_t added_bytes = 0; // Added bytes on the expanded key
    rcon_i = 1;

    /* Copy cipher key as part of the at the front of the expendend key */
    for(uint8_t i = 0; i < n; i++) {
        (*expended_key)[i] = aes_cipher_key[i];
        added_bytes++;
    }

    /* We need b bytes of expanded keys */
    while(added_bytes <= b) {
        uint8_t t[4];

        /* First four bytes */
        /* Copy the previous 4 bytes written in the expanded key */
        memcpy(t, (*expended_key)+added_bytes-4, 4);

        /* Key schedule core on t */
        uint8_t r[4]; // 32 bits output

        (*(uint32_t*)r) = (*(uint32_t*)t); // Copy 4 bytes

        uint8_t temp_rot = r[3];
        (*(uint32_t*)r) = ((*(uint32_t*)r) << 24) | temp_rot; // 8 bit rotation on the left

        for(uint8_t i = 0; i < 4; i++) {
            r[i] = s_box[r[i]]; // Apply S_box byte on byte on output
        }

        r[3] ^= rcon[rcon_i]; // Rcon on MSB

        /* Done key schedule core */
        rcon_i++;

        /* Apply temporary value, this becomes the next 4 bytes of the expended
         * key */
        for(uint8_t i = 0; i < 4; i++) {
            (*expended_key)[added_bytes+i] = r[i] ^ (*expended_key)[added_bytes-n+i];
        }
        added_bytes += 4;

        /* Next twelve bytes */
        for(uint8_t i = 0; i < 3; i++) {
            memcpy(t, (*expended_key)+added_bytes-4, 4);
            for(uint8_t j = 0; j < 4; j++) {
                (*expended_key)[added_bytes+j] = t[j] ^ (*expended_key)[added_bytes-n+j];
            }
            added_bytes += 4;
        }

        /* Next is for AES-256 only */
        if(v != AES256) continue;

        memcpy(t, (*expended_key)+added_bytes-4, 4);
        for(uint8_t i = 0; i < 4; i++) {
            t[i] = s_box[t[i]];
        }

        for(uint8_t i = 0; i < 3; i++) {
            (*expended_key)[added_bytes+i] = t[i] ^ (*expended_key)[added_bytes-n+i];
        }
        added_bytes += 4;

        /* Next is for AES-256 and AES-192 */
        if(v != AES192 || v != AES256) continue;

        for(uint8_t i = 0; i < (v == AES192 ? 2:3); i++) {
            memcpy(t, (*expended_key)+added_bytes-4, 4);
            for(uint8_t j = 0; j < 4; j++) {
                (*expended_key)[added_bytes+j] = t[j] ^ (*expended_key)[added_bytes-n+j];
            }
            added_bytes += 4;
        }
    }
}

static void
add_round_key(const uint8_t *roundKey, uint8_t *state)
{
    for(size_t i = 0; i < ROUND_KEY_SIZE; i++) {
        state[i] ^= roundKey[i];
    }
}

static void
sub_bytes(uint8_t *state) {
    for(size_t i = 0; i < STATE_SIZE; i++) {
        state[i] = s_box[state[i]];
    }
}

static void
shift_rows(uint8_t *state) {
    /* First row is untouched */
    uint32_t tword;
    uint8_t tbyte;

    for(uint8_t i = 1; i < 4; i++) {
        tword = ((uint32_t*)state)[i];
        for(uint8_t j = 0; j < i; j++) {
            tbyte = (tword >> 24);
            tword <<= 8;
            tword |= tbyte;
        }

        ((uint32_t*)state)[i] = tword;
    }
}

/* Source : https://en.wikipedia.org/wiki/Rijndael_MixColumns */
static void
mix_columns(uint8_t *col)
{
  uint8_t a[4];
  uint8_t b[4];
  uint8_t c;
  uint8_t h;

  /* The array 'a' is simply a copy of the input array 'r'
   * The array 'b' is each element of the array 'a' multiplied by 2
   * in Rijndael's Galois field
   * a[n] ^ b[n] is element n multiplied by 3 in Rijndael's Galois field */

  for (c = 0; c < 4; c++) {
      a[c] = col[c];
      /* h is 0xff if the high bit of r[c] is set, 0 otherwise */
      h = (uint8_t)((int8_t)col[c] >> 7); /* arithmetic right shift, thus shifting in either zeros or ones */
      b[c] = col[c] << 1; /* implicitly removes high bit because b[c] is an 8-bit char, so we xor by 0x1b and not 0x11b in the next line */
      b[c] ^= 0x1B & h; /* Rijndael's Galois field */
  }

  col[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
  col[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
  col[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
  col[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
}

static void
do_block128(uint8_t n_rounds,
            const uint8_t *key_expansions,
            const uint8_t *plaintext,
            uint8_t *ciphertext)
{
    /* Cipther text is the state now */
    memcpy(ciphertext, plaintext, STATE_SIZE);

    /* First round */
    add_round_key(key_expansions + ROUND_KEY_SIZE * 0, ciphertext);

    /* N rounds */
    for(uint8_t i = 0; i < n_rounds; i++) {
      sub_bytes(ciphertext);
      shift_rows(ciphertext);
      /* TODO : Mix columns to be integrated */
      add_round_key(key_expansions + (i+1) * ROUND_KEY_SIZE, ciphertext);
    }

    /* Final round */
    sub_bytes(ciphertext);
    shift_rows(ciphertext);
    add_round_key(key_expansions + (n_rounds+1) * ROUND_KEY_SIZE, ciphertext);
}

int
aes_cipher(enum AES_VERSION v,
           const uint8_t *aes_cipher_key,
           const uint8_t *plaintext,
           size_t plain_len,
           uint8_t *ciphertext,
           size_t cipher_len)
{
    if(cipher_len % STATE_SIZE != 0 || cipher_len < plain_len) {
        fprintf(stderr,
                "Ciphertext buffer has incorrect size : %ld bytes",
                cipher_len);
        return -EINVAL;
    }

    uint8_t *key_expansions;

    key_expansions(v, &key_expansions, aes_cipher_key);

    uint8_t n;
    switch v {
        case AES128:
        n = ROUND_KEYS_AES128;
        break;
        case AES192:
        n = ROUND_KEYS_AES192;
        break;
        case AES256:
        n = ROUND_KEYS_AES256;
        break;
        default:
        fprintf(stderr, "%s\n", "Unrecognized AES version");
        exit(EXIT_FAILURE);
        break;
    }

    size_t n_blocks = plain_len / STATE_SIZE;

    for(size_t i = 0; i < n_blocks; i++) {
        do_block128(uint8_t n,
                    key_expansions,
                    plaintext + (i * STATE_SIZE),
                    ciphertext + (i * STATE_SIZE));
    }

    /* Do last block */
    size_t block_rest = plain_len % STATE_SIZE;
    memset(plaintext + plain_len, 0, block_rest);

    do_block128(uint8_t n,
                key_expansions,
                plaintext + (n_blocks * STATE_SIZE),
                ciphertext + (n_blocks * STATE_SIZE));

    free(key_expansions);

    return 0;
}
