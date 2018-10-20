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
vector<uint32_t> sha1 (vector<uint8_t> &message);
string vec_to_str(vector<uint32_t> &vec);
vector<uint32_t> add_extra_bits(const vector<uint8_t> *message);
void add_length(uint64_t length, vector<uint32_t> *dest);
void round(vector<uint32_t>::iterator begin, vector<uint32_t>::iterator end);

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

  vector<uint32_t> result = sha1(message);
  cout << vec_to_str(result) << endl;
  return 0;
}

uint32_t leftrotate(uint32_t set, unsigned count) {
  unsigned shift = count % 32;
  return (set << shift) | (set >> (32-shift));
}

string vec_to_str(vector<uint32_t> &vec) {
  stringstream hex_str;
  for (auto i : vec)
    hex_str << hex << i;
  return hex_str.str();
}

vector<uint32_t> sha1 (vector<uint8_t> &message) {
  vector<uint32_t> exp_message = add_extra_bits(&message);
  add_length(message.size(), &exp_message);
  for (auto i = exp_message.begin(); i != exp_message.end(); i += 16) {
    round(i, i + 16)
  }
  return {A, B, C, D, E};
}

