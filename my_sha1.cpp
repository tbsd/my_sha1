#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <bitset>
using namespace std;

uint32_t leftrotate(uint32_t set, unsigned count);
vector<uint32_t> sha1 (vector<uint8_t> &message);

int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "Usage: my_sha1 file" << endl;
    return 1;
  }
  ifstream input( argv[1], std::ios::binary );
  vector<uint8_t> message(istreambuf_iterator<char>(input),
      (istreambuf_iterator<char>()));
  vector<uint32_t> result = sha1(message);
  return 0;
}

uint32_t leftrotate(uint32_t set, unsigned count) {
  unsigned shift = count % 32;
  return (set << shift) | (set >> (32-shift));
}

vector<uint32_t> sha1 (vector<uint8_t> &message) {
  vector<uint32_t> exp_message;
  add_extra_bits(&message, &exp_message);
  add_length(&message, &exp_message);
  uint32_t A = 0x67452301, B = 0xEFCDAB89, C = 0x98BADCFE, D = 0x10325476, 
           E = 0xC3D2E1F0;
  for (auto i = exp_message.begin(); i != exp_message.end(); i += 16) {
    round(i, i + 16, &A, &B, &C, &D, &E)
  }
  return {A, B, C, D, E};
}
