
- Cài ubuntu
   ```bash
   wsl --unregister Ubuntu
   wsl --install
   ```

- Cài C++
   ```bash
   sudo apt update && sudo apt upgrade -y
   sudo apt install build-essential -y
   g++ --version
   sudo apt install libssl-dev -y
   ```

- Cài libsecp256k1
   ```bash
   sudo apt update
   sudo apt install autoconf libtool pkg-config build-essential
   git clone https://github.com/bitcoin-core/secp256k1.git
   cd secp256k1
   ./autogen.sh
   ./configure --enable-module-ecdh --enable-module-recovery
   make
   sudo make install
   sudo ldconfig
   ```
   
- Copy 3 file và code (Lưu Ctrl+O, enter, Ctrl+x)
   ```bash
   cd
   nano evm_benchmark.cpp
   nano sha3.h
   nano sha3.c
   ```
  
- Biên dịch và và chạy 
   ```bash
   g++ evm_benchmark.cpp sha3.c -o evm_benchmark \
     -O3 -march=native -fopenmp \
     -lsecp256k1
  ./evm_benchmark
   ```
  
  
