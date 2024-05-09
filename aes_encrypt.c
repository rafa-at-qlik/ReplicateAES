#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include "ar_addon.h"
#include "ar_addon_transformation.h"

static void trans_aes_encrypt(sqlite3_context *context, int argc, sqlite3_value **argv);

AR_AO_EXPORTED int ar_addon_init(AR_ADDON_CONTEXT *context)
{
        AR_AO_TRANSFORMATION_DEF *transdef = NULL;

        AR_AO_INIT(context);

        transdef = GET_AR_AO_TRANSFORMATION_DEF();
        transdef->displayName = "aes_encrypt(X, Y)";
        transdef->functionName = "aes_encrypt";
        transdef->description = "aes_encrypt encrypts string Y with key X";
        transdef->func = trans_aes_encrypt;
        transdef->nArgs = 2;
        AR_AO_REGISRATION->register_user_defined_transformation(transdef);

        return 0;
}


//Error handling
//Use sqlite3_result_error_code function to return an error.
//Three codes could have been used:
// SQLLITE_ERROR (1) - causes Replicate to skip the problematic data record
// SQLITE_CALLBACK_FATAL_ERROR (251) - causes Replicate to stop the task immediately with a fatal error
// SQLITE_CALLBACK_RECOVERABLE_ERROR (250) - causes Replicate to reattach the target endpoint or to stop the task with a recoverable error 
// Any other error code will be proceeded as SQLLITE_ERROR. 
// sqlite3_result_error function could have been used to return an error message. If this function is used without sqlite3_result_error_code, SQLLITE_ERROR is returned.   
// Function to encode data to base64
char* base64_encode(const unsigned char *input, int length) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, input, length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);

    char *output = (char *)malloc(bufferPtr->length + 1);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    memcpy(output, bufferPtr->data, bufferPtr->length);
    output[bufferPtr->length] = '\0';

    BIO_free_all(bio);
    return output;
}

// Function to encrypt plaintext using AES ECB mode with PKCS#7 padding
char* encrypt_aes_ecb_pkcs7(const char *private_key, const char *plaintext) {
    const int key_length = strlen(private_key);
    const int plaintext_length = strlen(plaintext);

    // Initialize the cipher context for ECB
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        fprintf(stderr, "EVP_CIPHER_CTX_new failed\n");
        return NULL;
    }

    // Initialize encryption operation for ECB
    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, (const unsigned char *)private_key, NULL) != 1) {
        fprintf(stderr, "EVP_EncryptInit_ex failed\n");
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }

    // Calculate the padded length
    const int padded_length = ((plaintext_length + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    // Allocate memory for padded plaintext
    unsigned char *padded_plaintext = (unsigned char *)malloc(padded_length);
    if (padded_plaintext == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }
    memcpy(padded_plaintext, plaintext, plaintext_length);
    memset(padded_plaintext + plaintext_length, AES_BLOCK_SIZE - plaintext_length % AES_BLOCK_SIZE, AES_BLOCK_SIZE - plaintext_length % AES_BLOCK_SIZE);

    // Allocate memory for ciphertext
    unsigned char *ciphertext = (unsigned char *)malloc(padded_length);
    if (ciphertext == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(padded_plaintext);
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }

    int ciphertext_length;

    // Perform encryption for ECB
    if (EVP_EncryptUpdate(ctx, ciphertext, &ciphertext_length, padded_plaintext, padded_length) != 1) {
        fprintf(stderr, "EVP_EncryptUpdate failed\n");
        free(padded_plaintext);
        free(ciphertext);
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }

    // Clean up
    EVP_CIPHER_CTX_free(ctx);
    free(padded_plaintext);

    // Encode the ciphertext in base64
    char *base64_ciphertext = base64_encode(ciphertext, ciphertext_length);

    // Free the memory allocated for ciphertext
    free(ciphertext);

    return base64_ciphertext;
}

static void trans_aes_encrypt(sqlite3_context *context, int argc, sqlite3_value **argv)
{
        AR_AO_LOG->log_trace("enter trans_aes_encrypt");
        if (argc >= 2) 
        { // you should check that all the parameters declared in the function definition are provided
                char *private_key = (char *)AR_AO_SQLITE->sqlite3_value_text(argv[0]);
                char *plaintext = (char *)AR_AO_SQLITE->sqlite3_value_text(argv[1]);
                char pRes[2000] = {0};
                
                char *encrypted_text = encrypt_aes_ecb_pkcs7(private_key, plaintext);
                strcpy(pRes, encrypted_text);

                AR_AO_SQLITE->sqlite3_result_text(context, pRes, -1, SQLITE_TRANSIENT);
                AR_AO_LOG->log_trace("Before %s", "return");
                
        }
        else
        { // should not occur but only if you do not declare the function correct
        AR_AO_LOG->log_error("The sqlite addon function trans_aes_encrypt received an incorrect (%d instead of at least 2) number of parameters", argc);
                AR_AO_SQLITE->sqlite3_result_error(context, "incorrect parameter list", -1); 
                AR_AO_SQLITE->sqlite3_result_error_code(context, SQLITE_CALLBACK_FATAL_ERROR); 
        }

        AR_AO_LOG->log_trace("leave trans_aes_encrypt");
}