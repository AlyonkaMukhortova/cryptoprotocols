#include "protocol_funcs.h"


unsigned char* text1 = (unsigned char*)"THIS IS THE TEXT FOR DECRYPT (ZERO FANTAZY ENTERED THE CHAT)";
int mode_key_bytes[4] = {24, 16, 24, 32};
int mode_iv_bytes[4] = {8, 16, 16, 16};

void(*fhmac[2])(unsigned char *, size_t, unsigned char *, size_t, unsigned char *) = {hmac_md5, hmac_sha1};

static void (*fdecipher[4])(unsigned char *, size_t,
			       unsigned char *, unsigned char *,
				unsigned char *) = {des3_cbc_decrypt, aes128_cbc_decrypt,
        aes192_cbc_decrypt, aes256_cbc_decrypt};


static void (*fcipher[4])(unsigned char *, size_t,
			       unsigned char *, unsigned char *,
				unsigned char *) = {des3_cbc_encrypt, aes128_cbc_encrypt,
        aes192_cbc_encrypt, aes256_cbc_encrypt};



static void des3_cbc_decrypt(unsigned char *in, size_t in_len,
			       unsigned char *iv, unsigned char *key, unsigned char *out) {
	printf("In decrypt\n");
	DES_cblock key1, key2, key3;
	DES_key_schedule ks1, ks2, ks3;
	memcpy(key1, key, 8);
	memcpy(key2, key + 8, 8);
	memcpy(key3, key + 16, 8);

	DES_set_key((DES_cblock *) key1, &ks1);
	DES_set_key((DES_cblock *) key2, &ks2);
	DES_set_key((DES_cblock *) key3, &ks3);

	DES_ede3_cbc_encrypt(in, out, in_len, &ks1, &ks2, &ks3, (DES_cblock *) iv, DES_DECRYPT);
}


static void aes128_cbc_decrypt(unsigned char *in, size_t in_len,
				    unsigned char *iv, unsigned char *key, unsigned char *out) {
	AES_KEY akey;
	AES_set_decrypt_key(key, 128, &akey);
	AES_cbc_encrypt(in, out, in_len, &akey, iv, AES_DECRYPT);
}


static void aes192_cbc_decrypt(unsigned char *in, size_t in_len,
				    unsigned char *iv, unsigned char *key, unsigned char *out) {
	AES_KEY akey;
	AES_set_decrypt_key(key, 192, &akey);
	AES_cbc_encrypt(in, out, in_len, &akey, iv, AES_DECRYPT);
}


static void aes256_cbc_decrypt(unsigned char *in, size_t in_len,
				    unsigned char *iv, unsigned char *key,
				    unsigned char *out) {
	AES_KEY akey;
	AES_set_decrypt_key(key, 256, &akey);
	AES_cbc_encrypt(in, out, in_len, &akey, iv, AES_DECRYPT);
}


static void des3_cbc_encrypt(unsigned char *in, size_t in_len,
			       unsigned char *iv, unsigned char *key, unsigned char *out) {
	DES_cblock key1, key2, key3;
	DES_key_schedule ks1, ks2, ks3;
	memcpy(key1, key, 8);
	memcpy(key2, key + 8, 8);
	memcpy(key3, key + 16, 8);

	DES_set_key((DES_cblock *) key1, &ks1);
	DES_set_key((DES_cblock *) key2, &ks2);
	DES_set_key((DES_cblock *) key3, &ks3);

	DES_ede3_cbc_encrypt(in, out, in_len, &ks1, &ks2, &ks3, (DES_cblock *) iv, DES_ENCRYPT);
}


static void aes128_cbc_encrypt(unsigned char *in, size_t in_len,
				    unsigned char *iv, unsigned char *key, unsigned char *out) {
	AES_KEY akey;
	AES_set_encrypt_key(key, 128, &akey);
	AES_cbc_encrypt(in, out, in_len, &akey, iv, AES_ENCRYPT);
}


static void aes192_cbc_encrypt(unsigned char *in, size_t in_len,
				    unsigned char *iv, unsigned char *key, unsigned char *out) {
	AES_KEY akey;
	AES_set_encrypt_key(key, 192, &akey);
	AES_cbc_encrypt(in, out, in_len, &akey, iv, AES_ENCRYPT);
}


static void aes256_cbc_encrypt(unsigned char *in, size_t in_len,
				    unsigned char *iv, unsigned char *key,
				    unsigned char *out) {
	AES_KEY akey;
	AES_set_encrypt_key(key, 256, &akey);
	AES_cbc_encrypt(in, out, in_len, &akey, iv, AES_ENCRYPT);
}


