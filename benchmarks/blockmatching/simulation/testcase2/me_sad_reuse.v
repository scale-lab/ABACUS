`timescale 1ns / 1ps
module me_sad_reuse (clk, inSAD01, inSAD02, inSAD03, inSAD04, inSAD05, inSAD06, inSAD07, inSAD08, inSAD09, inSAD10, inSAD11, inSAD12, inSAD13, inSAD14, inSAD15, inSAD16, outSAD41);
	
	input clk;
	
	input [11:0] inSAD01, inSAD02, inSAD03, inSAD04, inSAD05, inSAD06, inSAD07, inSAD08, inSAD09, inSAD10, inSAD11, inSAD12, inSAD13, inSAD14, inSAD15, inSAD16;
	
	reg [12:0] outSAD17, outSAD18, outSAD19, outSAD20, outSAD21, outSAD22, outSAD23, outSAD24, outSAD25, outSAD26, outSAD27, outSAD28, outSAD29, outSAD30, outSAD31, outSAD32;
	reg [13:0] outSAD33, outSAD34, outSAD35, outSAD36;
	reg [14:0] outSAD37, outSAD38, outSAD39, outSAD40;
	output reg [15:0] outSAD41;
		
	wire [14:0] SAD37_delay, SAD38_delay, SAD39_delay, SAD40_delay;
	wire [15:0] SAD41_delay;
	
	always @ (posedge clk)
	begin
		outSAD17 <= inSAD01 + inSAD05;
		outSAD18 <= inSAD02 + inSAD06;
		outSAD19 <= inSAD03 + inSAD07;
		outSAD20 <= inSAD04 + inSAD08;
		outSAD21 <= inSAD09 + inSAD13;
		outSAD22 <= inSAD10 + inSAD14;
		outSAD23 <= inSAD11 + inSAD15;
		outSAD24 <= inSAD12 + inSAD16;
		outSAD25 <= inSAD01 + inSAD02;
		outSAD26 <= inSAD03 + inSAD04;
		outSAD27 <= inSAD05 + inSAD06;
		outSAD28 <= inSAD07 + inSAD08;
		outSAD29 <= inSAD09 + inSAD10;
		outSAD30 <= inSAD11 + inSAD12;
		outSAD31 <= inSAD13 + inSAD14;
		outSAD32 <= inSAD15 + inSAD16;
	end
	
	always @ (posedge clk)
	begin
		outSAD33 <= (inSAD01 + inSAD02) + (inSAD05 + inSAD06);
		outSAD34 <= (inSAD03 + inSAD04) + (inSAD07 + inSAD08);
		outSAD35 <= (inSAD09 + inSAD10) + (inSAD13 + inSAD14);
		outSAD36 <= (inSAD11 + inSAD12) + (inSAD15 + inSAD16);
	end
	
	assign SAD37_delay = {1'b0,outSAD33} + outSAD35;
	assign SAD38_delay = {1'b0,outSAD34} + outSAD36;
	assign SAD39_delay = {1'b0,outSAD33} + outSAD34;
	assign SAD40_delay = {1'b0,outSAD35} + outSAD36;
	assign SAD41_delay = ({1'b0,outSAD33} + outSAD35) + ({1'b0,outSAD34} + outSAD36);
	
	always @ (posedge clk)
	begin
		outSAD37 <= SAD37_delay;
		outSAD38 <= SAD38_delay;
		outSAD39 <= SAD39_delay;
		outSAD40 <= SAD40_delay;
		outSAD41 <= SAD41_delay;
	end
endmodule
