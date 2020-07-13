`timescale 1ns / 1ps
module me_topmodule (	//INPUTS
						clk, rst, start, stop, CurrentBlock,
						//MEM_CTRL
						WE_S, WE_S2, WADDR_S, WDATA_S,
						//OUTPUTS
						DE_out, 
						outSAD41, 
						positionSAD41
					);
	
	input clk, rst, start, stop;
	
	input WE_S,WE_S2;
	input [7:0] WADDR_S; input [135:0] WDATA_S;
	
	input [127:0] CurrentBlock; 	

	output [15:0] outSAD41;
	
	output [9:0] positionSAD41;
	
	output DE_out;

	wire [7:0] WDATA_S00, WDATA_S01, WDATA_S02, WDATA_S03, WDATA_S04, WDATA_S05, WDATA_S06, WDATA_S07, WDATA_S08, WDATA_S09, WDATA_S10, WDATA_S11, WDATA_S12, WDATA_S13, WDATA_S14, WDATA_S15, WDATA_S16;
	
	wire [7:0] RADDR_S00, RADDR_S01, RADDR_S02, RADDR_S03, RADDR_S04, RADDR_S05, RADDR_S06, RADDR_S07, RADDR_S08, RADDR_S09, RADDR_S10, RADDR_S11, RADDR_S12;
	wire [6:0] RADDR_S13, RADDR_S14, RADDR_S15, RADDR_S16;
	wire [7:0] RDATA_S00, RDATA_S01, RDATA_S02, RDATA_S03, RDATA_S04, RDATA_S05, RDATA_S06, RDATA_S07, RDATA_S08, RDATA_S09, RDATA_S10, RDATA_S11, RDATA_S12, RDATA_S13, RDATA_S14, RDATA_S15, RDATA_S16;
	
	reg [135:0] RDATA;
	
	wire extra_column_en; wire [0:16] read_en; wire [4:0] order; wire [2:0] state;
	
	parameter S0 = 0, S1 = 1, S2 = 2, S3 = 3, S4 = 4, S5 = 5, S6 = 6, S7 = 7;
		

	wire [135:0] RDATA_0;
	reg [135:0] RDATA_1, RDATA_2, RDATA_4, RDATA_8, RDATA_16;
		
	assign RDATA_0 = {RDATA_S00, RDATA_S01, RDATA_S02, RDATA_S03, RDATA_S04, RDATA_S05, RDATA_S06, RDATA_S07, RDATA_S08, RDATA_S09, RDATA_S10, RDATA_S11, RDATA_S12, RDATA_S13, RDATA_S14, RDATA_S15, RDATA_S16};	

	always @*
	begin
		if (state == S0)
			RDATA = 136'd0;
		else if (state == S1)
			RDATA = {7'd0 , CurrentBlock};
		else if (state == S7)
			RDATA = 136'd0;
		else // if (state == S2-S6)
		begin
			if (order[4])
				RDATA_16 = {RDATA_0[(1*8)-1:(0*8)], RDATA_0[(17*8)-1:(1*8)]};
			else
				RDATA_16 = RDATA_0;
			
			if	(order[3])
				RDATA_8 = {RDATA_16[(9*8)-1:(0*8)], RDATA_16[(17*8)-1:(9*8)]};
			else
				RDATA_8 = RDATA_16;
			
			if (order[2])
				RDATA_4 = {RDATA_8[(13*8)-1:(0*8)], RDATA_8[(17*8)-1:(13*8)]};
			else
				RDATA_4 = RDATA_8;
			
			if (order[1])
				RDATA_2 = {RDATA_4[(15*8)-1:(0*8)], RDATA_4[(17*8)-1:(15*8)]};
			else
				RDATA_2 = RDATA_4;
						
			if (order[0])
				RDATA_1 = {RDATA_2[(16*8)-1:(0*8)], RDATA_2[(17*8)-1:(16*8)]};
			else
				RDATA_1 = RDATA_2;	

			if (extra_column_en)
				RDATA = RDATA_1;
			else
				RDATA = {RDATA_1[135:8], 8'd0};				
		end
	end 

	wire [9:0] address16; wire comparator_init;
	
	me_control control (
		.clk 			(clk), 
		.reset			(rst), 
		.start			(start),
		.stop			(stop),
		.DE_out 		(DE_out),
		.state			(state),
		.comparator_init(comparator_init),
		.comp_start16	(comp_start16),
		.index16 (address16),
		.addr00 (RADDR_S00), 
		.addr01	(RADDR_S01), 
		.addr02	(RADDR_S02), 
		.addr03	(RADDR_S03),
		.addr04	(RADDR_S04), 
		.addr05	(RADDR_S05), 
		.addr06	(RADDR_S06), 
		.addr07	(RADDR_S07), 
		.addr08	(RADDR_S08), 
		.addr09	(RADDR_S09), 
		.addr10	(RADDR_S10), 
		.addr11	(RADDR_S11), 
		.addr12	(RADDR_S12), 
		.addr13	(RADDR_S13), 
		.addr14	(RADDR_S14), 
		.addr15	(RADDR_S15), 
		.addr16	(RADDR_S16),
		.extra_column_en_data 	(extra_column_en),
		.read_en				(read_en),
		.data_order				(order)
		);
		
	me_datapath datapath (
		.clk 		(clk), 
		.reset		(rst), 
		.state		(state),
		.comp_start16	(comp_start16),
		.address16		(address16),
		.comparator_init (comparator_init),
		.inputData	(RDATA),		
		.outSAD41	(outSAD41), // 16X16				
		.positionSAD41	(positionSAD41)  // 16X16
		);

	assign {WDATA_S00, WDATA_S01, WDATA_S02, WDATA_S03, WDATA_S04, WDATA_S05, WDATA_S06, WDATA_S07, WDATA_S08, WDATA_S09, WDATA_S10, WDATA_S11, WDATA_S12, WDATA_S13, WDATA_S14, WDATA_S15, WDATA_S16} = WDATA_S;
	
	me_bram_search_3column search_block00 (
		.clk					(clk),
		.search_read_addr		(RADDR_S00), 
		.search_write_addr		(WADDR_S), 
		.search_write_data		(WDATA_S00), 
		.search_read			(read_en[0]),
		.search_write			(WE_S),
		.search_data_out		(RDATA_S00));

	me_bram_search_3column search_block01 (
		.clk					(clk),
		.search_read_addr		(RADDR_S01), 
		.search_write_addr		(WADDR_S), 
		.search_write_data		(WDATA_S01), 
		.search_read			(read_en[1]),
		.search_write			(WE_S),
		.search_data_out		(RDATA_S01));

	me_bram_search_3column search_block02 (
		.clk					(clk),
		.search_read_addr		(RADDR_S02), 
		.search_write_addr		(WADDR_S), 
		.search_write_data		(WDATA_S02), 
		.search_read			(read_en[2]),
		.search_write			(WE_S),
		.search_data_out		(RDATA_S02));

	me_bram_search_3column search_block03 (
		.clk					(clk),
		.search_read_addr		(RADDR_S03), 
		.search_write_addr		(WADDR_S), 
		.search_write_data		(WDATA_S03), 
		.search_read			(read_en[3]),
		.search_write			(WE_S),
		.search_data_out		(RDATA_S03));

	me_bram_search_3column search_block04 (
		.clk					(clk),
		.search_read_addr		(RADDR_S04), 
		.search_write_addr		(WADDR_S), 
		.search_write_data		(WDATA_S04), 
		.search_read			(read_en[4]),
		.search_write			(WE_S),
		.search_data_out		(RDATA_S04));

	me_bram_search_3column search_block05 (
		.clk					(clk),
		.search_read_addr		(RADDR_S05), 
		.search_write_addr		(WADDR_S), 
		.search_write_data		(WDATA_S05), 
		.search_read			(read_en[5]),
		.search_write			(WE_S),
		.search_data_out		(RDATA_S05));

	me_bram_search_3column search_block06 (
		.clk					(clk),
		.search_read_addr		(RADDR_S06), 
		.search_write_addr		(WADDR_S), 
		.search_write_data		(WDATA_S06), 
		.search_read			(read_en[6]),
		.search_write			(WE_S),
		.search_data_out		(RDATA_S06));

	me_bram_search_3column search_block07 (
		.clk					(clk),
		.search_read_addr		(RADDR_S07), 
		.search_write_addr		(WADDR_S), 
		.search_write_data		(WDATA_S07), 
		.search_read			(read_en[7]),
		.search_write			(WE_S),
		.search_data_out		(RDATA_S07));

	me_bram_search_3column search_block08 (
		.clk					(clk),
		.search_read_addr		(RADDR_S08), 
		.search_write_addr		(WADDR_S), 
		.search_write_data		(WDATA_S08), 
		.search_read			(read_en[8]),
		.search_write			(WE_S),
		.search_data_out		(RDATA_S08));

	me_bram_search_3column search_block09 (
		.clk					(clk),
		.search_read_addr		(RADDR_S09), 
		.search_write_addr		(WADDR_S), 
		.search_write_data		(WDATA_S09), 
		.search_read			(read_en[9]),
		.search_write			(WE_S),
		.search_data_out		(RDATA_S09));

	me_bram_search_3column search_block10 (
		.clk					(clk),
		.search_read_addr		(RADDR_S10), 
		.search_write_addr		(WADDR_S), 
		.search_write_data		(WDATA_S10), 
		.search_read			(read_en[10]),
		.search_write			(WE_S),
		.search_data_out		(RDATA_S10));

	me_bram_search_3column search_block11 (
		.clk					(clk),
		.search_read_addr		(RADDR_S11), 
		.search_write_addr		(WADDR_S), 
		.search_write_data		(WDATA_S11), 
		.search_read			(read_en[11]),
		.search_write			(WE_S),
		.search_data_out		(RDATA_S11));

	me_bram_search_3column search_block12 (
		.clk					(clk),
		.search_read_addr		(RADDR_S12), 
		.search_write_addr		(WADDR_S), 
		.search_write_data		(WDATA_S12), 
		.search_read			(read_en[12]),
		.search_write			(WE_S),
		.search_data_out		(RDATA_S12));

	me_bram_search_2column search_block13 (
		.clk					(clk),
		.search_read_addr		(RADDR_S13), 
		.search_write_addr		(WADDR_S[6:0]), 
		.search_write_data		(WDATA_S13), 
		.search_read			(read_en[13]),
		.search_write			(WE_S2),
		.search_data_out		(RDATA_S13));

	me_bram_search_2column search_block14 (
		.clk					(clk),
		.search_read_addr		(RADDR_S14), 
		.search_write_addr		(WADDR_S[6:0]), 
		.search_write_data		(WDATA_S14), 
		.search_read			(read_en[14]),
		.search_write			(WE_S2),
		.search_data_out		(RDATA_S14));

	me_bram_search_2column search_block15 (
		.clk					(clk),
		.search_read_addr		(RADDR_S15), 
		.search_write_addr		(WADDR_S[6:0]), 
		.search_write_data		(WDATA_S15), 
		.search_read			(read_en[15]),
		.search_write			(WE_S2),
		.search_data_out		(RDATA_S15));

	me_bram_search_2column search_block16 (
		.clk					(clk),
		.search_read_addr		(RADDR_S16), 
		.search_write_addr		(WADDR_S[6:0]), 
		.search_write_data		(WDATA_S16), 
		.search_read			(read_en[16]),
		.search_write			(WE_S2),
		.search_data_out		(RDATA_S16));
	
endmodule