void hmac_md5(unsigned char *text,
    size_t text_len, unsigned char *key, size_t key_len, unsigned char *md)
{
	static unsigned char m[16];
	MD5_CTX context;
	unsigned char k_ipad[65];
	unsigned char k_opad[65];
	unsigned char tk[16];
	int i;

	if (md == NULL)
		md = m;


	if (key_len > 64) {
		MD5_CTX tctx;

		MD5_Init(&tctx);
		MD5_Update(&tctx, key, key_len);
		MD5_Final(tk, &tctx);

		key = tk;
		key_len = 16;
	}

	memset(k_ipad, 0x36, sizeof k_ipad);
	memset(k_opad, 0x5c, sizeof k_opad);

	for (i = 0; i < key_len; i++) {
		k_ipad[i] ^= key[i];
		k_opad[i] ^= key[i];
	}

	MD5_Init(&context);
	MD5_Update(&context, k_ipad, 64);
	MD5_Update(&context, text, text_len);
	MD5_Final(md, &context);
	MD5_Init(&context);
	MD5_Update(&context, k_opad, 64);
	MD5_Update(&context, md, 16);
	MD5_Final(md, &context);

	//return md;
}


void hmac_sha1(const unsigned char *text,
    size_t text_len, const unsigned char *key, size_t key_len,
    unsigned char *md)
{
	static unsigned char m[20];
	SHA_CTX context;
	unsigned char k_ipad[65];
	unsigned char k_opad[65];
	unsigned char tk[20];
	int i;

	if (md == NULL)
		md = m;


	if (key_len > 64) {
		SHA_CTX tctx;

		SHA1_Init(&tctx);
		SHA1_Update(&tctx, key, key_len);
		SHA1_Final(tk, &tctx);
		key = tk;
		key_len = 20;
	}



	memset(k_ipad, 0x36, sizeof k_ipad);
	memset(k_opad, 0x5c, sizeof k_opad);


	for (i = 0; i < key_len; i++) {
		k_ipad[i] ^= key[i];
		k_opad[i] ^= key[i];
	}

	SHA1_Init(&context);
	SHA1_Update(&context, k_ipad, 64);
	SHA1_Update(&context, text, text_len);
	SHA1_Final(md, &context);

	SHA1_Init(&context);
	SHA1_Update(&context, k_opad, 64);
	SHA1_Update(&context, md, 20);
	SHA1_Final(md, &context);
	//return md;
}


unsigned char* generate_rand(int length){
  unsigned char* res = (unsigned char*)malloc(length * sizeof(unsigned char));
  unsigned char* r = res;
	srand(time(NULL));
  for (int i = 0; i < length; i++){

    res[i] = (rand() * rand()%(256));

    r++;
  }
  return res;
}





void generator (int mode_hash, int mode_cipher, unsigned char* password) {
  unsigned char* key = (unsigned char*)calloc(1, sizeof(unsigned char));
  unsigned char* hmac = (unsigned char*)malloc(16);
  unsigned char* new_text = (unsigned char*)calloc(8, sizeof(unsigned char));
  unsigned char* cipher_text = (unsigned char*)calloc(8, sizeof(unsigned char));
  unsigned char* iv1 = (unsigned char*)calloc(1, sizeof(unsigned char));
  unsigned char* nonce = generate_rand(64);
	unsigned char* nonce1 = (unsigned char*)calloc(1, sizeof(unsigned char));
  unsigned char* iv = generate_rand(mode_iv_bytes[mode_cipher]);
  iv1 = concatenate(iv1, iv, 0, mode_iv_bytes[mode_cipher]);
	nonce1 = concatenate(nonce1, nonce, 0, 64);
  new_text = concatenate(new_text, text1, 8, strlen((char*)text1));

  fhmac[mode_hash](password, 4, nonce, 64, hmac);

	int key_len = 0;
  key = concatenate(key, hmac, key_len, 16);
	key_len += 16;
  while(key_len < mode_key_bytes[mode_cipher]) {
    fhmac[mode_hash](password, 4, hmac, 16, hmac);
    key = concatenate(key, hmac, key_len, 16);
		key_len += 16;
  }
  if (key_len > mode_key_bytes[mode_cipher]) {
    key[mode_key_bytes[mode_cipher]] = '\0';
  }
	for(int i = 0; i < key_len; i++) {
	 printf("%02x", key[i]);
	}
	printf("\nKEY\n");
	for(int i = 0; i < 16; i++) {
		printf("%02x", iv1[i]);
	}
	printf("\nIV\n");
	for(int i = 0; i < 16; i++) {
		printf("%02x", iv[i]);
	}
	printf("\nIV\n");

  char* file_name = construct_fname(mode_hash, mode_cipher, password);
	FILE* fd;

  fd = fopen(file_name, "w+b");
  char* str = "ENC";
  fwrite(str, sizeof(char), 3, fd);
  fwrite(&mode_hash, sizeof(unsigned char), 1, fd);
  fwrite(&mode_cipher, sizeof(unsigned char), 1, fd);

  fwrite(nonce, sizeof(unsigned char), 64, fd);

  fwrite(iv, sizeof(unsigned char), mode_iv_bytes[mode_cipher], fd);
	for(int i = 0; i < TEXT_LEN + 8; i++) {
		printf("%02x", new_text[i]);
	}
	printf("\nOPEN TEXT\n");
	fcipher[mode_cipher](new_text, TEXT_LEN + 8, iv, key, cipher_text);
	for(int i = 0; i < TEXT_LEN + 8; i++) {
		printf("%02x", cipher_text[i]);
	}
	printf("\nCIPHER TEXT\n");
	for(int i = 0; i < 16; i++) {
		printf("%02x", iv[i]);
	}
	printf("\nIV\n");
	for(int i = 0; i < 16; i++) {
		printf("%02x", iv1[i]);
	}
	printf("\nIV\n");
  //to_file(mode_hash, mode_cipher, nonce1, iv1, file_name);
	int len = strlen((char*)cipher_text);

	fwrite(cipher_text, sizeof(unsigned char), TEXT_LEN + 8, fd);
	fclose(fd);
	/*FILE* fd;
	fd = fopen(file_name, "w+b");
	fseek(fd, 0l, SEEK_END);
	fwrite(cipher_text, sizeof(unsigned char), TEXT_LEN + 8, fd);
	fclose(fd);*/
	free(key);
	free(hmac);
	free(new_text);
	free(cipher_text);
	//free(iv1);
	free(iv);
	//free(password);
}



