#include <verilated.h>
#include "Vme_topmodule.h"
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>

/* GLOBAL SIGNAL */
CData clk, reset, start, stop, outready;
CData searchwrite3col, searchwrite2col, x, y;
CData searchaddress = 0;
SData outSAD = 0;
SData positionSAD = 0;
//WData refdata[4] = {0, 0, 0, 0};  // 128 bits
//WData searchdata[5] = {0, 0, 0, 0, 0}; //136 bits

/* GLOBAL VARIABLE */
Vme_topmodule *bm;
vluint64_t main_time = 0;
static vluint64_t PERIOD = 20;



double sc_time_stamp(){
    return main_time;
}

void topmodule(void){
    if(main_time % 10 == 0)
        bm->clk = !bm->clk;
    bm->rst = reset;
    bm->start = start;
    bm->stop = stop;

    bm->WE_S = searchwrite3col;
    bm->WE_S2 = searchwrite2col;
    bm->WADDR_S = searchaddress;
//    bm->WDATA_S = searchdata;
//    bm->CurrentBlock = refdata;
    bm->eval();

    // Output
    outready = bm->DE_out;
    outSAD = bm->outSAD41;
    positionSAD = bm->positionSAD41;
}

#define BUFSIZE 4096
int main(int argc, char** argv){


    vluint64_t cur_time = 0;
    int marcoblock = 0;
    // Allocating the verilator model
    bm = new Vme_topmodule;
    FILE *search_file, *ref_file;
    unsigned char ref_frame[101376] = "";
    unsigned char search_frame[101376] = "";
    unsigned char mid_frame16x16[101376] = "";
    unsigned char reference[256] = "";
    unsigned char search[2304] = "";
    char buf[BUFSIZE] = "";
    FILE *f1, *f2, *o1, *o2;

    int i, j, k, b;

    search_file=fopen("frame1.txt","r");
    ref_file=fopen("frame2.txt","r");
    f1 = fopen("output16.txt","w");
    f2 = fopen("psnr.txt","w");
    o1 = fopen("midframe.txt","w");
    o2 = fopen("ref_frame.txt","w");

    if(search_file == NULL || ref_file == NULL || f1 == NULL
            || f2 == NULL || o1 == NULL || o2 == NULL){
        std::cout << "Unable to open input/output file" << endl;
        return 0;
    }

    j = 0;
    while (fgets(buf, sizeof(buf), ref_file) != NULL) {
        k = j + 352;
        i = 0;
//        printf("\n");
        while (buf[i] != '\n' && buf[i] != '\0' && i < BUFSIZE && j < k) {
            sscanf(&buf[i], "%2x", &b);
            ref_frame[j] = b;
            j += 1;
            i += 3;
//            printf("%X ",b);
        }
    }

    j = 0;
    while (fgets(buf, sizeof(buf), search_file) != NULL) {
        i = 0; k = j + 352;

 //       printf("j: %d\n",j);
        while (buf[i] != '\n' && buf[i] != '\0' && j < k) {
            sscanf(&buf[i], "%2x", &b);
            search_frame[j] = b;
            j += 1;
            i += 3;
        }
    }

    fclose(ref_file);
    fclose(search_file);


    // initial condition:
    bm->clk = 1;
    reset = (CData)0;
    start = (CData)0;
    stop = (CData)0;

    searchwrite3col = 0;
    searchwrite2col = 0;
    searchaddress = 0;
    // wait 10
    cur_time = sc_time_stamp();
    while(main_time < cur_time + PERIOD/2){
        topmodule();
        main_time++;
    }

    reset = 1;
    // wait 60
    cur_time = sc_time_stamp();
    while(main_time < cur_time + 3*PERIOD){
        topmodule();
        main_time++;
    }

    reset = 0;
    // wait 15
    cur_time = sc_time_stamp();
    while(main_time < cur_time + 15){
        topmodule();
        main_time++;
    }

    int m_i, n_i;
    for(i=0; i < 288; i=i+16){
        for(j=0; j<352; j=j+16 ){
            // Reference
            for(int m=0; m<16; m++){
                for(int n=0; n<16; n++){
                    reference[m*16+n] = ref_frame[((i+m)*352)+(j+n)];
                }
            }
            // Search frame
            for(int m=0; m<48; m++){
                for(int n=0; n<48; n++){
                    m_i = i + m;
                    if(m_i < 16)
                        m_i = i;
                    else if(m_i > (288+15)) // 288 - 1 + 16
                        m_i = i + 15;
                    else
                        m_i = i + m - 16;

                    n_i = j + n;
                    if (n_i < 16)
                        n_i = j;
                    else if(n_i > (352 + 15)) // 352 - 1 + 16
                        n_i = j + 15;
                    else
                        n_i = j + n - 16;

                    search[m*48 + n] = search_frame[m_i*352 + n_i];
                }
            }
            // wait 20
            cur_time = sc_time_stamp();
            while(main_time < cur_time + PERIOD){
                topmodule();
                main_time++;
            }

            searchwrite3col = 1;
            searchwrite2col = 1;

            for(int a=0; a<141; a++){
                // wait 20
                cur_time = sc_time_stamp();
                while(main_time < cur_time + PERIOD){
                    topmodule();
                    main_time++;
                }

                stop = 0;
                if(a == 30)
                    start = 1;
                else
                    start = 0;

               // printf("--\n");
                if(a > 31 && a < 48){
                    for(int m = 0; m < 16; m++){
                        _VL_INSERT_WI(0, bm->CurrentBlock, 0, m*8+7, m*8);
                        _VL_INSERT_WI(0, bm->CurrentBlock, (IData)reference[(a-32)*16+15-m],m*8+7 ,m*8);
                        //VL_ASSIGNSEL_WIII(8,m*8,bm->CurrentBlock,\
                                (IData)reference[(a-32)*16 + 15 - m]);
                    }
                }
                else{
                    _VL_INSERT_WI(0, bm->CurrentBlock, 0, 31, 0);
                    _VL_INSERT_WI(0, bm->CurrentBlock, 0, 63, 32);
                    _VL_INSERT_WI(0, bm->CurrentBlock, 0, 95, 64);
                    _VL_INSERT_WI(0, bm->CurrentBlock, 0, 127, 96);
                }

                searchaddress = (unsigned char)a;
                if(a < 47){
                    searchwrite2col = 1;
                    for(int m = 0; m < 17; m++){
                        _VL_INSERT_WI(0,bm->WDATA_S, (IData)search[48*a + 16 - m], m*8+7, m*8);
                    }
                }
                else if(a < 94){
                    searchwrite2col = 1;
                    for(int m = 0; m < 17; m++){
                        _VL_INSERT_WI(0,bm->WDATA_S, (IData)search[48*(a-47) + 17 + 16 - m], m*8+7, m*8);
                    }
                }
                else{
                    searchwrite2col = 0;
                    for(int m = 0; m < 17; m++){
                        _VL_INSERT_WI(0, bm->WDATA_S, (IData)search[48*(a-94) + 34 + 16 - m], m*8+7, m*8);
                    }
                }
             //   printf("CurrentBlock: %x %x %x %x %x .. %x %x %x %x .. %d\n",bm->WDATA_S[4],bm->WDATA_S[3],bm->WDATA_S[2],\
                        bm->WDATA_S[1],bm->WDATA_S[0],(IData)search[a*48],\
                   (IData)search[48*a+1],(IData)search[48*a+2],(IData)search[48*a+3],a);

            }

            // wait 20
            cur_time = sc_time_stamp();
            while(main_time < cur_time + PERIOD){
                topmodule();
                main_time++;
            }

            while(!outready){
                // wait 20
                cur_time = sc_time_stamp();
                while(main_time < cur_time + PERIOD){
                    topmodule();
                    main_time++;
                }
            }

            // wait 20
            cur_time = sc_time_stamp();
            while(main_time < cur_time + PERIOD){
                topmodule();
                main_time++;
            }

            // writing output
            x = (CData)((positionSAD>>5)&((SData)0x1F));
            y = (CData)(positionSAD&((SData)0x1F));
            //printf("%x %x, %x\n",x,y,positionSAD);
            fprintf(f1,"(%3d, %3d, %3d)",x,y,outSAD);

            // mid-frame building
            for(int m=0; m<16; m++){
                for(int n=0; n<16; n++){
                    mid_frame16x16[((i+m)*352)+(j+n)] = search[((m+y)*48)+(n+x)];
                }
            }

        } // j - loop
        fprintf(f1,"\n");
    }

    // PSNR16
    int mse = 0;
    int diff = 0;
    for(int a=0; a < 101376; a++){
        diff = abs(mid_frame16x16[a] - ref_frame[a]);
        diff = diff*diff;
        mse = mse + diff;

        fprintf(o1, "%d \n", mid_frame16x16[a]);
        fprintf(o2, "%d \n", ref_frame[a]);
    }
    fprintf(f2,"MSETOT16: %3d \n", mse);
    fprintf(f2,"MSE16: %3d \n", mse/101376);

    delete(bm);
    fclose(f1); fclose(f2); fclose(o1); fclose(o2);
    return 0;
}
