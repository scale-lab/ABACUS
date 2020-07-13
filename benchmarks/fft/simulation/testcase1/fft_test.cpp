#include <verilated.h>
#include "Vfft_test.h"
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>

Vfft_test *fft;
vluint64_t main_time = 0;
/*
const char * const quads = { "0000", "0001", "0010", "0011",
                             "0100", "0101", "0110", "0111",
                             "1000", "1001", "1010", "1011",
                             "1100", "1101", "1110", "1111" };
const char * hex_to_bin_quad(unsigned char c)
{
  if (c >= '0' && c <= '9') return quads[     c - '0'];
  if (c >= 'A' && c <= 'F') return quads[10 + c - 'A'];
  if (c >= 'a' && c <= 'f') return quads[10 + c - 'a'];
  return -1;
}
*/

double sc_time_stamp(){
    return main_time;
}
/*
int binConv(char* str) {
    int n = 0;
    int size = strlen(str) - 1;
    int count = 0;
    while ( *str != '\0' ) {
        if ( *str == '1' ) {
            n = n + pow(2, size - count );
        }
        count++;
        str++;
    }
    return n;
}*/

int main(int argc, char** argv){

    vluint64_t cur_time = 0;
    fft = new Vfft_test;

    fft->enable = 0;
    fft->wt = 0;
    fft->clk = 0;
    while (!Verilated::gotFinish()) {
        if(main_time % 10 == 0)
            fft->clk = !fft->clk;

        if (main_time >= 5) {
            fft->enable = 1;   // Deassert reset
        }
        if (main_time >= 200) {
            fft->wt = 1;
        }
        if (main_time >= 205){
            break;
        }
        fft->eval();            // Evaluate model
        main_time++;            // Time passes...
    }

    fft->final();
    delete fft;
}
