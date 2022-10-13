## Description
This is a simple symmetric encryptor based XOR operator implemented by C/C++.
## Installation
`make`

## Usage
To encrypt your target file named like `file_to_encrypt`, type in shell as follows:
`./build/main.o file_to_encrypt`
Some primary mathematics will tell you How to dencrypt the same file.

## Warning
This is just a beta multithreads-symmetric encryptor, by in which the NUM_THREADS>1 will break your file inevitably, and this is anti-example for multi-thread mutex.
