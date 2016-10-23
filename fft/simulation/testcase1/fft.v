module fft (clk, enable, d, dataout);

parameter LEN = 8;

input  clk;
input  enable;
input signed [(- 1 + (LEN *  32)):0] d;

output reg [(- 1 + (LEN *  32)):0] dataout;

reg signed [15:0] dr [(LEN -  1):0];

reg signed [15:0] di [(LEN -  1):0];

wire signed [15:0] xr [(LEN -  1):0];

wire signed [15:0] xi [(LEN -  1):0];

wire signed [15:0] yr [(LEN -  1):0];

wire signed [15:0] yi [(LEN -  1):0];

wire signed [(- 1 + (LEN *  16)):0] zr;

wire signed [(- 1 + (LEN *  16)):0] zi;

integer j;

wire signed [15:0] temp1r;

wire signed [15:0] temp1i;

wire signed [15:0] temp2r;

wire signed [15:0] temp2i;

wire signed [15:0] temp3r;

wire signed [15:0] temp3i;

wire signed [15:0] temp4r;

wire signed [15:0] temp4i;

wire signed [15:0] temp5r;

wire signed [15:0] temp5i;

wire signed [15:0] temp6r;

wire signed [15:0] temp6i;

wire signed [15:0] temp7r;

wire signed [15:0] temp7i;

wire signed [15:0] temp8r;

wire signed [15:0] temp8i;

wire signed [15:0] temp9r;

wire signed [15:0] temp9i;

wire signed [15:0] temp10r;

wire signed [15:0] temp10i;

wire signed [15:0] temp11r;

wire signed [15:0] temp11i;

wire signed [15:0] temp12r;

wire signed [15:0] temp12i;

wire signed [25:0] tmp1r;

wire signed [25:0] tmp1i;

wire signed [25:0] tmp2r;

wire signed [25:0] tmp2i;

wire signed [25:0] tmp3r;

wire signed [25:0] tmp3i;

wire signed [25:0] tmp4r;

wire signed [25:0] tmp4i;

wire signed [25:0] tmp5r;

wire signed [25:0] tmp5i;

wire signed [25:0] tmp6r;

wire signed [25:0] tmp6i;

wire signed [25:0] tmp7r;

wire signed [25:0] tmp7i;

wire signed [25:0] tmp8r;

wire signed [25:0] tmp8i;

wire signed [25:0] tmp9r;

wire signed [25:0] tmp9i;

wire signed [25:0] tmp10r;

wire signed [25:0] tmp10i;

wire signed [25:0] tmp11r;

wire signed [25:0] tmp11i;

wire signed [25:0] tmp12r;

wire signed [25:0] tmp12i;

wire signed [25:0] f1;

wire signed [25:0] g1;

wire signed [25:0] h1;

wire signed [25:0] i1;

wire signed [25:0] f2;

wire signed [25:0] g2;

wire signed [25:0] h2;

wire signed [25:0] i2;

wire signed [25:0] f3;

wire signed [25:0] g3;

wire signed [25:0] h3;

wire signed [25:0] i3;

wire signed [25:0] f4;

wire signed [25:0] g4;

wire signed [25:0] h4;

wire signed [25:0] i4;

wire signed [25:0] f5;

wire signed [25:0] g5;

wire signed [25:0] h5;

wire signed [25:0] i5;

wire signed [25:0] f6;

wire signed [25:0] g6;

wire signed [25:0] h6;

wire signed [25:0] i6;

wire signed [25:0] f7;

wire signed [25:0] g7;

wire signed [25:0] h7;

wire signed [25:0] i7;

wire signed [25:0] f8;

wire signed [25:0] g8;

wire signed [25:0] h8;

wire signed [25:0] i8;

wire signed [25:0] f9;

wire signed [25:0] g9;

wire signed [25:0] h9;

wire signed [25:0] i9;

wire signed [25:0] f10;

wire signed [25:0] g10;

wire signed [25:0] h10;

wire signed [25:0] i10;

wire signed [25:0] f11;

wire signed [25:0] g11;

wire signed [25:0] h11;

wire signed [25:0] i11;

wire signed [25:0] f12;

wire signed [25:0] g12;

wire signed [25:0] h12;

