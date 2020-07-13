#include <verilated.h>
#include "Vfir_filter.h"
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>

#define PERIOD 20

Vfir_filter *fir_filter;
vluint64_t main_time = 0;

double sc_time_stamp(){
    return main_time;
}

template <class type> type binConv(char* str) {
    type n = 0;
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
}

int main(int argc, char** argv){
    vluint64_t cur_time = 0;
    int nDataPoints = 63488;

    int nCoef = 25;
    int wait1_4 = 0;
    int wait1 = PERIOD;
    ssize_t read;
    size_t len=0;
    FILE *data_file, *coef_file, *output_file;
    char *line;
    int n;

    data_file=fopen("data_in.txt","r");
    coef_file=fopen("coef_in.txt","r");
    output_file=fopen("output.txt", "w");
    if(data_file==NULL || coef_file==NULL || output_file == NULL){
        std::cout << "Unable to open data/coef/output file" \
            << endl;
        return 0;
    }

    char temp[64]="", temp1[64]="";
    fir_filter = new Vfir_filter;

    // initialization
    fir_filter->clk = 1;
    fir_filter->reset = 0;
    fir_filter->coef_in = 0;
    fir_filter->data_in = 0;
    fir_filter->load_c = 1;

    for (int i=0; i < nCoef; i++){
        if((read=getline(&line, &len, coef_file))==-1){
            std::cout << "Cannot read input coefficients." << endl;
            exit(0);
        }
        sscanf(line,"%s ", temp);
        fir_filter->coef_in = binConv<SData>(temp);
        fprintf(output_file, "'");
        n = 0;
        while (n < 15) {
            if (fir_filter->data_out & 1)
                fprintf(output_file,"1");
            else
                fprintf(output_file,"0");
            fir_filter->data_out >>= 1;
            n++;
        }
        // the space before \n is necessary
        fprintf(output_file,"' \n");
        //fprintf(output_file, "%X' \n",fir_filter->data_out );
        // wait 20
        cur_time = sc_time_stamp();
        while(main_time < cur_time + PERIOD){
            if(main_time % 10 == 0)
                fir_filter->clk = !fir_filter->clk;
            fir_filter->eval();
            main_time++;
        }
    }
    fir_filter->load_c = 0;
    fir_filter->coef_in = 0;

    for (int i=0; i < nDataPoints*4; i++){
        if((read=getline(&line, &len, data_file))==-1){
            std::cout << "End of input data. " << i \
                << " " << main_time << endl;
            //printf(stdout,"%d\n",i);
            break;
        }else{
            sscanf(line,"%s ", temp);
            fir_filter->data_in = binConv<CData>(temp);
        }
        fprintf(output_file, "'");
        n = 0;
        while (n < 15) {
            if (fir_filter->data_out & (SData)0x4000)
                fprintf(output_file,"1");
            else
                fprintf(output_file,"0");
            fir_filter->data_out <<= 1;
            n++;
        }
        // the space before \n is necessary
        fprintf(output_file,"' \n");

        // wait 20
        cur_time = sc_time_stamp();
        while(main_time < cur_time + PERIOD){
            if(main_time % 10 == 0)
                fir_filter->clk = !fir_filter->clk;
            fir_filter->eval();
            main_time++;
        }
//        if(main_time >= PERIOD*58086)
//            break;

    }
    // LEAVING
    fclose(output_file);
    fclose(data_file);
    fclose(coef_file);
    fir_filter->final();
    delete fir_filter;
}