void to_file (int mode_hash, int mode_cipher, unsigned char* nonce,
              unsigned char* iv, char* file_name)
{
  FILE* fd;

  fd = fopen(file_name, "w+b");
  char* str = "ENC";
  fwrite(str, sizeof(char), 3, fd);
  fwrite(&mode_hash, sizeof(unsigned char), 1, fd);
  fwrite(&mode_cipher, sizeof(unsigned char), 1, fd);

  fwrite(nonce, sizeof(unsigned char), 64, fd);

  fwrite(iv, sizeof(unsigned char), mode_iv_bytes[mode_cipher], fd);
  //int len = strlen((char*)cipher_text);

  //fwrite(cipher_text, sizeof(unsigned char), TEXT_LEN + 8, fd);
  fclose(fd);
}



void hexchar (unsigned int num, unsigned char** str) {
  unsigned char s= 0;
  *str = (unsigned char*)malloc(4);
  for (int i = 0; i < 4; i++) {
    (*str)[3 - i] = num % 256;
    num = num / 256;
  }
}




int decipher (int mode_hash, int mode_cipher, unsigned char* nonce,
   unsigned char* iv, unsigned char* text, unsigned char* password, int text_len) {
     unsigned char* key = (unsigned char*)calloc(1, sizeof(unsigned char));
     unsigned char* hmac = (unsigned char*)malloc(16);
     unsigned char* open_text = (unsigned char*)calloc(text_len, sizeof(unsigned char));
		 unsigned char* iv1 = (unsigned char*)calloc(1, sizeof(unsigned char));
	   iv1 = concatenate(iv1, iv, 0, mode_iv_bytes[mode_cipher]);

		 int key_len = 0;
		 fhmac[mode_hash](password, 4, nonce, 64, hmac);
	   key = concatenate(key, hmac, 0, 16);
		 key_len+=16;
	   while(key_len < mode_key_bytes[mode_cipher]) {
	     fhmac[mode_hash](password, 4, hmac, 16, hmac);
	     key = concatenate(key, hmac, key_len, 16);
			 key_len+=16;
	   }
	   if (key_len > mode_key_bytes[mode_cipher]) {
	     key[mode_key_bytes[mode_cipher]] = '\0';
	   }
		 /*for(int i = 0; i < key_len; i++) {
			printf("%02x", key[i]);
		 }
		 printf("\nKEY\n");
		 for(int i = 0; i < 16; i++) {
			 printf("%02x", iv1[i]);
		 }
		 printf("\nIV\n");*/
     //des3_cbc_decrypt(text, text_len, iv, key, open_text);
     fdecipher[mode_cipher](text, text_len, iv1, key, open_text);
		 /*for(int i = 0; i < text_len; i++) {
			printf("%02x", text[i]);
		 }
		 printf("\nTEXT\n");*/
		 /*for(int i = 0; i < text_len; i++) {
	 		printf("%02x", open_text[i]);
	 	}
	 	printf("\nOPEN TEXT\n");*/


		free(iv1);
		free(key);
		free(hmac);
     for (int i = 0; i < 8; i++) {
			 //printf("%02x\n", open_text[i]);
       if (open_text[i] != 0) {
         return WRONG_VALUE;
       }
     }
		 free(open_text);
     return 0;
}


