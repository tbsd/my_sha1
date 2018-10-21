#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <bitset>
#include <sstream>
using namespace std;

uint32_t leftrotate(uint32_t set, unsigned count);
void sha1 (const vector<uint8_t> *message);
vector<uint32_t> add_extra_bits(const vector<uint8_t> *message);
void add_length(uint64_t length, vector<uint32_t> *dest);
void process_block(vector<uint32_t>::iterator begin, vector<uint32_t>::iterator end);

uint32_t A = 0x67452301, B = 0xEFCDAB89, C = 0x98BADCFE, D = 0x10325476, 
         E = 0xC3D2E1F0;

int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "Usage: my_sha1 file" << endl;
    return 1;
  }
  ifstream input( argv[1], std::ios::binary );
  vector<uint8_t> message(istreambuf_iterator<char>(input),
      (istreambuf_iterator<char>()));

  sha1(&message);

  stringstream hex_str;
  hex_str << hex << A << B << C << D << E;
  cout << hex_str.str() << endl;

  return 0;
}

uint32_t leftrotate(uint32_t set, unsigned count) {
  unsigned shift = count % 32;
  return (set << shift) | (set >> (32-shift));
}

void sha1 (const vector<uint8_t> *message) {
  vector<uint32_t> exp_message = add_extra_bits(message);
  add_length(message->size(), &exp_message);
  for (auto i = exp_message.begin(); i != exp_message.end(); i += 16) {
    process_block(i, i + 16);
  }
  return;
}

vector<uint32_t> add_extra_bits(const vector<uint8_t> *message) {
  vector<uint8_t> tmp(*message);
  tmp.push_back(0b10000000);
  while (tmp.size() % 64 != 56) {
    tmp.push_back(0);
  }
  vector<uint32_t> result;
  for (auto i = tmp.begin(); i != tmp.end(); i += 4) {
    result.push_back(((*i << 24) | ((*(i + 1)) << 16 ) | ((*(i + 2)) << 8) | (*(i + 3))));
  }
  return result;
}

void add_length(uint64_t length, vector<uint32_t> *dest) {
  dest->push_back(length >> 32);
  dest->push_back(length);
  return;
}
