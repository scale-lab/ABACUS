#include <verilated.h>
#include "Vsvm.h"
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>

Vsvm *svm;
vluint64_t main_time = 0;

double sc_time_stamp(){
    return main_time;
}

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
}

int main(int argc, char** argv){
    static vluint64_t PERIOD = 20;
    vluint64_t cur_time = 0;
    int nSVs = 6;
    int nTest = 1000;
    int wait1_4 = 0;
    int wait1 = PERIOD;
    ssize_t read;
    size_t len=0;
    FILE* weight, *input, *output;
    char* line;
    weight=fopen("init_weight.txt","r");
    input=fopen("init_input.txt","r");
    output=fopen("out_labels.txt", "w");
    if(weight==NULL || input==NULL || output == NULL){
        std::cout << "Unable to open weight/input file" << endl;
        return 0;
    }
    char temp[64]="", temp1[64]="";
    svm = new Vsvm;

    svm->clk = 1;
    cur_time = sc_time_stamp();
    while(main_time < cur_time + PERIOD/10){
        svm->eval();
        main_time++;
    }

    svm->reset = 1;
    svm->start = 0;
    svm->DE_in = 0;
    cur_time = sc_time_stamp();
    while(main_time < cur_time + PERIOD){
        if(main_time % 10 == 0)
            svm->clk = !svm->clk;
        svm->eval();
        main_time++;
    }

    svm->reset = 0;
    svm->start = 1;
    cur_time = sc_time_stamp();
    while(main_time < cur_time + 2*PERIOD){
        if(main_time % 10 == 0)
            svm->clk = !svm->clk;
        svm->eval();
        main_time++;
    }

    for(int i=0; i < nSVs; i++){
        if((read=getline(&line, &len, weight))==-1){
            std::cout << "Cannot read input weights." << endl;
            exit(0);
        }
        sscanf(line,"%s ", temp);
        svm->weight = (short int)binConv(temp);
        cur_time = sc_time_stamp();
        while(main_time < cur_time + PERIOD){
            if(main_time % 10 == 0)
                svm->clk = !svm->clk;
            svm->eval();
            main_time++;
        }
    }

    svm->DE_in = 1;
    for(int i=0; i < nTest; i++){
        if((read=getline(&line, &len, input))==-1){
            std::cout << "Cannot read input weights." << endl;
            exit(0);
        }
        sscanf(line,"%s %s", temp, temp1);
        svm->data_x = (short int)binConv(temp);
        svm->data_y = (short int)binConv(temp1);
//        printf("Input Data: %04x %04x\n",svm->data_x, svm->data_y);
        cur_time = sc_time_stamp();
        while(main_time < cur_time + PERIOD){
            if(main_time % 10 == 0)
                svm->clk = !svm->clk;
            svm->eval();
            main_time++;
            if(svm->DE_out){
                if(wait1_4 < PERIOD/4){
                    wait1_4++;
                }
                else{
                    if(wait1 < PERIOD){
                        wait1++;
                    }else{
                        wait1 = 1;
                        fprintf(output,"%hd\n", svm->label);
                    }
                }
            }
        }
    }
    svm->DE_in = 0;
    svm->eval();
    main_time++;

    while(svm->DE_out){
        if(wait1 < PERIOD){
            wait1++;
        }else{
            wait1 = 1;
            fprintf(output,"%hd\n", svm->label);
        }

        if(main_time % 10 == 0)
            svm->clk = !svm->clk;
        svm->eval();
        main_time++;
        if (Verilated::gotFinish()) exit(0);
    }
LEAVING:
    fclose(output);
    fclose(input);
    fclose(weight);
    svm->final();
    delete svm;
}
