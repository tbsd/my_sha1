#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

uint32_t leftrotate(uint32_t set, unsigned count);
void sha1 (const vector<uint8_t> *message);
vector<uint32_t> add_extra_bits(const vector<uint8_t> *message);
void add_length(uint64_t length, vector<uint32_t> *dest);
void process_block(vector<uint32_t>::iterator begin,
    vector<uint32_t>::iterator end);

uint32_t A = 0x67452301,
         B = 0xEFCDAB89,
         C = 0x98BADCFE,
         D = 0x10325476,
         E = 0xC3D2E1F0;

int main(int argc, char **argv) {
  vector<uint8_t> message;
  if (argc == 2) {
    ifstream input( argv[1], std::ios::binary );
    if (!input.good()) {
      cerr << "File not found." << endl;
      return 1;
    }
    message = vector<uint8_t>(istreambuf_iterator<char>(input),
        (istreambuf_iterator<char>()));
  } else if (argc == 3 && string(argv[1]) == "-s"){
    string s(argv[2]);
    message = vector<uint8_t>(s.begin(), s.end());
  } else {
    cerr << "Usage: \nmy_sha1 file\nmy_sha1 -s 'string'" << endl;
    return 1;
  }

  sha1(&message);

  printf("%08x%08x%08x%08x%08x\n", A, B, C, D, E);
  return 0;
}

uint32_t leftrotate(uint32_t set, unsigned count) {
  unsigned shift = count % 32;
  return (set << shift) | (set >> (32-shift));
}

void sha1 (const vector<uint8_t> *message) {
  vector<uint32_t> exp_message = add_extra_bits(message);
  add_length(message->size() * 8, &exp_message);
  for (auto i = exp_message.begin(); i != exp_message.end(); i += 16) {
    process_block(i, i + 16);
  }
  return;
}

vector<uint32_t> add_extra_bits(const vector<uint8_t> *message) {
  vector<uint8_t> tmp(*message);
  tmp.push_back(0b10000000);
  tmp.insert(tmp.cend(), (64 - (int)tmp.size() % 64 + 56) % 64, 0);
  vector<uint32_t> result;
  for (auto i = tmp.begin(); i != tmp.end(); i += 4) {
    result.push_back(((*i << 24) | ((*(i + 1)) << 16 ) | ((*(i + 2)) << 8) |
          (*(i + 3))));
  }
  return result;
}

void add_length(uint64_t length, vector<uint32_t> *dest) {
  dest->insert(dest->cend(), {(uint32_t)(length >> 32), (uint32_t)(length)});
  return;
}

void process_block(vector<uint32_t>::iterator begin,
    vector<uint32_t>::iterator end) {
  uint32_t a = A, b = B, c = C, d = D, e = E;
  vector<uint32_t> block(begin, end);
  for (size_t i = 16; i < 80; ++i) {
    block.push_back(leftrotate(block[i - 3] ^ block[i - 8] ^ block[i - 14] ^
          block[i - 16], 1));
  }
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
    tmp = leftrotate(a, 5) + f + e + block[i] + k;
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
