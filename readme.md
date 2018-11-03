my_sha1.cpp
-
This implementation directly follows the algorithm and does not contain any significant optimizations.

my_sha1_pbp.cpp
-
This is modified version so it reads input piece-by-piece instead of loading whole file at once. So it won't eat your RAM when it is used with large file. 

Anyway, both are slower compared to well-optimized implementations.