int cracker (char* file_name, int v) {
  unsigned char* nonce = (unsigned char*)calloc(64, sizeof(unsigned char));
  unsigned char* iv = NULL;
  unsigned char* text = NULL;
  int mode_hash = 0, mode_cipher = 0;
  unsigned char* password = (unsigned char*)malloc(4 * sizeof(unsigned char));
  int a = -1;
  unsigned int result = 0;
  password[3] = 89;
  int text_len = 0;
  analyse_file (file_name, &mode_hash, &mode_cipher, &nonce, &iv, &text, &text_len);
	printf("hmac_%s, %s\n", hash_modes[mode_hash], cipher_modes[mode_cipher]);
	printf("NONCE: ");
	for(int i = 0; i < 64; i++) {
		printf("%02x", nonce[i]);
	}
	printf("\nIV:");
	for(int i = 0; i < mode_iv_bytes[mode_cipher]; i++) {
		printf("%02x", iv[i]);
	}
	printf("\nCT:");
	for(int i = 0; i < text_len; i++) {
		printf("%02x", text[i]);
	}
	printf("\nStart cracking\n");
	clock_t first, last;
	first = clock();
	if (v == 0){
		printf("Current:   %08x - %08x\n", 0x0, 0xffff);
	}
  for (unsigned int i = 0x00000000; i < 0xffffffff; i++) {
		if(!(i%0xffff) & i != 0 & v == 0){
			last = clock();
			printf("Current:   %08x - %08x, speed  %f   per ms\n", i, i + 0xffff,
						0x1000 / (double)((last - first) * 1000 /CLOCKS_PER_SEC));
			first = clock();
		}
    //printf("\n%08x\n", i);
    hexchar(i, &password);                                             //works but like a shit
    a = decipher(mode_hash, mode_cipher, nonce, iv, text, password, text_len);

    if(a == 0){
      result = i;
      break;
    }
  }
  printf("Found:   %08x\n", result);
  return result;
}


void analyse_file (char* file_name, int* mode_hash, int* mode_cipher, unsigned char** nonce,
   unsigned char** iv, unsigned char** text, int* text_len) {
  FILE* fd;
  int offset = 0, size = 0;
  fd = fopen(file_name, "rb");
  fseek(fd, 3, SEEK_SET);
  fread(mode_hash, sizeof(unsigned char), 1, fd);
  fread(mode_cipher, sizeof(unsigned char), 1, fd);
  fread(*nonce, sizeof(unsigned char), 64, fd);
  *iv = (unsigned char*)malloc(mode_iv_bytes[*mode_cipher]);
  fread(*iv, sizeof(unsigned char), mode_iv_bytes[*mode_cipher], fd);
  offset = ftell(fd);
  fseek(fd, 0L, SEEK_END);
  size = ftell(fd);
  fseek(fd, offset, SEEK_SET);
  *text = (unsigned char*)malloc(size - offset);
  fread(*text, sizeof(unsigned char), size - offset, fd);
  *text_len = size - offset;
}


int checker_symbol(unsigned char symb, int position){
  char* first = "ENC";
  if (position < 3){
    return symb == first[position] ? 0 : WRONG_VALUE;
  }
  else if (position == 3){
    return (symb == 0 || symb == 1) ? 0 : WRONG_VALUE;
  }
  else if (position == 4){
    return (symb >= 0 && symb <= 4) ? 0 : WRONG_VALUE;
  }
  else{
    return (position < 5 + 64 + 16 + 4096) ? 0 : WRONG_VALUE;
  }
}


int file_checker(char* file_name){
  FILE* fd;
  int size = 0, err;
  fd = fopen(file_name, "r");
  if (fd == NULL){
    return WRONG_VALUE;
  }
  unsigned char symb = 0;
  fseek(fd, 0L, SEEK_END);
  size = ftell(fd);
  fseek(fd, 0, SEEK_SET);
  for (int i = 0; i < size; i++) {
    fread(&symb, sizeof(unsigned char), 1, fd);
    err = checker_symbol(symb, i);
    if (err == WRONG_VALUE){
      return WRONG_VALUE;
    }
  }
  return 0;
}

