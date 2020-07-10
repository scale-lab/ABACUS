`timescale 1ns / 1ps
module me_datapath (clk, reset, comparator_init, comp_start16, state, address16, inputData, outSAD41, positionSAD41);

	input clk, reset;
	
	input comparator_init, comp_start16;	
	input [9:0] address16;
	 
	input [((8*17)-1):0] inputData;
	
	output [15:0] outSAD41;
	
	output [9:0] positionSAD41;
	
	reg [((8*16*16)-1):0] current_array;
	reg [((8*16*17)-1):0] search_array;
	
	wire [11:0] SAD01,SAD02,SAD03,SAD04,SAD05,SAD06,SAD07,SAD08,SAD09,SAD10,SAD11,SAD12,SAD13,SAD14,SAD15,SAD16;

	wire [15:0] SAD41_comp;

	input [2:0] state;
		
	// REGISTER SHIFTERS
	
	always @ (posedge clk)
	begin
		if (state == 3'd1)
		begin
			current_array <= {current_array[((8*15*16)-1):0], inputData[((8*16)-1):0]};
		end
	end
	
	always @ (posedge clk)
	begin
		if (state == 3'd2)
		begin
			search_array <= {search_array[((8*15*17)-1):0], inputData};
		end
		else if (state == 3'd3)
		begin
			search_array <= {search_array[((8*15*17)-1):0], inputData};
		end
		else if (state == 3'd4 || state == 3'd6)
		begin
			search_array <= search_array<<8;
		end
		else if (state == 3'd5)
		begin
			search_array <= {inputData, search_array[((8*16*17)-1):(8*1*17)]};
		end
	end
	
	// MODULE DECLERATIONS
	
me_sad_calculation sad_calculation (
		.clk 		(clk),
		.search 	(search_array),
		.current 	(current_array),
		.SAD01 	(SAD01[11:0]),
		.SAD02 	(SAD02[11:0]),
		.SAD03 	(SAD03[11:0]),
		.SAD04 	(SAD04[11:0]),
		.SAD05 	(SAD05[11:0]),
		.SAD06 	(SAD06[11:0]),
		.SAD07 	(SAD07[11:0]),
		.SAD08 	(SAD08[11:0]),
		.SAD09 	(SAD09[11:0]),
		.SAD10 	(SAD10[11:0]),
		.SAD11 	(SAD11[11:0]),
		.SAD12 	(SAD12[11:0]),
		.SAD13 	(SAD13[11:0]),
		.SAD14 	(SAD14[11:0]),
		.SAD15 	(SAD15[11:0]),
		.SAD16 	(SAD16[11:0]));
		
me_sad_reuse sad_reuse(
		.clk	(clk),
		.inSAD01 (SAD01), .inSAD02 (SAD02), .inSAD03 (SAD03), .inSAD04 (SAD04),
		.inSAD05 (SAD05), .inSAD06 (SAD06),	.inSAD07 (SAD07), .inSAD08 (SAD08),
		.inSAD09 (SAD09), .inSAD10 (SAD10), .inSAD11 (SAD11), .inSAD12 (SAD12),
		.inSAD13 (SAD13), .inSAD14 (SAD14), .inSAD15 (SAD15), .inSAD16 (SAD16),
		.outSAD41 (SAD41_comp));
		
me_comparator comparator(
		.clk 		(clk),
		.reset 		(reset),
		.clear		(comparator_init),
		.start16	(comp_start16),
		.position16	(address16),
		
		.inSAD41 (SAD41_comp), 
		.finalSAD41 (outSAD41),
		.positionSAD41	(positionSAD41));
endmodule
