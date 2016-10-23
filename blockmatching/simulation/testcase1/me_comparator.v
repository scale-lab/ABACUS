`timescale 1ns/1ps
module me_comparator (clk, reset, clear, start16, position16, inSAD41, finalSAD41, positionSAD41);

	input clk;
	input reset, clear;
	input start16;

	input [9:0] position16;

	input [15:0] inSAD41;

	output reg [15:0] finalSAD41;

	output reg [9:0] positionSAD41;

	always @ (posedge clk or posedge reset)
	begin
	if (reset)
	begin
		positionSAD41  <= 10'd0;
	//
		finalSAD41  <= 16'hFFFF;
	end
	else if (clear)
	begin
		positionSAD41  <= 10'd0;
//
		finalSAD41  <= 16'hFFFF;
    end	
	else 
	begin
		if (start16 == 1'b1)
		begin
			if (inSAD41 < finalSAD41)
				begin
					finalSAD41 <= inSAD41;
					positionSAD41 <= position16;
				end
			else
				begin
					finalSAD41 <= finalSAD41;
					positionSAD41 <= positionSAD41;
				end
			end
		end
	end
endmodule
