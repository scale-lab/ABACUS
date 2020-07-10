// This is the top Module for FIR Filter algorithm.
module fir_filter (clk, reset, data_in, data_out, coef_in, load_c);

parameter TAPS = 25;
parameter coefWidth = 16;

input  clk;
input  reset;
input [7:0] data_in;
input [(coefWidth -  1):0] coef_in;
input  load_c;

output reg [14:0] data_out;

wire  clk;

wire  reset;

wire [7:0] data_in;

wire [(coefWidth -  1):0] coef_in;

wire  load_c;

reg signed [15:0] r_storage [(TAPS -  1):0];
reg signed [15:0] coef_storage [(TAPS -  1):0];

reg signed [31:0] product_array [(TAPS -  1):0];

wire signed [14:0] temp1,temp2,temp3,temp4,temp5,temp6;
wire signed [14:0] temp7,temp8,temp9,temp10,temp11,temp12;
wire signed [14:0] temp13,temp14,temp15,temp16,temp17,temp18;
wire signed [14:0] temp19,temp20,temp21,temp22,temp23,temp24;

always @ (posedge clk)
begin
 product_array[0] <=  (r_storage[0] * coef_storage[0]);

 product_array[1] <=  (r_storage[1] * coef_storage[1]);

 product_array[2] <=  (r_storage[2] * coef_storage[2]);

 product_array[3] <=  (r_storage[3] * coef_storage[3]);

 product_array[4] <=  (r_storage[4] * coef_storage[4]);

 product_array[5] <=  (r_storage[5] * coef_storage[5]);

 product_array[6] <=  (r_storage[6] * coef_storage[6]);

 product_array[7] <=  (r_storage[7] * coef_storage[7]);

 product_array[8] <=  (r_storage[8] * coef_storage[8]);

 product_array[9] <=  (r_storage[9] * coef_storage[9]);

 product_array[10] <=  (r_storage[10] * coef_storage[10]);

 product_array[11] <=  (r_storage[11] * coef_storage[11]);

 product_array[12] <=  (r_storage[12] * coef_storage[12]);

 product_array[13] <=  (r_storage[13] * coef_storage[13]);

 product_array[14] <=  (r_storage[14] * coef_storage[14]);

 product_array[15] <=  (r_storage[15] * coef_storage[15]);

 product_array[16] <=  (r_storage[16] * coef_storage[16]);

 product_array[17] <=  (r_storage[17] * coef_storage[17]);

 product_array[18] <=  (r_storage[18] * coef_storage[18]);

 product_array[19] <=  (r_storage[19] * coef_storage[19]);

 product_array[20] <=  (r_storage[20] * coef_storage[20]);

 product_array[21] <=  (r_storage[21] * coef_storage[21]);

 product_array[22] <=  (r_storage[22] * coef_storage[22]);

 product_array[23] <=  (r_storage[23] * coef_storage[23]);

 product_array[24] <=  (r_storage[24] * coef_storage[24]);


end

