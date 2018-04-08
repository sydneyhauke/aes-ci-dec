#ifndef __AES_H
#define __AES_H

#include <stdint.h>

#define ROUND_KEY_SIZE 16 // In bytes (126 bits)
#define STATE_SIZE ROUND_KEY_SIZE
#define ROUND_KEYS_AES128 10
#define ROUND_KEYS_AES192 12
#define ROUND_KEYS_AES256 14
#define MAX_ROUND_KEYS ROUND_KEYS_AES256

enum AES_VERSION {
    AES128,
    AES192,
    AES256
};

static void
key_expansions(enum AES_VERSION v,
               uint8_t **expendend_key,
               const uint8_t *aes_cipher_key);

static void
add_round_key(const uint8_t *roundkey, uint8_t *state);

static void
sub_bytes(uint8_t *state);

static void
shift_rows(uint8_t *state);

static void
mix_columns(uint8_t *col);

static void
do_block128(uint8_t n_rounds,
            const uint8_t *key_expansions,
            const uint8_t *plaintext,
            uint8_t *ciphertext);

int aes_cipher(enum AES_VERSION v,
               const uint8_t *aes_cipher_key,
               uint8_t *plaintext,
               size_t len,
               uint8_t *ciphertext);

#endif