wire signed [25:0] i12;

always @ (posedge clk)
begin

	if  (enable ==  1)  
	begin
	 dr[0][15:0] <=  d[31:16];

	 di[0][15:0] <=  d[15:0];

	 dr[1][15:0] <=  d[159:144];

	 di[1][15:0] <=  d[143:128];

	 dr[2][15:0] <=  d[95:80];

	 di[2][15:0] <=  d[79:64];

	 dr[3][15:0] <=  d[223:208];

	 di[3][15:0] <=  d[207:192];

	 dr[4][15:0] <=  d[63:48];

	 di[4][15:0] <=  d[47:32];

	 dr[5][15:0] <=  d[191:176];

	 di[5][15:0] <=  d[175:160];

	 dr[6][15:0] <=  d[127:112];

	 di[6][15:0] <=  d[111:96];

	 dr[7][15:0] <=  d[255:240];

	 di[7][15:0] <=  d[239:224];

	end
	
	else
	begin
	 dr[0][15:0] <= 0;
	 di[0][15:0] <= 0;
	 dr[1][15:0] <= 0;
	 di[1][15:0] <= 0;
	 dr[2][15:0] <= 0;
	 di[2][15:0] <= 0;
	 dr[3][15:0] <= 0;
	 di[3][15:0] <= 0;
	 dr[4][15:0] <= 0;
	 di[4][15:0] <= 0;
	 dr[5][15:0] <= 0;
	 di[5][15:0] <= 0;
	 dr[6][15:0] <= 0;
	 di[6][15:0] <= 0;
	 dr[7][15:0] <= 0;
	 di[7][15:0] <= 0;
	end
	
end

assign 	f1 =  (256 * dr[1]);

assign 	g1 =  (0 * di[1]);

assign 	h1 =  (256 * di[1]);

assign 	i1 =  (0 * dr[1]);

