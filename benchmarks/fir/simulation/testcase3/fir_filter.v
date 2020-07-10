module fir_filter (clk, reset, data_in, data_out, coef_in, load_c);

parameter TAPS = 25;
parameter coefWidth = 16;

input  clk;
input  reset;
input [7:0] data_in;
input [(coefWidth -  1):0] coef_in;
input  load_c;

output [14:0] data_out; //was [13:0]

wire  clk;

wire  reset;

wire [7:0] data_in;

wire [(coefWidth -  1):0] coef_in;

wire  load_c;

reg [31:0] temp;

reg [14:0] data_out;

reg [15:0] r_storage [(TAPS -  1):0]; //rstorage=[7:0]

reg [15:0] coef_storage [(TAPS -  1):0];

integer i;

reg [31:0] product_array [(TAPS -  1):0];

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

		
	/* this is the part of the code that represents the accumulator*/
	temp = 0;
	temp = temp + product_array[0];
	temp = temp + product_array[1];
	temp = temp + product_array[2];
	temp = temp + product_array[3];
	temp = temp + product_array[4];
	temp = temp + product_array[5];
	temp = temp + product_array[6];
	temp = temp + product_array[7];
	temp = temp + product_array[8];
	temp = temp + product_array[9];
	temp = temp + product_array[10];
	temp = temp + product_array[11];
	temp = temp + product_array[12];
	temp = temp + product_array[13];
	temp = ( temp + product_array[14] );
	temp = (temp + product_array[15]);
	temp = temp + product_array[16];
	temp = temp + product_array[17];
	temp = temp + product_array[18];
	temp = temp + product_array[19];
	temp = temp + product_array[20];
	temp = temp + product_array[21];
	temp = temp + product_array[22];
	temp = temp + product_array[23];
	temp = temp + product_array[24];
	
	data_out <= temp[14:0]; //was [13:0]

	
	end	
  end	
	

endmodule
