# Polynomial Secret Reconstruction (C++)

This program reads input in JSON format, reconstructs a degree `k-1` polynomial using Lagrange interpolation, and outputs the secret value `f(0)`.

It is designed to work with the given assignment test cases.

---

## Build Instructions

Make sure you have **g++** installed (C++17 or newer).

```bash
g++ -std=c++17 secret.cpp -o secret