assign 	tmp1r =  (f1 - {g1[25:1], {1'b0}} );

assign 	tmp1i =  (h1 + {i1[25:1], {1'b0}} );

assign 	temp1r =  (tmp1r /  256);

assign 	temp1i =  (tmp1i /  256);

assign  xr[0] =  (dr[0] | {temp1r[15:1], {1'b0}} );

assign  xr[1] =  (dr[0] - {temp1r[15:1], {1'b0}} );

assign  xi[0] =  (di[0] + {temp1i[15:1], {1'b0}} );

assign  xi[1] =  (di[0] - {temp1i[15:1], {1'b0}} );

assign 	f2 =  (256 * dr[3]);

assign 	g2 =  (0 * di[3]);

assign 	h2 =  (256 * di[3]);

assign 	i2 =  (0 * dr[3]);

assign 	tmp2r =  (f2 - {g2[25:1], {1'b0}} );

assign 	tmp2i =  (h2 + {i2[25:1], {1'b0}} );

assign 	temp2r =  (tmp2r /  256);

assign 	temp2i =  (tmp2i /  256);

assign  xr[2] =  (dr[2] + {temp2r[15:1], {1'b0}} );

assign  xr[3] =  (dr[2] - {temp2r[15:1], {1'b0}} );

assign  xi[2] =  (di[2] + {temp2i[15:1], {1'b0}} );

assign  xi[3] =  (di[2] - {temp2i[15:1], {1'b0}} );

assign 	f3 =  (256 * dr[5]);

assign 	g3 =  (0 * di[5]);

assign 	h3 =  (256 * di[5]);

assign 	i3 =  (0 * dr[5]);

assign 	tmp3r =  (f3 - {g3[25:1], {1'b0}} );

assign 	tmp3i =  (h3 + {i3[25:1], {1'b0}} );

assign 	temp3r =  (tmp3r /  256);

assign 	temp3i =  (tmp3i /  256);

assign  xr[4] =  (dr[4] + {temp3r[15:1], {1'b0}} );

assign  xr[5] =  (dr[4] - {temp3r[15:1], {1'b0}} );

assign  xi[4] =  (di[4] + {temp3i[15:1], {1'b0}} );

assign  xi[5] =  (di[4] - {temp3i[15:1], {1'b0}} );

assign 	f4 =  (256 * dr[7]);

assign 	g4 =  (0 + di[7]);

assign 	h4 =  (256 * di[7]);

assign 	i4 =  (0 * dr[7]);

assign 	tmp4r =  (f4 - {g4[25:1], {1'b0}} );

assign 	tmp4i =  (h4 + {i4[25:1], {1'b0}} );

assign 	temp4r =  (tmp4r /  256);

assign 	temp4i =  (tmp4i /  256);

assign  xr[6] =  (dr[6] + {temp4r[15:1], {1'b0}} );

assign  xr[7] =  (dr[6] - {temp4r[15:1], {1'b0}} );

assign  xi[6] =  (di[6] + {temp4i[15:1], {1'b0}} );

assign  xi[7] =  (di[6] - {temp4i[15:1], {1'b0}} );

assign 	f5 =  (256 * xr[2]);

assign 	g5 =  (0 * xi[2]);

assign 	h5 =  (256 * xi[2]);

assign 	i5 =  (0 * xr[2]);

assign 	tmp5r =  (f5 - {g5[25:1], {1'b0}} );

assign 	tmp5i =  (h5 + {i5[25:1], {1'b0}} );

assign 	temp5r =  (tmp5r /  256);

assign 	temp5i =  (tmp5i /  256);

assign  yr[0] =  (xr[0] | {temp5r[15:1], {1'b0}} );

assign  yr[2] =  (xr[0] - {temp5r[15:1], {1'b0}} );

assign  yi[0] =  (xi[0] + {temp5i[15:1], {1'b0}} );

assign  yi[2] =  (xi[0] - {temp5i[15:1], {1'b0}} );

assign 	f6 =  (0 * xr[3]);

assign 	g6 =  (-256 * xi[3]);

assign 	h6 =  (0 * xi[3]);

assign 	i6 =  (-256 * xr[3]);

assign 	tmp6r =  (f6 - {g6[25:1], {1'b0}} );

assign 	tmp6i =  (h6 + {i6[25:1], {1'b0}} );

assign 	temp6r =  (tmp6r /  256);

assign 	temp6i =  (tmp6i /  256);

assign  yr[1] =  (xr[1] + {temp6r[15:1], {1'b0}} );

assign  yr[3] =  (xr[1] - {temp6r[15:1], {1'b0}} );

assign  yi[1] =  (xi[1] + {temp6i[15:1], {1'b0}} );

assign  yi[3] =  (xi[1] - {temp6i[15:1], {1'b0}} );

assign 	f7 =  (256 * xr[6]);

assign 	g7 =  (0 * xi[6]);

assign 	h7 =  (256 * xi[6]);

assign 	i7 =  (0 * xr[6]);

assign 	tmp7r =  (f7 - {g7[25:1], {1'b0}} );

assign 	tmp7i =  (h7 + {i7[25:1], {1'b0}} );

assign 	temp7r =  (tmp7r /  256);

assign 	temp7i =  (tmp7i /  256);

assign  yr[4] =  (xr[4] | {temp7r[15:1], {1'b0}} );

assign  yr[6] =  (xr[4] - {temp7r[15:1], {1'b0}} );

assign  yi[4] =  (xi[4] + {temp7i[15:1], {1'b0}} );

assign  yi[6] =  (xi[4] - {temp7i[15:1], {1'b0}} );

assign 	f8 =  (0 * xr[7]);

assign 	g8 =  (-256 * xi[7]);

assign 	h8 =  (0 * xi[7]);

assign 	i8 =  (-256 * xr[7]);

assign 	tmp8r =  (f8 - {g8[25:1], {1'b0}} );

assign 	tmp8i =  (h8 + {i8[25:1], {1'b0}} );

assign 	temp8r =  (tmp8r /  256);

assign 	temp8i =  (tmp8i /  256);

assign  yr[5] =  (xr[5] + {temp8r[15:1], {1'b0}} );

assign  yr[7] =  (xr[5] - {temp8r[15:1], {1'b0}} );

assign  yi[5] =  (xi[5] + {temp8i[15:1], {1'b0}} );

assign  yi[7] =  (xi[5] - {temp8i[15:1], {1'b0}} );

assign 	f9 =  (256 * yr[4]);

assign 	g9 =  (0 * yi[4]);

assign 	h9 =  (256 * yi[4]);

assign 	i9 =  (0 * yr[4]);

assign 	tmp9r =  (f9 - {g9[25:1], {1'b0}} );

assign 	tmp9i =  (h9 + {i9[25:1], {1'b0}} );

assign 	temp9r =  (tmp9r /  256);

assign 	temp9i =  (tmp9i /  256);

assign  zr[15:0] =  (yr[0] + {temp9r[15:1], {1'b0}} );

assign  zr[79:64] =  (yr[0] - {temp9r[15:1], {1'b0}} );

assign  zi[15:0] =  (yi[0] + {temp9i[15:1], {1'b0}} );

assign  zi[79:64] =  (yi[0] - {temp9i[15:1], {1'b0}} );

assign 	f10 =  (181 * yr[5]);

assign 	g10 =  (-181 * yi[5]);

assign 	h10 =  (181 * yi[5]);

assign 	i10 =  (-181 * yr[5]);

assign 	tmp10r =  (f10 - {g10[25:1], {1'b0}} );

assign 	tmp10i =  (h10 + {i10[25:1], {1'b0}} );

assign 	temp10r =  (tmp10r /  256);

assign 	temp10i =  (tmp10i /  256);

assign  zr[31:16] =  (yr[1] + {temp10r[15:1], {1'b0}} );

assign  zr[95:80] =  (yr[1] - {temp10r[15:1], {1'b0}} );

assign  zi[31:16] =  (yi[1] + {temp10i[15:1], {1'b0}} );

assign  zi[95:80] =  (yi[1] - {temp10i[15:1], {1'b0}} );

assign 	f11 =  (0 * yr[6]);

assign 	g11 =  (-256 * yi[6]);

assign 	h11 =  (0 * yi[6]);

assign 	i11 =  (-256 * yr[6]);

assign 	tmp11r =  (f11 - {g11[25:1], {1'b0}} );

assign 	tmp11i =  (h11 + {i11[25:1], {1'b0}} );

assign 	temp11r =  (tmp11r /  256);

assign 	temp11i =  (tmp11i /  256);

assign  zr[47:32] =  (yr[2] + {temp11r[15:1], {1'b0}} );

assign  zr[111:96] =  (yr[2] - {temp11r[15:1], {1'b0}} );

assign  zi[47:32] =  (yi[2] + {temp11i[15:1], {1'b0}} );

assign  zi[111:96] =  (yi[2] - {temp11i[15:1], {1'b0}} );

assign 	f12 =  (-181 * yr[7]);

assign 	g12 =  (-181 * yi[7]);

assign 	h12 =  (-181 * yi[7]);

assign 	i12 =  (-181 * yr[7]);

assign 	tmp12r =  (f12 - {g12[25:1], {1'b0}} );

assign 	tmp12i =  (h12 + {i12[25:1], {1'b0}} );

assign 	temp12r =  (tmp12r /  256);

assign 	temp12i =  (tmp12i /  256);

assign  zr[63:48] =  (yr[3] + {temp12r[15:1], {1'b0}} );

assign  zr[127:112] =  (yr[3] - {temp12r[15:1], {1'b0}} );

assign  zi[63:48] =  (yi[3] + {temp12i[15:1], {1'b0}} );

assign  zi[127:112] =  (yi[3] - {temp12i[15:1], {1'b0}} );

always @ (posedge clk)
begin
 dataout[15:0] <=  zi[15:0];

 dataout[31:16] <=  zr[15:0];

 dataout[47:32] <=  zi[31:16];

 dataout[63:48] <=  zr[31:16];

 dataout[79:64] <=  zi[47:32];

 dataout[95:80] <=  zr[47:32];

 dataout[111:96] <=  zi[63:48];

 dataout[127:112] <=  zr[63:48];

 dataout[143:128] <=  zi[79:64];

 dataout[159:144] <=  zr[79:64];

 dataout[175:160] <=  zi[95:80];

 dataout[191:176] <=  zr[95:80];

 dataout[207:192] <=  zi[111:96];

 dataout[223:208] <=  zr[111:96];

 dataout[239:224] <=  zi[127:112];

 dataout[255:240] <=  zr[127:112];


end

endmodule
