module fft_test(clk, enable, wt);
parameter LEN=8;
input clk, enable, wt;

integer j,f;
reg signed[LEN*32-1:0] d;
wire signed[LEN*32-1:0] dataout;
reg signed[31:0]data[LEN-1:0];

initial
begin
    $readmemh("infft.txt",data);
    for (j = 0; j < LEN; j = j+1) begin
        d[32*j+:32]=data[j];
    end
end

fft DUT(.clk(clk), .enable(enable),.d(d),.dataout(dataout));

always@(posedge wt)
begin
	f = $fopen("outfft.txt", "w");
	for (j = 0; j <LEN; j = j + 1) begin
        $fdisplay(f, "%x",dataout[32*j+:32]);
	end
	$fclose(f);
end

endmodule

