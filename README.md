# Branch Predictor Exploration for RISC-V Softcores
This repository serves as a branch predictor softcore testing framework. It contains multiple implementations of branch predictors of varying complexity. Each branch predictor performance is tested using the coremark benchmark program.

## 📑 Table of Contents

* [Repository Structure](#-repository-structure)
* [Baseline](#-baseline)
* [Usage](#-usage)
* [Prerequisites](#-prerequisites)
* [Installation](#-installation)

## 📂 Repository Structure

```
branch_predictor_versions/  Different branch predictors with a RV32IM core
custom_c_test/              Test custom programs on the core
riscv-tests/          
dhrystone/          
coremark/          
rtl/                        RTL code for RISC-V Core to test
Makefile
test.py                     Cocotb test file
README.md                   This file
```

## 📊 Baseline
To test the performance of the different branch predictors, a baseline risc-v core has been used. It is based on the RV32IM ISA. Some features are:
* 5 Stage pipelined core
* RV32IM ISA support
* Multiplication takes 1 cycle and Division/Remainder is configurable and execution cycles depend on desired Fmax. For purpose of experients here it takes 16 cycles
* Jump/Branch operation is performed in MEM stage to achieve high Fmax
* The core has memory mapped registers for cycle count, instruction count, jump/branch instruction and misprediction count
* Branch Prediction Unit is connected to IF stage for prediction and update happens in MEM stage
* All big memory elements like memories, btb tables etc are modelled using BRAM

```

## 🔄 Usage
Copy the required version of the code from the branch_predictor_versions folder into the root directory of this repository and rename it to rtl.
To run any of the tests, after installing the pre-requisties, you can run the following commands:

```bash
make custom
make riscv-tests
make dhrystone
make coremark
```

## 🛠️ Prerequisites

* Linux / WSL
* **RISC-V GNU Toolchain** (Tested: `riscv32-unknown-elf-gcc 15.2.0`)
* **Verilator** (Tested: `v5.042`)
* **cocotb** (Tested: `2.0.1`)
* Gtkwave (optional)

---

## 📦 Installation

### 1️⃣ RISC-V GNU Toolchain (RV32I)

```bash
git clone https://github.com/riscv/riscv-gnu-toolchain
cd riscv-gnu-toolchain
```

```bash
sudo apt-get install -y autoconf automake autotools-dev curl python3 python3-pip \
libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf \
libtool patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake \
libglib2.0-dev libslirp-dev libncurses-dev
```

```bash
./configure --prefix=$HOME/riscv32i --with-arch=rv32i --with-abi=ilp32
make
```

```bash
echo 'export RISCV=$HOME/riscv32i' >> ~/.bashrc
echo 'export PATH=$RISCV/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
```

```bash
riscv32-unknown-elf-gcc --version
```

---

### 2️⃣ Verilator

```bash
sudo apt-get install -y git help2man perl python3 make autoconf g++ flex bison \
ccache libgoogle-perftools-dev numactl perl-doc \
libfl2 libfl-dev zlib1g zlib1g-dev
```

```bash
git clone https://github.com/verilator/verilator
cd verilator
git checkout stable
autoconf && ./configure && make
sudo make install
```

```bash
verilator --version
```

---

### 3️⃣ cocotb (Python Virtual Environment)

```bash
sudo apt-get install -y python3 python3-pip python3-venv libpython3-dev
```

```bash
python3 -m venv ~/cocotb
source ~/cocotb/bin/activate
pip install cocotb cocotb-bus cocotb-test
```

```bash
python3 -c "import cocotb; print('cocotb OK')"
```

---