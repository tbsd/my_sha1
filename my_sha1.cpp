#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <bitset>
using namespace std;

uint32_t leftrotate(uint32_t set, unsigned count);

int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "Usage: my_sha1 file" << endl;
    return 1;
  }
  // ifstream input( argv[1], std::ios::binary );
  // vector<uint8_t> message(istreambuf_iterator<char>(input),
      // (istreambuf_iterator<char>()));
  
  return 0;
}

uint32_t leftrotate(uint32_t set, unsigned count) {
  unsigned shift = count % 32;
  return (set << shift) | (set >> (32-shift));
}
