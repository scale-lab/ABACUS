`timescale 1ns / 1ps
module me_PE(clk, A, B, AD);

	input clk;
	input [7:0] A,B;
	output reg[7:0] AD;

	wire A_smaller;
	wire [7:0] D, E;

	assign A_smaller = (A < B);
	assign D = A_smaller ? B : A;
	assign E = A_smaller ? A : B;

	always @ (posedge clk)
	begin
		AD <= D - E;
	end

endmodule
