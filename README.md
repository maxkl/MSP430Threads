Simple threading for the MSP430G2553 (it should work with other MSP430s by changing the timer configuration code).

## Usage

See `src/main.c`.

## Building

Requires cmake and mspgcc.

```
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../msp430.cmake -DCMAKE_BUILD_TYPE=Release -DMCU=msp430g2553 ..
make
```

## Uploading

```
sudo mspdebug rf2500 "prog MSP430Threads"
```