always @ (posedge clk)
begin

	if  (reset ==  1'b1)  
	begin
	 r_storage[0] <= 16'b0000000000000000;
	 r_storage[1] <= 16'b0000000000000000;
	 r_storage[2] <= 16'b0000000000000000;
	 r_storage[3] <= 16'b0000000000000000;
	 r_storage[4] <= 16'b0000000000000000;
	 r_storage[5] <= 16'b0000000000000000;
	 r_storage[6] <= 16'b0000000000000000;
	 r_storage[7] <= 16'b0000000000000000;
	 r_storage[8] <= 16'b0000000000000000;
	 r_storage[9] <= 16'b0000000000000000;
	 r_storage[10] <= 16'b0000000000000000;
	 r_storage[11] <= 16'b0000000000000000;
	 r_storage[12] <= 16'b0000000000000000;
	 r_storage[13] <= 16'b0000000000000000;
	 r_storage[14] <= 16'b0000000000000000;
	 r_storage[15] <= 16'b0000000000000000;
	 r_storage[16] <= 16'b0000000000000000;
	 r_storage[17] <= 16'b0000000000000000;
	 r_storage[18] <= 16'b0000000000000000;
	 r_storage[19] <= 16'b0000000000000000;
	 r_storage[20] <= 16'b0000000000000000;
	 r_storage[21] <= 16'b0000000000000000;
	 r_storage[22] <= 16'b0000000000000000;
	 r_storage[23] <= 16'b0000000000000000;
	 r_storage[24] <= 16'b0000000000000000;
	end
	
	else 
	if  (load_c ==  1'b1)  
	begin
	 coef_storage[1] <=  coef_storage[0];

	 coef_storage[2] <=  coef_storage[1];

	 coef_storage[3] <=  coef_storage[2];

	 coef_storage[4] <=  coef_storage[3];

	 coef_storage[5] <=  coef_storage[4];

	 coef_storage[6] <=  coef_storage[5];

	 coef_storage[7] <=  coef_storage[6];

	 coef_storage[8] <=  coef_storage[7];

	 coef_storage[9] <=  coef_storage[8];

	 coef_storage[10] <=  coef_storage[9];

	 coef_storage[11] <=  coef_storage[10];

	 coef_storage[12] <=  coef_storage[11];

	 coef_storage[13] <=  coef_storage[12];

	 coef_storage[14] <=  coef_storage[13];

	 coef_storage[15] <= coef_storage[14];

	 coef_storage[16] <=  coef_storage[15];

	 coef_storage[17] <=  coef_storage[16];

	 coef_storage[18] <=  coef_storage[17];

	 coef_storage[19] <=  coef_storage[18];

	 coef_storage[20] <=  coef_storage[19];

	 coef_storage[21] <=  coef_storage[20];

	 coef_storage[22] <=  coef_storage[21];

	 coef_storage[23] <=  coef_storage[22];

	 coef_storage[24] <=  coef_storage[23];

	 coef_storage[0] <= $signed(coef_in);
	end
	
	else
	begin
	 r_storage[1] <=  r_storage[0];

	 r_storage[2] <=  r_storage[1];

	 r_storage[3] <=  r_storage[2];

	 r_storage[4] <=  r_storage[3];

	 r_storage[5] <=  r_storage[4];

	 r_storage[6] <=  r_storage[5];

	 r_storage[7] <=  r_storage[6];

	 r_storage[8] <=  r_storage[7];

	 r_storage[9] <=  r_storage[8];

	 r_storage[10] <=  r_storage[9];

	 r_storage[11] <=  r_storage[10];

	 r_storage[12] <=  r_storage[11];

	 r_storage[13] <=  r_storage[12];

	 r_storage[14] <=  r_storage[13];

	 r_storage[15] <= r_storage[14];

	 r_storage[16] <=  r_storage[15];

	 r_storage[17] <=  r_storage[16];

	 r_storage[18] <=  r_storage[17];

	 r_storage[19] <=  r_storage[18];

	 r_storage[20] <=  r_storage[19];

	 r_storage[21] <=  r_storage[20];

	 r_storage[22] <=  r_storage[21];

	 r_storage[23] <=  r_storage[22];

	 r_storage[24] <=  r_storage[23];

	 r_storage[0] <= $signed(data_in);

	data_out <= temp24[14:0];

	end	
  end

/* this is the part of the code that represents the accumulator*/
assign temp1 = product_array[0] + product_array[1];
assign temp2 = product_array[2] + product_array[3];
assign temp3 = product_array[4] + product_array[5];
assign temp4 = product_array[6] + product_array[7];
assign temp5 = product_array[8] + product_array[9];
assign temp6 = product_array[10] + product_array[11];
assign temp7 = product_array[12] + product_array[13];
assign temp8 = product_array[14] + product_array[15];
assign temp9 = product_array[16] + product_array[17];
assign temp10 = product_array[18] + product_array[19];
assign temp11 = product_array[20] + product_array[21];
assign temp12 = product_array[22] + product_array[23];//first level finished.
assign temp13 = temp1 + temp2;
assign temp14 = temp3 + temp4;
assign temp15 = temp5 + temp6;
assign temp16 = temp7 + temp8;
assign temp17 = temp9 + temp10;
assign temp18 = temp11 + temp12;//second level finished.
assign temp19 = temp13 + temp14;
assign temp20 = temp15 + temp16;
assign temp21 = temp17 + temp18;//third level finished.
assign temp22 = temp19 + temp20;
assign temp23 = temp21 + product_array[24];
assign temp24 = temp22 + temp23;

endmodule
