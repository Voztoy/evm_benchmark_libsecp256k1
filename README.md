# Cài ubuntu
   ```bash
   wsl --unregister Ubuntu
   wsl --install
   ```

# Cài C++
   ```bash
   sudo apt update && sudo apt upgrade -y
   sudo apt install build-essential -y
   g++ --version
   sudo apt install libssl-dev -y
   ```

# Cài libsecp256k1
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
   
# Copy 4 file và code (Lưu Ctrl+O, enter, Ctrl+x)
   ```bash
   cd
   nano evm_benchmark.cpp
   nano vanity_finder.cpp
   nano keccak.h
   nano keccak.c
   ```
  
# Biên dịch evm_benchmark.cpp
   ```bash
   g++ evm_benchmark.cpp keccak.c -o evm_benchmark \
     -O3 -march=native -fopenmp \
     -lsecp256k1
  ./evm_benchmark
   ```
# Chạy evm_benchmark.cpp

- evm_benchmark.cpp có logic tạo các số cuối trùng nhau, sửa số muốn chọn ví dụ 5 là 5 số cuối giống nhau, mỗi số cuối giống nhau tỷ lệ 1/16 nhân lên (mở nano evm_benchmark.cpp để sửa)
   ```bash
    if (!ok || !lastNDigitsSame(hexp, 5)) continue;
   ```
- Chạy file để tìm ví, muốn tìm bao nhiêu thì thay vào, ví dụ 10
   ```bash
    ./evm_benchmark 10
   ```
  
  
