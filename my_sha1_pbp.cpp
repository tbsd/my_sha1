#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

uint32_t leftrotate(uint32_t set, unsigned count);
void sha1(ifstream *message);
void process_block(const vector<uint32_t> *block);
vector<uint32_t> bits_to_block(const char *array, size_t size);
vector<uint32_t> extend_block(const vector<uint32_t> *block);

uint32_t A = 0x67452301,
         B = 0xEFCDAB89,
         C = 0x98BADCFE,
         D = 0x10325476,
         E = 0xC3D2E1F0;

int main(int argc, char **argv) {
  const char *tmp_f_name = "str_tmp";
  ifstream message;
  if (argc == 2) {
    message = ifstream(argv[1], std::ios::binary);
    if (!message.good()) {
      cerr << "File not found." << endl;
      return 1;
    }
  } else if (argc == 3 && string(argv[1]) == "-s"){
    ofstream str_tmp(tmp_f_name,
        ofstream::out | ofstream::trunc | ofstream::binary);
    str_tmp << argv[2];
    str_tmp.close();
    message = ifstream(tmp_f_name, std::ios::binary);
  } else {
    cerr << "Usage: \nmy_sha1 file\nmy_sha1 -s 'string'" << endl;
    return 1;
  }

  sha1(&message);

  remove(tmp_f_name);
  printf("%08x%08x%08x%08x%08x\n", A, B, C, D, E);
  return 0;
}

uint32_t leftrotate(uint32_t set, unsigned count) {
  unsigned shift = count % 32;
  return (set << shift) | (set >> (32-shift));
}

void sha1 (ifstream *message) {
  uint64_t message_size = 0;
  size_t block_size = 64;
  char buff[block_size];
  while (true) {
    message->read(buff, block_size);
    size_t count = message->gcount();
    if (count == block_size) {
      message_size += count;
      vector<uint32_t> block = bits_to_block(buff, block_size);
      block = extend_block(&block);
      process_block(&block);
    } else {
      message_size += count;
      buff[count] = 0b10000000;
      vector<uint32_t> block = bits_to_block(buff, count + 1);
      if (count  < 48) {
         block[14] = message_size * 8 >> 32;
         block[15] = message_size * 8;
         block = extend_block(&block);
         process_block(&block);
      } else {
        block = extend_block(&block);
        process_block(&block);
        vector<uint32_t> additional(16, 0);
        additional[14] = message_size * 8 >> 32;
        additional[15] = message_size * 8;
        additional = extend_block(&additional);
        process_block(&additional);
      }
      break;
    }
  }
  return;
}

vector<uint32_t> extend_block(const vector<uint32_t> *block) {
  vector<uint32_t> extended(80);
  for (size_t i = 0; i < 16; ++i)
    extended[i] = (*block)[i];
  for (size_t i = 16; i < 80; ++i) 
    extended[i] = leftrotate(extended[i - 3] ^
        extended[i - 8] ^ extended[i - 14] ^ extended[i - 16], 1);
  return extended;
}

vector<uint32_t> bits_to_block(const char *array, size_t size) {
  vector<uint32_t> result(16, 0);
  size_t trailing_byts = size % 4;
  for (size_t i = 0; i < size / 4; ++i) 
    result[i] = (((uint8_t)array[i * 4] << 24) |
        ((uint8_t)array[i * 4 + 1] << 16 ) | ((uint8_t)array[i * 4 + 2] << 8) |
        ((uint8_t)array[i * 4 + 3]));
  for (size_t i = 0; i < trailing_byts; ++i) {
    size_t shift = (3 - i) * 8;
    result[size / 4] |= (((uint8_t)array[size - trailing_byts  + i]) << shift);
  }
  return result;
}

void process_block(const vector<uint32_t> *block) {
  uint32_t a = A, b = B, c = C, d = D, e = E;
  for (size_t i = 0; i < 80; ++i) {
    uint32_t f, k, tmp;
    if (i <= 19) {
      f = (b & c) | ((~b) & d);
      k = 0x5A827999;
    } else if (i <= 39) {
      f = b ^ c ^ d;
      k = 0x6ED9EBA1;
    } else if (i <= 59){
      f = (b & c) | (b & d) | (c & d);
      k = 0x8F1BBCDC;
    } else {
      f = b ^ c ^ d;
      k = 0xCA62C1D6;
    }
    tmp = leftrotate(a, 5) + f + e + (*block)[i] + k;
    e = d;
    d = c;
    c = leftrotate(b, 30);
    b = a;
    a = tmp;
  }
  A += a;
  B += b;
  C += c;
  D += d;
  E += e;
  return;
}
