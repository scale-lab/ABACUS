`timescale 1ns / 1ps
module me_control(	//INPUTS 
					clk, reset, start, stop,
					//OUTPUTS
					DE_out, state, comparator_init, comp_start16, index16, 
					addr00, addr01, addr02, addr03, addr04, addr05, addr06, addr07, addr08, addr09, addr10, addr11, addr12, addr13, addr14, addr15, addr16,
					extra_column_en_data, read_en, data_order
					);

/////// INSTANTIATIONS ///////
		
	input clk, reset, start, stop;
				
	// S0 = idle
	// S1 = wait for current window
	// S2 = wait for search window
	// S3 = calculate SADs (down shift)
	// S4 = calculate SADs (right shift)
	// S5 = calculate SADs (up shift)
	// S6 = calculate SADs (right shift)
	// S7 = computation finished
	parameter S0 = 0, S1 = 1, S2 = 2, S3 = 3, S4 = 4, S5 = 5, S6 = 6, S7 = 7;
	reg [2:0] current_state, next_state;
	
/////// COUNTERS ///////	
	reg [3:0] count_current;
	always @ (posedge clk, posedge reset)
	begin
		if (reset)
			count_current <= 1'd0;
		else if (current_state == S1)
			count_current <= count_current + 1'd1;
		else if (current_state == S7)
			count_current <= 1'd0;
	end

	reg [3:0] count_search;
	always @ (posedge clk, posedge reset)
	begin
		if (reset)
			count_search <= 1'd0;
		else if (current_state == S2)
			count_search <= count_search + 1'd1;
		else if (current_state == S7)
			count_search <= 1'd0;
	end

	reg [4:0] count_vertical;	
	always @ (posedge clk, posedge reset)
	begin
		if (reset)
			count_vertical <= 1'd0;
		else if (current_state == 3'd3 || current_state == 3'd4 || current_state == 3'd5 || current_state == 3'd6)
			count_vertical <= count_vertical + 1'd1;
		else if (current_state == S7)
			count_vertical <= 1'd0;
	end
	
	reg [4:0] count_yaxis;
	always @ (posedge clk, posedge reset)
	begin
		if (reset)
			count_yaxis <= 1'd0;
		else if (current_state == S3)
			count_yaxis <= count_yaxis + 1'd1;
		else if (current_state == S5)
			count_yaxis <= count_yaxis - 1'd1;
		else if (current_state == S7)
			count_yaxis <= 1'd0;
	end	

	reg [4:0] count_xaxis;
	always @ (posedge clk, posedge reset)
	begin
		if (reset)
			count_xaxis <= 1'd0;
		else if (current_state == S4 || current_state == S6)
			count_xaxis <= count_xaxis + 1'd1;
		else if (current_state == S7)
			count_xaxis <= 1'd0;
	end
		
/////// STATE MACHINE ///////

	always @ (posedge clk, posedge reset)
	begin
		if (reset)
			current_state <= 3'd0;
		else if (stop)
			current_state <= S7;
		else
			current_state <= next_state;
	end
	
	always @*
	begin
		if (current_state == S0)
		begin
			if (start)
				next_state = S1;
			else
				next_state = S0;
		end
		else if (current_state == S1)
		begin
			if (count_current == 4'd15)
				next_state = S2;
			else
				next_state = S1;		
		end
		else if (current_state == S2)
		begin
			if (count_search == 4'd15)
				next_state = S3;
			else
				next_state = S2;
		end
		else if (current_state == S3)
		begin 
			if (count_vertical == 5'd30)
				next_state = S4;
			else
				next_state = S3;		
		end
		else if (current_state == S4)
		begin
			next_state = S5;
		end
		else if (current_state == S5)
		begin
			if (count_vertical == 5'd30)
				next_state = S6;
			else
				next_state = S5;
		end
		else if (current_state == S6)
		begin
			if (count_xaxis == 5'd31)
				next_state = S7;
			else
				next_state = S3;
		end
		else // if (current_state == S7)
		begin
			if (start)
				next_state = S1;
			else
				next_state = S7;	
		end
	end
	
/////// ENABLE SIGNALS ///////
	
	output reg [2:0] state; // current state is delayed to match reading data from mem	
	always @ (posedge clk)
	begin
		state <= current_state;
	end		

	output reg DE_out;	
	always @*	
	begin		
		if (state == S7)
			DE_out = 1'b1;
		else
			DE_out = 1'b0;
	end
	
	output reg comparator_init; // re-intantiate comparator
	always @ (posedge clk)
	begin
		if (state == 3'd2)
			comparator_init <= 1'd1;
		else
			comparator_init <= 1'd0;
	end

	
	output comp_start16;
	reg comp_start, start_d0, start_d1, start_d2, start_d3;	
		
	always @ (posedge clk)
	begin
		if (state == 3'd3 || state == 3'd4 || state == 3'd5 || state == 3'd6)
			comp_start <= 1'b1;
		else
			comp_start <= 1'b0;
	end
	
	always @ (posedge clk)
	begin
		start_d0 <= comp_start;
		start_d1 <= start_d0;
		start_d2 <= start_d1;
		start_d3 <= start_d2;
	end
	
	assign comp_start16 = start_d3;
	
	output [9:0] index16;
	reg [9:0] index_d0, index_d1, index_d2, index_d3, index_d4, index_d5;
	
	always @ (posedge clk)
	begin
		index_d0 <= {count_xaxis, count_yaxis};
		index_d1 <= index_d0;
		index_d2 <= index_d1;
		index_d3 <= index_d2;
		index_d4 <= index_d3;
		index_d5 <= index_d4;
	end
	
	assign index16 = index_d5;

/////// ADDRESS GENERATION ///////	
	
	reg [6:0] address_a;
	always @*
	begin
		if (current_state == S2)
		begin
			address_a = count_search;
		end
		else if (count_xaxis > 5'd16)
		begin
			if (current_state == S3)
				address_a = 6'd63 + count_vertical;
			else // if (current_state == S5)
				address_a = 7'd77 - count_vertical;
		end
		else // count_xaxis < 5'd16
		begin
			if (current_state == S3)
				address_a = 5'd16 + count_vertical;
			else // if (current_state == S5)
				address_a = 5'd30 - count_vertical;
		end
	end
	
	wire [7:0] address_b;
	assign address_b = address_a + 6'd47;
	
	wire [4:0] order;
	assign order = (count_xaxis > 5'd16) ? (count_xaxis - 5'd17) : count_xaxis;
	
	output reg [4:0] data_order;
	always @ (posedge clk)
	begin
		data_order <= order;
	end
	
	wire extra_column_en_addr;	
	assign extra_column_en_addr = (count_vertical > 5'd14 && count_xaxis != 5'd31) ? 1'b1 : 1'b0;
	
	output reg extra_column_en_data;
	always @ (posedge clk)
	begin
		extra_column_en_data <= extra_column_en_addr;
	end
	
	wire [16:0] read_en_0;
	reg [16:0] read_en_2, read_en_4, read_en_8, read_en_16;
	
	assign read_en_0 = 17'h1FFFE; // 17'b1_1111_1111_1111_1110;
	
	output reg [16:0] read_en;
	always @*
	begin
		if (current_state == S2 || current_state == S3 || current_state == S5)
		begin
			if (extra_column_en_addr)
				read_en = 17'h1FFFF;
			else
			begin
				if (order[4])
					read_en_16 = {read_en_0[15:0], read_en_0[16]};
				else
					read_en_16 = read_en_0;
				
				if	(order[3])
					read_en_8 = {read_en_16[7:0], read_en_16[16:8]};
				else
					read_en_8 = read_en_16;
				
				if (order[2])
					read_en_4 = {read_en_8[3:0], read_en_8[16:4]};
				else
					read_en_4 = read_en_8;
				
				if (order[1])
					read_en_2 = {read_en_4[1:0], read_en_4[16:2]};
				else
					read_en_2 = read_en_4;
							
				if (order[0])
					read_en = {read_en_2[0], read_en_2[16:1]};
				else
					read_en = read_en_2;
			end
		end
		else
			read_en = 17'd0;
	end
	
	output [7:0] addr00, addr01, addr02, addr03, addr04, addr05, addr06, addr07, addr08, addr09, addr10, addr11, addr12;
	output [6:0] addr13, addr14, addr15, addr16;
	
	assign addr00 = (order == 5'd0) ? address_a : address_b;
	assign addr01 = (order < 5'd2) ? address_a : address_b;
	assign addr02 = (order < 5'd3) ? address_a : address_b;
	assign addr03 = (order < 5'd4) ? address_a : address_b;
	assign addr04 = (order < 5'd5) ? address_a : address_b;
	assign addr05 = (order < 5'd6) ? address_a : address_b;
	assign addr06 = (order < 5'd7) ? address_a : address_b;
	assign addr07 = (order < 5'd8) ? address_a : address_b;
	assign addr08 = (order < 5'd9) ? address_a : address_b;
	assign addr09 = (order < 5'd10) ? address_a : address_b;
	assign addr10 = (order < 5'd11) ? address_a : address_b;
	assign addr11 = (order < 5'd12) ? address_a : address_b;
	assign addr12 = (order < 5'd13) ? address_a : address_b;	
	assign addr13 = (order < 5'd14) ? address_a : address_b;
	assign addr14 = (order < 5'd15) ? address_a : address_b;
	assign addr15 = (order == 5'd16) ? address_b : address_a;
	assign addr16 = address_a;	

endmodule
