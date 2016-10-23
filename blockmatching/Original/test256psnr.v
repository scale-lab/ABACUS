`timescale 1ns/1ps

module test256psnr;
//////////////////////////////////////////////////
reg [7:0] ref_frame [0:101375];
reg [7:0] search_frame [0:101375];
reg [7:0] mid_frame16x16 [0:101375];

reg [7:0] reference [0:255];
reg [7:0] search [0:2303]; //48x48 ama 47x47si kullanilacak
////////////////////////////////////////////////
//Outputs
wire [9:0] positionSAD; 
	
wire outready;

wire [15:0] outSAD;
////////////////////////////////////////////////
//Inputs
reg clk, reset, start, terminate;
	
reg searchwrite3col,searchwrite2col;
reg [7:0] searchaddress;
reg [127:0] refdata;
reg [135:0] searchdata;
////////////////////////////////////////////////
//Vars
integer i,j,m,n,f1,f2,o1,o2,a,macroblock,mse,diff,diffkare;


integer m_i, n_i;

/////////////////////////////////////////////
//Outputvars
wire [4:0] y;
wire [4:0] x;

assign x= positionSAD[9:5]; assign y = positionSAD[4:0];
/////////////////////////////////////////////

always #(20/2) clk= ~clk;

initial
begin

// /////////initial conditions/////////////
macroblock = 0;
clk = 1;
reset = 0;
start = 0;
terminate = 0;

searchwrite3col = 0;
searchwrite2col = 0;
refdata = 0;
searchaddress = 0;
searchdata = 0;

$readmemh("./SRC/frame2.txt",ref_frame);  
$readmemh("./SRC/frame1.txt",search_frame);


f1 = $fopen("./output/output16.txt");
f2 = $fopen("./output/psnr.txt");

o1 = $fopen("./output/midframe.txt");
o2 = $fopen("./output/ref_frame.txt");


//////////////////////////////////////
#10;
reset = 1;
#20;
#20;
#20;
reset = 0;
#15;

for(i=0; i<288; i=i+16)
begin
    for(j=0; j<352; j=j+16)
	begin
		
		////////////////Reference
		for (m=0; m<16; m=m+1)
		begin
			for (n=0; n<16; n=n+1)
			begin
				reference [m*16+n] = ref_frame[((i+m)*352)+(j+n)];                
            end
		end
		////////////////Search
		for(m=0; m<48; m=m+1)
		begin
			for(n=0; n<48; n=n+1)
			begin
				
				// if m is out of boundry
				m_i = i+m;
				if (m_i < 16)
					m_i = i;
				else if (m_i > (288-1+16))
					m_i = i + 15;
				else
					m_i = i+m-16;
					
				// if n is out of boundry
				n_i = j+n;
				if (n_i < 16)
					n_i = j;
				else if (n_i > (352-1+16))
					n_i = j + 15;
				else
					n_i = j+n-16;
			
				search [(m*48+n)] = search_frame[m_i*352+n_i];
			end
		end
		
		#20;
		searchwrite3col = 1;
		searchwrite2col = 1;

		for(a=0;a<141;a=a+1)
		begin
			#20;
			
			terminate = 0;
			
			if (a == 30)
			begin
				start = 1; //46'dan itibaren çalismaya basladik
			end
			else
			begin
				start = 0; // Start inputuna gerek kalmadi
			end
			
			if ( a > 31 && a < 48 )
			begin
				refdata = {reference[((a-32)*16)], reference[((a-32)*16)+1], reference[((a-32)*16)+2], reference[((a-32)*16)+3],
							 reference[((a-32)*16)+4], reference[((a-32)*16)+5], reference[((a-32)*16)+6], reference[((a-32)*16)+7],
							  reference[((a-32)*16)+8], reference[((a-32)*16)+9], reference[((a-32)*16)+10], reference[((a-32)*16)+11],
							   reference[((a-32)*16)+12], reference[((a-32)*16)+13], reference[((a-32)*16)+14], reference[((a-32)*16)+15]};
			end
			else
			begin				
				refdata = 0;
			end
			
			if (a < 47)
			begin
				searchaddress = a;	
				searchwrite2col = 1;
				searchdata = {search[(48*a)], search[(48*a)+1], search[(48*a)+2], search[(48*a)+3], search[(48*a)+4],
								 search[(48*a)+5], search[(48*a)+6], search[(48*a)+7], search[(48*a)+8], search[(48*a)+9],
								  search[(48*a)+10], search[(48*a)+11], search[(48*a)+12], search[(48*a)+13], search[(48*a)+14],
								   search[(48*a)+15], search[(48*a)+16]};
			end
			else if (a < 94)
			begin
				searchaddress = a;
				searchwrite2col = 1;
				searchdata = {search[(48*(a-47))+17], search[(48*(a-47))+1+17], search[(48*(a-47))+2+17], search[(48*(a-47))+3+17], search[(48*(a-47))+4+17],
								 search[(48*(a-47))+5+17], search[(48*(a-47))+6+17], search[(48*(a-47))+7+17], search[(48*(a-47))+8+17], search[(48*(a-47))+9+17],
								  search[(48*(a-47))+10+17], search[(48*(a-47))+11+17], search[(48*(a-47))+12+17], search[(48*(a-47))+13+17], search[(48*(a-47))+14+17],
								   search[(48*(a-47))+15+17], search[(48*(a-47))+16+17]};
			end
			else
			begin
				searchaddress = a;
				searchwrite2col = 0;
				searchdata = {search[(48*(a-94))+34], search[(48*(a-94))+1+34], search[(48*(a-94))+2+34], search[(48*(a-94))+3+34], search[(48*(a-94))+4+34],
								 search[(48*(a-94))+5+34], search[(48*(a-94))+6+34], search[(48*(a-94))+7+34], search[(48*(a-94))+8+34], search[(48*(a-94))+9+34],
								  search[(48*(a-94))+10+34], search[(48*(a-94))+11+34], search[(48*(a-94))+12+34], search[(48*(a-94))+13+34], search[(48*(a-94))+14+34],
								   search[(48*(a-94))+15+34], search[(48*(a-94))+16+34]};	
			end
			// Not: search memorysi 48x48 ama 47 x 47 kullaniliyor		
		end

		#20;

		while(!outready)
		begin
			#20;
			
			if (1'd0 == 1'd1) // (outSAD01 == 12'd0) // early termination condition
			begin
				terminate = 1; //46'dan itibaren çalismaya basladik
			end
		end

		#20;
///////////////////////////////////////////
/////Output Written
		macroblock = macroblock + 1;
		$fwrite(f1,"(%3d, %3d, %3d)", x, y, outSAD);
	
/////////////////////////////////////////////
////Mid-Frame Building

		for(m=0; m<16; m=m+1)
		begin
			for(n=0; n<16; n=n+1)
			begin
				mid_frame16x16[((i+m)*352)+(j+n)] = search[((m+y)*48)+(n+x)];
			end
		end				
	end //end of j loop
	$fwrite(f1,"\n");   
end //end of i loop

/////////////PSNR16//////////
mse = 0;
diff = 0;
diffkare = 0;

for(a = 0; a < 101376; a = a+1)
begin
	if(mid_frame16x16[a] > ref_frame[a])
	begin
		diff = mid_frame16x16[a] - ref_frame[a];
	end
	else
	begin
		diff = ref_frame[a] - mid_frame16x16[a];
	end
	diffkare = diff*diff;
	mse = mse + diffkare;

	$fwrite(o1, "%d \n", mid_frame16x16[a]);
	$fwrite(o2, "%d \n", ref_frame[a]);
end

$fwrite(f2,"MSETOT16: %3d \n", mse);
$fwrite(f2,"MSE16: %3d \n", mse/101376);

$fclose(f1);
$fclose(f2);
$fclose(o1);
$fclose(o2);
$finish;


end //end of initial statement



me_topmodule top(
			.clk(clk),
			.rst(reset),
			.start(start),
			.stop(terminate),
			.CurrentBlock(refdata),
			.WE_S(searchwrite3col),
			.WE_S2(searchwrite2col),
			.WADDR_S(searchaddress),
			.WDATA_S(searchdata),
			.DE_out(outready),
			.outSAD41(outSAD),
			.positionSAD41(positionSAD)
		   );
		   
		   
		   
endmodule
