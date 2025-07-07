# 1. Cài ubuntu
   ```bash
   wsl --unregister Ubuntu
   wsl --install
   ```

# 2. Cài C++
   ```bash
   sudo apt update && sudo apt upgrade -y
   sudo apt install build-essential -y
   g++ --version
   sudo apt install libssl-dev -y
   ```

# 3. Cài libsecp256k1
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
   
# 4. Copy 4 file và code (Lưu Ctrl+O, enter, Ctrl+x)
   ```bash
   cd
   nano evm_benchmark.cpp
   nano vanity_finder.cpp
   nano keccak.h
   nano keccak.c
   ```
evm_benchmark và evm_benchmark_0 có logic khác nhau. evm_benchmark tạo ví 10 số đầu vào 10 số cuối, số cuối trùng nhau theo yêu cầu evm_benchmark_0 tạo số cuối trùng nhau thôi, ít logic thì nhanh hơn. Sử dụng chung file làm theo bước 5.
# 5.  Biên dịch và chạy evm_benchmark.cpp
- Biên dịch
   ```bash
   g++ evm_benchmark.cpp keccak.c -o evm_benchmark \
     -O3 -march=native -fopenmp \
     -lsecp256k1
  ./evm_benchmark
   ```
- Chạy evm_benchmark.cpp: evm_benchmark.cpp có logic tạo các số cuối trùng nhau, sửa số muốn chọn ví dụ 5 là 5 số cuối giống nhau, mỗi số cuối giống nhau tỷ lệ 1/16 nhân lên (mở nano evm_benchmark.cpp để sửa ở dòng bên dưới)
   ```bash
    if (!ok || !lastNDigitsSame(hexp, 5)) continue;
   ```
- Chạy file để tìm ví, muốn tìm bao nhiêu thì thay vào, ví dụ 10
   ```bash
    ./evm_benchmark 10
   ```
Kết quả tin ra file wallet.txt
# 6.  Biên dịch và chạy vanity_finder.cpp (tìm nhiều ví bất kỳ siêu nhanh)
- Biên dịch
   ```bash
   g++ vanity_finder.cpp keccak.c -o vanity_finder \
     -O3 -march=native -fopenmp \
     -lsecp256k1
   ```
- Chạy file để tìm ví bất kỳ
   ```bash
    ./vanity_finder 10000
   ```
Kết quả in ra file wallet.csv
  
  
  
