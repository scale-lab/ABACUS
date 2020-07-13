`timescale 1ns/1ps
module fir_test;

parameter nCoef=25;
parameter nDataPoints= 58061;//63488;//was 58061; 
parameter coefWidth=16;
parameter dataWidth=8;
parameter dataOutWidth=15; //was 14
//general purpose registers
reg clk, load_c, reset;
reg [coefWidth-1:0] coef_rom [0:nCoef-1];
reg [dataWidth-1:0] data_rom[0:(nDataPoints*4)-1];

reg [coefWidth-1:0] coef_in;
reg [dataWidth-1:0] data_in; //was[datawidth-1:0]

reg sim_done_flag;
wire [dataOutWidth-1:0] data_out;
integer out;
integer i, j;

//define clk
parameter PERIOD=20;
always #(PERIOD/2) clk= ~clk;

//read_inputs
initial
begin
	$readmemb("data_in.txt", data_rom);
	$readmemb("coef_in.txt", coef_rom);
end

//write output
initial
begin
	out = $fopen("output.txt");
	sim_done_flag=0;
	
	while(sim_done_flag==0)
	//while(1)
	begin
		$fwrite(out,"'%b' \n", data_out);
		#PERIOD;	
	end

	//#(PERIOD*20);
	//$finish;
	
end

initial
begin
	#(PERIOD*63542);//58115)//63542)// was 58115); //74590
	sim_done_flag=1;
	$finish;
end

initial
begin

	clk = 1;
	reset=0;
	coef_in=0;
	data_in=0;
	coef_in=0;
	load_c=1;
//	#PERIOD

	for (i=0; i<nCoef; i=i+1)
	begin
		coef_in = coef_rom[i];
		#PERIOD;
	
		if (i==nCoef-1)
		begin
			load_c=0;
			coef_in=0;
		end
	end
	
	for (j=0; (j<nDataPoints*4); j=j+1)
	begin
		data_in = data_rom[j];
		#PERIOD;
	end

	//#(PERIOD*100);
	//sim_done_flag=1;
	
end

//initial
//begin	
//	for (j=0; (j<nDataPoints*4); j=j+1)
//	begin
//		data_in = data_rom[j];
//		#PERIOD;
//	end
	

//end

fir_filter fir1(
	.clk(clk),
	.coef_in(coef_in),
	.data_in(data_in),
	.data_out(data_out),
	.load_c(load_c),
	.reset(reset)
	);

endmodule
