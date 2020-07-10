`timescale 1ns/1ps
module me_bram_search_2column(  //INPUTS
                    clk, 
                    search_read_addr, 
                    search_write_addr, 
                    search_write_data, 
					search_read, 
                    search_write, 
                    //OUTPUT
                    search_data_out);

input clk;
input [6:0] search_read_addr;
input [6:0] search_write_addr;
input [7:0] search_write_data;
input search_read, search_write;

output [7:0] search_data_out;

reg [6:0] search_read_addr_d;

reg [7:0] reg_file[0:93];

assign search_data_out = reg_file[search_read_addr_d];
 
always @(posedge clk)
begin
   if (search_read) search_read_addr_d <= search_read_addr;
   if (search_write) reg_file[search_write_addr] <= search_write_data;
end

endmodule
