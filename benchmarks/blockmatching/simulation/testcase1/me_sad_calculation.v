module me_sad_calculation (clk, search, current, SAD01, SAD02, SAD03, SAD04, SAD05, SAD06, SAD07, SAD08, SAD09, SAD10, SAD11, SAD12, SAD13, SAD14, SAD15, SAD16);


input  clk;
input [2175:0] search;
input [2047:0] current;

output reg [11:0] SAD01;
output reg [11:0] SAD02;
output reg [11:0] SAD03;
output reg [11:0] SAD04;
output reg [11:0] SAD05;
output reg [11:0] SAD06;
output reg [11:0] SAD07;
output reg [11:0] SAD08;
output reg [11:0] SAD09;
output reg [11:0] SAD10;
output reg [11:0] SAD11;
output reg [11:0] SAD12;
output reg [11:0] SAD13;
output reg [11:0] SAD14;
output reg [11:0] SAD15;
output reg [11:0] SAD16;

wire [7:0] difference_matrix [15:0][15:0];

reg [9:0] add01;

reg [9:0] add02;

reg [9:0] add03;

reg [9:0] add04;

reg [9:0] add05;

reg [9:0] add06;

reg [9:0] add07;

reg [9:0] add08;

reg [9:0] add09;

reg [9:0] add10;

reg [9:0] add11;

reg [9:0] add12;

reg [9:0] add13;

reg [9:0] add14;

reg [9:0] add15;

reg [9:0] add16;

reg [9:0] add17;

reg [9:0] add18;

reg [9:0] add19;

reg [9:0] add20;

reg [9:0] add21;

reg [9:0] add22;

reg [9:0] add23;

reg [9:0] add24;

reg [9:0] add25;

reg [9:0] add26;

reg [9:0] add27;

reg [9:0] add28;

reg [9:0] add29;

reg [9:0] add30;

reg [9:0] add31;

reg [9:0] add32;

reg [9:0] add33;

reg [9:0] add34;

reg [9:0] add35;

reg [9:0] add36;

reg [9:0] add37;

reg [9:0] add38;

reg [9:0] add39;

reg [9:0] add40;

reg [9:0] add41;

reg [9:0] add42;

reg [9:0] add43;

reg [9:0] add44;

reg [9:0] add45;

reg [9:0] add46;

reg [9:0] add47;

reg [9:0] add48;

reg [9:0] add49;

reg [9:0] add50;

reg [9:0] add51;

reg [9:0] add52;

reg [9:0] add53;

reg [9:0] add54;

reg [9:0] add55;

reg [9:0] add56;

reg [9:0] add57;

reg [9:0] add58;

reg [9:0] add59;

reg [9:0] add60;

reg [9:0] add61;

reg [9:0] add62;

reg [9:0] add63;

reg [9:0] add64;

me_PE PE_ij256(	.clk(clk), 
 		.A(search[15:8]),
	 	.B(current[7:0]),
	 	.AD(difference_matrix[0][0]));

me_PE PE_ij241(	.clk(clk), 
 		.A(search[23:16]),
	 	.B(current[15:8]),
	 	.AD(difference_matrix[0][1]));

me_PE PE_ij242(	.clk(clk), 
 		.A(search[31:24]),
	 	.B(current[23:16]),
	 	.AD(difference_matrix[0][2]));

me_PE PE_ij243(	.clk(clk), 
 		.A(search[39:32]),
	 	.B(current[31:24]),
	 	.AD(difference_matrix[0][3]));

me_PE PE_ij244(	.clk(clk), 
 		.A(search[47:40]),
	 	.B(current[39:32]),
	 	.AD(difference_matrix[0][4]));

me_PE PE_ij245(	.clk(clk), 
 		.A(search[55:48]),
	 	.B(current[47:40]),
	 	.AD(difference_matrix[0][5]));

me_PE PE_ij246(	.clk(clk), 
 		.A(search[63:56]),
	 	.B(current[55:48]),
	 	.AD(difference_matrix[0][6]));

me_PE PE_ij247(	.clk(clk), 
 		.A(search[71:64]),
	 	.B(current[63:56]),
	 	.AD(difference_matrix[0][7]));

me_PE PE_ij248(	.clk(clk), 
 		.A(search[79:72]),
	 	.B(current[71:64]),
	 	.AD(difference_matrix[0][8]));

me_PE PE_ij249(	.clk(clk), 
 		.A(search[87:80]),
	 	.B(current[79:72]),
	 	.AD(difference_matrix[0][9]));

me_PE PE_ij250(	.clk(clk), 
 		.A(search[95:88]),
	 	.B(current[87:80]),
	 	.AD(difference_matrix[0][10]));

me_PE PE_ij251(	.clk(clk), 
 		.A(search[103:96]),
	 	.B(current[95:88]),
	 	.AD(difference_matrix[0][11]));

me_PE PE_ij252(	.clk(clk), 
 		.A(search[111:104]),
	 	.B(current[103:96]),
	 	.AD(difference_matrix[0][12]));

me_PE PE_ij253(	.clk(clk), 
 		.A(search[119:112]),
	 	.B(current[111:104]),
	 	.AD(difference_matrix[0][13]));

me_PE PE_ij254(	.clk(clk), 
 		.A(search[127:120]),
	 	.B(current[119:112]),
	 	.AD(difference_matrix[0][14]));

me_PE PE_ij255(	.clk(clk), 
 		.A(search[135:128]),
	 	.B(current[127:120]),
	 	.AD(difference_matrix[0][15]));

me_PE PE_ij16(	.clk(clk), 
 		.A(search[151:144]),
	 	.B(current[135:128]),
	 	.AD(difference_matrix[1][0]));

me_PE PE_ij1(	.clk(clk), 
 		.A(search[159:152]),
	 	.B(current[143:136]),
	 	.AD(difference_matrix[1][1]));

me_PE PE_ij2(	.clk(clk), 
 		.A(search[167:160]),
	 	.B(current[151:144]),
	 	.AD(difference_matrix[1][2]));

me_PE PE_ij3(	.clk(clk), 
 		.A(search[175:168]),
	 	.B(current[159:152]),
	 	.AD(difference_matrix[1][3]));

me_PE PE_ij4(	.clk(clk), 
 		.A(search[183:176]),
	 	.B(current[167:160]),
	 	.AD(difference_matrix[1][4]));

me_PE PE_ij5(	.clk(clk), 
 		.A(search[191:184]),
	 	.B(current[175:168]),
	 	.AD(difference_matrix[1][5]));

me_PE PE_ij6(	.clk(clk), 
 		.A(search[199:192]),
	 	.B(current[183:176]),
	 	.AD(difference_matrix[1][6]));

me_PE PE_ij7(	.clk(clk), 
 		.A(search[207:200]),
	 	.B(current[191:184]),
	 	.AD(difference_matrix[1][7]));

me_PE PE_ij8(	.clk(clk), 
 		.A(search[215:208]),
	 	.B(current[199:192]),
	 	.AD(difference_matrix[1][8]));

me_PE PE_ij9(	.clk(clk), 
 		.A(search[223:216]),
	 	.B(current[207:200]),
	 	.AD(difference_matrix[1][9]));

me_PE PE_ij10(	.clk(clk), 
 		.A(search[231:224]),
	 	.B(current[215:208]),
	 	.AD(difference_matrix[1][10]));

me_PE PE_ij11(	.clk(clk), 
 		.A(search[239:232]),
	 	.B(current[223:216]),
	 	.AD(difference_matrix[1][11]));

me_PE PE_ij12(	.clk(clk), 
 		.A(search[247:240]),
	 	.B(current[231:224]),
	 	.AD(difference_matrix[1][12]));

me_PE PE_ij13(	.clk(clk), 
 		.A(search[255:248]),
	 	.B(current[239:232]),
	 	.AD(difference_matrix[1][13]));

me_PE PE_ij14(	.clk(clk), 
 		.A(search[263:256]),
	 	.B(current[247:240]),
	 	.AD(difference_matrix[1][14]));

me_PE PE_ij15(	.clk(clk), 
 		.A(search[271:264]),
	 	.B(current[255:248]),
	 	.AD(difference_matrix[1][15]));

me_PE PE_ij32(	.clk(clk), 
 		.A(search[287:280]),
	 	.B(current[263:256]),
	 	.AD(difference_matrix[2][0]));

me_PE PE_ij17(	.clk(clk), 
 		.A(search[295:288]),
	 	.B(current[271:264]),
	 	.AD(difference_matrix[2][1]));

me_PE PE_ij18(	.clk(clk), 
 		.A(search[303:296]),
	 	.B(current[279:272]),
	 	.AD(difference_matrix[2][2]));

me_PE PE_ij19(	.clk(clk), 
 		.A(search[311:304]),
	 	.B(current[287:280]),
	 	.AD(difference_matrix[2][3]));

me_PE PE_ij20(	.clk(clk), 
 		.A(search[319:312]),
	 	.B(current[295:288]),
	 	.AD(difference_matrix[2][4]));

me_PE PE_ij21(	.clk(clk), 
 		.A(search[327:320]),
	 	.B(current[303:296]),
	 	.AD(difference_matrix[2][5]));

me_PE PE_ij22(	.clk(clk), 
 		.A(search[335:328]),
	 	.B(current[311:304]),
	 	.AD(difference_matrix[2][6]));

me_PE PE_ij23(	.clk(clk), 
 		.A(search[343:336]),
	 	.B(current[319:312]),
	 	.AD(difference_matrix[2][7]));

me_PE PE_ij24(	.clk(clk), 
 		.A(search[351:344]),
	 	.B(current[327:320]),
	 	.AD(difference_matrix[2][8]));

me_PE PE_ij25(	.clk(clk), 
 		.A(search[359:352]),
	 	.B(current[335:328]),
	 	.AD(difference_matrix[2][9]));

me_PE PE_ij26(	.clk(clk), 
 		.A(search[367:360]),
	 	.B(current[343:336]),
	 	.AD(difference_matrix[2][10]));

me_PE PE_ij27(	.clk(clk), 
 		.A(search[375:368]),
	 	.B(current[351:344]),
	 	.AD(difference_matrix[2][11]));

me_PE PE_ij28(	.clk(clk), 
 		.A(search[383:376]),
	 	.B(current[359:352]),
	 	.AD(difference_matrix[2][12]));

me_PE PE_ij29(	.clk(clk), 
 		.A(search[391:384]),
	 	.B(current[367:360]),
	 	.AD(difference_matrix[2][13]));

me_PE PE_ij30(	.clk(clk), 
 		.A(search[399:392]),
	 	.B(current[375:368]),
	 	.AD(difference_matrix[2][14]));

me_PE PE_ij31(	.clk(clk), 
 		.A(search[407:400]),
	 	.B(current[383:376]),
	 	.AD(difference_matrix[2][15]));

me_PE PE_ij48(	.clk(clk), 
 		.A(search[423:416]),
	 	.B(current[391:384]),
	 	.AD(difference_matrix[3][0]));

me_PE PE_ij33(	.clk(clk), 
 		.A(search[431:424]),
	 	.B(current[399:392]),
	 	.AD(difference_matrix[3][1]));

me_PE PE_ij34(	.clk(clk), 
 		.A(search[439:432]),
	 	.B(current[407:400]),
	 	.AD(difference_matrix[3][2]));

me_PE PE_ij35(	.clk(clk), 
 		.A(search[447:440]),
	 	.B(current[415:408]),
	 	.AD(difference_matrix[3][3]));

me_PE PE_ij36(	.clk(clk), 
 		.A(search[455:448]),
	 	.B(current[423:416]),
	 	.AD(difference_matrix[3][4]));

me_PE PE_ij37(	.clk(clk), 
 		.A(search[463:456]),
	 	.B(current[431:424]),
	 	.AD(difference_matrix[3][5]));

me_PE PE_ij38(	.clk(clk), 
 		.A(search[471:464]),
	 	.B(current[439:432]),
	 	.AD(difference_matrix[3][6]));

me_PE PE_ij39(	.clk(clk), 
 		.A(search[479:472]),
	 	.B(current[447:440]),
	 	.AD(difference_matrix[3][7]));

me_PE PE_ij40(	.clk(clk), 
 		.A(search[487:480]),
	 	.B(current[455:448]),
	 	.AD(difference_matrix[3][8]));

me_PE PE_ij41(	.clk(clk), 
 		.A(search[495:488]),
	 	.B(current[463:456]),
	 	.AD(difference_matrix[3][9]));

me_PE PE_ij42(	.clk(clk), 
 		.A(search[503:496]),
	 	.B(current[471:464]),
	 	.AD(difference_matrix[3][10]));

me_PE PE_ij43(	.clk(clk), 
 		.A(search[511:504]),
	 	.B(current[479:472]),
	 	.AD(difference_matrix[3][11]));

me_PE PE_ij44(	.clk(clk), 
 		.A(search[519:512]),
	 	.B(current[487:480]),
	 	.AD(difference_matrix[3][12]));

me_PE PE_ij45(	.clk(clk), 
 		.A(search[527:520]),
	 	.B(current[495:488]),
	 	.AD(difference_matrix[3][13]));

me_PE PE_ij46(	.clk(clk), 
 		.A(search[535:528]),
	 	.B(current[503:496]),
	 	.AD(difference_matrix[3][14]));

me_PE PE_ij47(	.clk(clk), 
 		.A(search[543:536]),
	 	.B(current[511:504]),
	 	.AD(difference_matrix[3][15]));

me_PE PE_ij64(	.clk(clk), 
 		.A(search[559:552]),
	 	.B(current[519:512]),
	 	.AD(difference_matrix[4][0]));

me_PE PE_ij49(	.clk(clk), 
 		.A(search[567:560]),
	 	.B(current[527:520]),
	 	.AD(difference_matrix[4][1]));

me_PE PE_ij50(	.clk(clk), 
 		.A(search[575:568]),
	 	.B(current[535:528]),
	 	.AD(difference_matrix[4][2]));

me_PE PE_ij51(	.clk(clk), 
 		.A(search[583:576]),
	 	.B(current[543:536]),
	 	.AD(difference_matrix[4][3]));

me_PE PE_ij52(	.clk(clk), 
 		.A(search[591:584]),
	 	.B(current[551:544]),
	 	.AD(difference_matrix[4][4]));

me_PE PE_ij53(	.clk(clk), 
 		.A(search[599:592]),
	 	.B(current[559:552]),
	 	.AD(difference_matrix[4][5]));

me_PE PE_ij54(	.clk(clk), 
 		.A(search[607:600]),
	 	.B(current[567:560]),
	 	.AD(difference_matrix[4][6]));

me_PE PE_ij55(	.clk(clk), 
 		.A(search[615:608]),
	 	.B(current[575:568]),
	 	.AD(difference_matrix[4][7]));

me_PE PE_ij56(	.clk(clk), 
 		.A(search[623:616]),
	 	.B(current[583:576]),
	 	.AD(difference_matrix[4][8]));

me_PE PE_ij57(	.clk(clk), 
 		.A(search[631:624]),
	 	.B(current[591:584]),
	 	.AD(difference_matrix[4][9]));

me_PE PE_ij58(	.clk(clk), 
 		.A(search[639:632]),
	 	.B(current[599:592]),
	 	.AD(difference_matrix[4][10]));

me_PE PE_ij59(	.clk(clk), 
 		.A(search[647:640]),
	 	.B(current[607:600]),
	 	.AD(difference_matrix[4][11]));

me_PE PE_ij60(	.clk(clk), 
 		.A(search[655:648]),
	 	.B(current[615:608]),
	 	.AD(difference_matrix[4][12]));

me_PE PE_ij61(	.clk(clk), 
 		.A(search[663:656]),
	 	.B(current[623:616]),
	 	.AD(difference_matrix[4][13]));

me_PE PE_ij62(	.clk(clk), 
 		.A(search[671:664]),
	 	.B(current[631:624]),
	 	.AD(difference_matrix[4][14]));

me_PE PE_ij63(	.clk(clk), 
 		.A(search[679:672]),
	 	.B(current[639:632]),
	 	.AD(difference_matrix[4][15]));

me_PE PE_ij80(	.clk(clk), 
 		.A(search[695:688]),
	 	.B(current[647:640]),
	 	.AD(difference_matrix[5][0]));

me_PE PE_ij65(	.clk(clk), 
 		.A(search[703:696]),
	 	.B(current[655:648]),
	 	.AD(difference_matrix[5][1]));

me_PE PE_ij66(	.clk(clk), 
 		.A(search[711:704]),
	 	.B(current[663:656]),
	 	.AD(difference_matrix[5][2]));

me_PE PE_ij67(	.clk(clk), 
 		.A(search[719:712]),
	 	.B(current[671:664]),
	 	.AD(difference_matrix[5][3]));

me_PE PE_ij68(	.clk(clk), 
 		.A(search[727:720]),
	 	.B(current[679:672]),
	 	.AD(difference_matrix[5][4]));

me_PE PE_ij69(	.clk(clk), 
 		.A(search[735:728]),
	 	.B(current[687:680]),
	 	.AD(difference_matrix[5][5]));

me_PE PE_ij70(	.clk(clk), 
 		.A(search[743:736]),
	 	.B(current[695:688]),
	 	.AD(difference_matrix[5][6]));

me_PE PE_ij71(	.clk(clk), 
 		.A(search[751:744]),
	 	.B(current[703:696]),
	 	.AD(difference_matrix[5][7]));

me_PE PE_ij72(	.clk(clk), 
 		.A(search[759:752]),
	 	.B(current[711:704]),
	 	.AD(difference_matrix[5][8]));

me_PE PE_ij73(	.clk(clk), 
 		.A(search[767:760]),
	 	.B(current[719:712]),
	 	.AD(difference_matrix[5][9]));

me_PE PE_ij74(	.clk(clk), 
 		.A(search[775:768]),
	 	.B(current[727:720]),
	 	.AD(difference_matrix[5][10]));

me_PE PE_ij75(	.clk(clk), 
 		.A(search[783:776]),
	 	.B(current[735:728]),
	 	.AD(difference_matrix[5][11]));

me_PE PE_ij76(	.clk(clk), 
 		.A(search[791:784]),
	 	.B(current[743:736]),
	 	.AD(difference_matrix[5][12]));

me_PE PE_ij77(	.clk(clk), 
 		.A(search[799:792]),
	 	.B(current[751:744]),
	 	.AD(difference_matrix[5][13]));

me_PE PE_ij78(	.clk(clk), 
 		.A(search[807:800]),
	 	.B(current[759:752]),
	 	.AD(difference_matrix[5][14]));

me_PE PE_ij79(	.clk(clk), 
 		.A(search[815:808]),
	 	.B(current[767:760]),
	 	.AD(difference_matrix[5][15]));

me_PE PE_ij96(	.clk(clk), 
 		.A(search[831:824]),
	 	.B(current[775:768]),
	 	.AD(difference_matrix[6][0]));

me_PE PE_ij81(	.clk(clk), 
 		.A(search[839:832]),
	 	.B(current[783:776]),
	 	.AD(difference_matrix[6][1]));

me_PE PE_ij82(	.clk(clk), 
 		.A(search[847:840]),
	 	.B(current[791:784]),
	 	.AD(difference_matrix[6][2]));

me_PE PE_ij83(	.clk(clk), 
 		.A(search[855:848]),
	 	.B(current[799:792]),
	 	.AD(difference_matrix[6][3]));

me_PE PE_ij84(	.clk(clk), 
 		.A(search[863:856]),
	 	.B(current[807:800]),
	 	.AD(difference_matrix[6][4]));

me_PE PE_ij85(	.clk(clk), 
 		.A(search[871:864]),
	 	.B(current[815:808]),
	 	.AD(difference_matrix[6][5]));

me_PE PE_ij86(	.clk(clk), 
 		.A(search[879:872]),
	 	.B(current[823:816]),
	 	.AD(difference_matrix[6][6]));

me_PE PE_ij87(	.clk(clk), 
 		.A(search[887:880]),
	 	.B(current[831:824]),
	 	.AD(difference_matrix[6][7]));

me_PE PE_ij88(	.clk(clk), 
 		.A(search[895:888]),
	 	.B(current[839:832]),
	 	.AD(difference_matrix[6][8]));

me_PE PE_ij89(	.clk(clk), 
 		.A(search[903:896]),
	 	.B(current[847:840]),
	 	.AD(difference_matrix[6][9]));

me_PE PE_ij90(	.clk(clk), 
 		.A(search[911:904]),
	 	.B(current[855:848]),
	 	.AD(difference_matrix[6][10]));

me_PE PE_ij91(	.clk(clk), 
 		.A(search[919:912]),
	 	.B(current[863:856]),
	 	.AD(difference_matrix[6][11]));

me_PE PE_ij92(	.clk(clk), 
 		.A(search[927:920]),
	 	.B(current[871:864]),
	 	.AD(difference_matrix[6][12]));

me_PE PE_ij93(	.clk(clk), 
 		.A(search[935:928]),
	 	.B(current[879:872]),
	 	.AD(difference_matrix[6][13]));

me_PE PE_ij94(	.clk(clk), 
 		.A(search[943:936]),
	 	.B(current[887:880]),
	 	.AD(difference_matrix[6][14]));

me_PE PE_ij95(	.clk(clk), 
 		.A(search[951:944]),
	 	.B(current[895:888]),
	 	.AD(difference_matrix[6][15]));

me_PE PE_ij112(	.clk(clk), 
 		.A(search[967:960]),
	 	.B(current[903:896]),
	 	.AD(difference_matrix[7][0]));

me_PE PE_ij97(	.clk(clk), 
 		.A(search[975:968]),
	 	.B(current[911:904]),
	 	.AD(difference_matrix[7][1]));

me_PE PE_ij98(	.clk(clk), 
 		.A(search[983:976]),
	 	.B(current[919:912]),
	 	.AD(difference_matrix[7][2]));

me_PE PE_ij99(	.clk(clk), 
 		.A(search[991:984]),
	 	.B(current[927:920]),
	 	.AD(difference_matrix[7][3]));

me_PE PE_ij100(	.clk(clk), 
 		.A(search[999:992]),
	 	.B(current[935:928]),
	 	.AD(difference_matrix[7][4]));

me_PE PE_ij101(	.clk(clk), 
 		.A(search[1007:1000]),
	 	.B(current[943:936]),
	 	.AD(difference_matrix[7][5]));

me_PE PE_ij102(	.clk(clk), 
 		.A(search[1015:1008]),
	 	.B(current[951:944]),
	 	.AD(difference_matrix[7][6]));

me_PE PE_ij103(	.clk(clk), 
 		.A(search[1023:1016]),
	 	.B(current[959:952]),
	 	.AD(difference_matrix[7][7]));

me_PE PE_ij104(	.clk(clk), 
 		.A(search[1031:1024]),
	 	.B(current[967:960]),
	 	.AD(difference_matrix[7][8]));

me_PE PE_ij105(	.clk(clk), 
 		.A(search[1039:1032]),
	 	.B(current[975:968]),
	 	.AD(difference_matrix[7][9]));

me_PE PE_ij106(	.clk(clk), 
 		.A(search[1047:1040]),
	 	.B(current[983:976]),
	 	.AD(difference_matrix[7][10]));

me_PE PE_ij107(	.clk(clk), 
 		.A(search[1055:1048]),
	 	.B(current[991:984]),
	 	.AD(difference_matrix[7][11]));

me_PE PE_ij108(	.clk(clk), 
 		.A(search[1063:1056]),
	 	.B(current[999:992]),
	 	.AD(difference_matrix[7][12]));

me_PE PE_ij109(	.clk(clk), 
 		.A(search[1071:1064]),
	 	.B(current[1007:1000]),
	 	.AD(difference_matrix[7][13]));

me_PE PE_ij110(	.clk(clk), 
 		.A(search[1079:1072]),
	 	.B(current[1015:1008]),
	 	.AD(difference_matrix[7][14]));

me_PE PE_ij111(	.clk(clk), 
 		.A(search[1087:1080]),
	 	.B(current[1023:1016]),
	 	.AD(difference_matrix[7][15]));

me_PE PE_ij128(	.clk(clk), 
 		.A(search[1103:1096]),
	 	.B(current[1031:1024]),
	 	.AD(difference_matrix[8][0]));

me_PE PE_ij113(	.clk(clk), 
 		.A(search[1111:1104]),
	 	.B(current[1039:1032]),
	 	.AD(difference_matrix[8][1]));

me_PE PE_ij114(	.clk(clk), 
 		.A(search[1119:1112]),
	 	.B(current[1047:1040]),
	 	.AD(difference_matrix[8][2]));

me_PE PE_ij115(	.clk(clk), 
 		.A(search[1127:1120]),
	 	.B(current[1055:1048]),
	 	.AD(difference_matrix[8][3]));

me_PE PE_ij116(	.clk(clk), 
 		.A(search[1135:1128]),
	 	.B(current[1063:1056]),
	 	.AD(difference_matrix[8][4]));

me_PE PE_ij117(	.clk(clk), 
 		.A(search[1143:1136]),
	 	.B(current[1071:1064]),
	 	.AD(difference_matrix[8][5]));

me_PE PE_ij118(	.clk(clk), 
 		.A(search[1151:1144]),
	 	.B(current[1079:1072]),
	 	.AD(difference_matrix[8][6]));

me_PE PE_ij119(	.clk(clk), 
 		.A(search[1159:1152]),
	 	.B(current[1087:1080]),
	 	.AD(difference_matrix[8][7]));

me_PE PE_ij120(	.clk(clk), 
 		.A(search[1167:1160]),
	 	.B(current[1095:1088]),
	 	.AD(difference_matrix[8][8]));

me_PE PE_ij121(	.clk(clk), 
 		.A(search[1175:1168]),
	 	.B(current[1103:1096]),
	 	.AD(difference_matrix[8][9]));

me_PE PE_ij122(	.clk(clk), 
 		.A(search[1183:1176]),
	 	.B(current[1111:1104]),
	 	.AD(difference_matrix[8][10]));

me_PE PE_ij123(	.clk(clk), 
 		.A(search[1191:1184]),
	 	.B(current[1119:1112]),
	 	.AD(difference_matrix[8][11]));

me_PE PE_ij124(	.clk(clk), 
 		.A(search[1199:1192]),
	 	.B(current[1127:1120]),
	 	.AD(difference_matrix[8][12]));

me_PE PE_ij125(	.clk(clk), 
 		.A(search[1207:1200]),
	 	.B(current[1135:1128]),
	 	.AD(difference_matrix[8][13]));

me_PE PE_ij126(	.clk(clk), 
 		.A(search[1215:1208]),
	 	.B(current[1143:1136]),
	 	.AD(difference_matrix[8][14]));

me_PE PE_ij127(	.clk(clk), 
 		.A(search[1223:1216]),
	 	.B(current[1151:1144]),
	 	.AD(difference_matrix[8][15]));

me_PE PE_ij144(	.clk(clk), 
 		.A(search[1239:1232]),
	 	.B(current[1159:1152]),
	 	.AD(difference_matrix[9][0]));

me_PE PE_ij129(	.clk(clk), 
 		.A(search[1247:1240]),
	 	.B(current[1167:1160]),
	 	.AD(difference_matrix[9][1]));

me_PE PE_ij130(	.clk(clk), 
 		.A(search[1255:1248]),
	 	.B(current[1175:1168]),
	 	.AD(difference_matrix[9][2]));

me_PE PE_ij131(	.clk(clk), 
 		.A(search[1263:1256]),
	 	.B(current[1183:1176]),
	 	.AD(difference_matrix[9][3]));

me_PE PE_ij132(	.clk(clk), 
 		.A(search[1271:1264]),
	 	.B(current[1191:1184]),
	 	.AD(difference_matrix[9][4]));

me_PE PE_ij133(	.clk(clk), 
 		.A(search[1279:1272]),
	 	.B(current[1199:1192]),
	 	.AD(difference_matrix[9][5]));

me_PE PE_ij134(	.clk(clk), 
 		.A(search[1287:1280]),
	 	.B(current[1207:1200]),
	 	.AD(difference_matrix[9][6]));

me_PE PE_ij135(	.clk(clk), 
 		.A(search[1295:1288]),
	 	.B(current[1215:1208]),
	 	.AD(difference_matrix[9][7]));

me_PE PE_ij136(	.clk(clk), 
 		.A(search[1303:1296]),
	 	.B(current[1223:1216]),
	 	.AD(difference_matrix[9][8]));

me_PE PE_ij137(	.clk(clk), 
 		.A(search[1311:1304]),
	 	.B(current[1231:1224]),
	 	.AD(difference_matrix[9][9]));

me_PE PE_ij138(	.clk(clk), 
 		.A(search[1319:1312]),
	 	.B(current[1239:1232]),
	 	.AD(difference_matrix[9][10]));

me_PE PE_ij139(	.clk(clk), 
 		.A(search[1327:1320]),
	 	.B(current[1247:1240]),
	 	.AD(difference_matrix[9][11]));

me_PE PE_ij140(	.clk(clk), 
 		.A(search[1335:1328]),
	 	.B(current[1255:1248]),
	 	.AD(difference_matrix[9][12]));

me_PE PE_ij141(	.clk(clk), 
 		.A(search[1343:1336]),
	 	.B(current[1263:1256]),
	 	.AD(difference_matrix[9][13]));

me_PE PE_ij142(	.clk(clk), 
 		.A(search[1351:1344]),
	 	.B(current[1271:1264]),
	 	.AD(difference_matrix[9][14]));

me_PE PE_ij143(	.clk(clk), 
 		.A(search[1359:1352]),
	 	.B(current[1279:1272]),
	 	.AD(difference_matrix[9][15]));

me_PE PE_ij160(	.clk(clk), 
 		.A(search[1375:1368]),
	 	.B(current[1287:1280]),
	 	.AD(difference_matrix[10][0]));

me_PE PE_ij145(	.clk(clk), 
 		.A(search[1383:1376]),
	 	.B(current[1295:1288]),
	 	.AD(difference_matrix[10][1]));

me_PE PE_ij146(	.clk(clk), 
 		.A(search[1391:1384]),
	 	.B(current[1303:1296]),
	 	.AD(difference_matrix[10][2]));

me_PE PE_ij147(	.clk(clk), 
 		.A(search[1399:1392]),
	 	.B(current[1311:1304]),
	 	.AD(difference_matrix[10][3]));

me_PE PE_ij148(	.clk(clk), 
 		.A(search[1407:1400]),
	 	.B(current[1319:1312]),
	 	.AD(difference_matrix[10][4]));

me_PE PE_ij149(	.clk(clk), 
 		.A(search[1415:1408]),
	 	.B(current[1327:1320]),
	 	.AD(difference_matrix[10][5]));

me_PE PE_ij150(	.clk(clk), 
 		.A(search[1423:1416]),
	 	.B(current[1335:1328]),
	 	.AD(difference_matrix[10][6]));

me_PE PE_ij151(	.clk(clk), 
 		.A(search[1431:1424]),
	 	.B(current[1343:1336]),
	 	.AD(difference_matrix[10][7]));

me_PE PE_ij152(	.clk(clk), 
 		.A(search[1439:1432]),
	 	.B(current[1351:1344]),
	 	.AD(difference_matrix[10][8]));

me_PE PE_ij153(	.clk(clk), 
 		.A(search[1447:1440]),
	 	.B(current[1359:1352]),
	 	.AD(difference_matrix[10][9]));

me_PE PE_ij154(	.clk(clk), 
 		.A(search[1455:1448]),
	 	.B(current[1367:1360]),
	 	.AD(difference_matrix[10][10]));

me_PE PE_ij155(	.clk(clk), 
 		.A(search[1463:1456]),
	 	.B(current[1375:1368]),
	 	.AD(difference_matrix[10][11]));

me_PE PE_ij156(	.clk(clk), 
 		.A(search[1471:1464]),
	 	.B(current[1383:1376]),
	 	.AD(difference_matrix[10][12]));

me_PE PE_ij157(	.clk(clk), 
 		.A(search[1479:1472]),
	 	.B(current[1391:1384]),
	 	.AD(difference_matrix[10][13]));

me_PE PE_ij158(	.clk(clk), 
 		.A(search[1487:1480]),
	 	.B(current[1399:1392]),
	 	.AD(difference_matrix[10][14]));

me_PE PE_ij159(	.clk(clk), 
 		.A(search[1495:1488]),
	 	.B(current[1407:1400]),
	 	.AD(difference_matrix[10][15]));

me_PE PE_ij176(	.clk(clk), 
 		.A(search[1511:1504]),
	 	.B(current[1415:1408]),
	 	.AD(difference_matrix[11][0]));

me_PE PE_ij161(	.clk(clk), 
 		.A(search[1519:1512]),
	 	.B(current[1423:1416]),
	 	.AD(difference_matrix[11][1]));

me_PE PE_ij162(	.clk(clk), 
 		.A(search[1527:1520]),
	 	.B(current[1431:1424]),
	 	.AD(difference_matrix[11][2]));

me_PE PE_ij163(	.clk(clk), 
 		.A(search[1535:1528]),
	 	.B(current[1439:1432]),
	 	.AD(difference_matrix[11][3]));

me_PE PE_ij164(	.clk(clk), 
 		.A(search[1543:1536]),
	 	.B(current[1447:1440]),
	 	.AD(difference_matrix[11][4]));

me_PE PE_ij165(	.clk(clk), 
 		.A(search[1551:1544]),
	 	.B(current[1455:1448]),
	 	.AD(difference_matrix[11][5]));

me_PE PE_ij166(	.clk(clk), 
 		.A(search[1559:1552]),
	 	.B(current[1463:1456]),
	 	.AD(difference_matrix[11][6]));

me_PE PE_ij167(	.clk(clk), 
 		.A(search[1567:1560]),
	 	.B(current[1471:1464]),
	 	.AD(difference_matrix[11][7]));

me_PE PE_ij168(	.clk(clk), 
 		.A(search[1575:1568]),
	 	.B(current[1479:1472]),
	 	.AD(difference_matrix[11][8]));

me_PE PE_ij169(	.clk(clk), 
 		.A(search[1583:1576]),
	 	.B(current[1487:1480]),
	 	.AD(difference_matrix[11][9]));

me_PE PE_ij170(	.clk(clk), 
 		.A(search[1591:1584]),
	 	.B(current[1495:1488]),
	 	.AD(difference_matrix[11][10]));

me_PE PE_ij171(	.clk(clk), 
 		.A(search[1599:1592]),
	 	.B(current[1503:1496]),
	 	.AD(difference_matrix[11][11]));

me_PE PE_ij172(	.clk(clk), 
 		.A(search[1607:1600]),
	 	.B(current[1511:1504]),
	 	.AD(difference_matrix[11][12]));

me_PE PE_ij173(	.clk(clk), 
 		.A(search[1615:1608]),
	 	.B(current[1519:1512]),
	 	.AD(difference_matrix[11][13]));

me_PE PE_ij174(	.clk(clk), 
 		.A(search[1623:1616]),
	 	.B(current[1527:1520]),
	 	.AD(difference_matrix[11][14]));

me_PE PE_ij175(	.clk(clk), 
 		.A(search[1631:1624]),
	 	.B(current[1535:1528]),
	 	.AD(difference_matrix[11][15]));

me_PE PE_ij192(	.clk(clk), 
 		.A(search[1647:1640]),
	 	.B(current[1543:1536]),
	 	.AD(difference_matrix[12][0]));

me_PE PE_ij177(	.clk(clk), 
 		.A(search[1655:1648]),
	 	.B(current[1551:1544]),
	 	.AD(difference_matrix[12][1]));

me_PE PE_ij178(	.clk(clk), 
 		.A(search[1663:1656]),
	 	.B(current[1559:1552]),
	 	.AD(difference_matrix[12][2]));

me_PE PE_ij179(	.clk(clk), 
 		.A(search[1671:1664]),
	 	.B(current[1567:1560]),
	 	.AD(difference_matrix[12][3]));

me_PE PE_ij180(	.clk(clk), 
 		.A(search[1679:1672]),
	 	.B(current[1575:1568]),
	 	.AD(difference_matrix[12][4]));

me_PE PE_ij181(	.clk(clk), 
 		.A(search[1687:1680]),
	 	.B(current[1583:1576]),
	 	.AD(difference_matrix[12][5]));

me_PE PE_ij182(	.clk(clk), 
 		.A(search[1695:1688]),
	 	.B(current[1591:1584]),
	 	.AD(difference_matrix[12][6]));

me_PE PE_ij183(	.clk(clk), 
 		.A(search[1703:1696]),
	 	.B(current[1599:1592]),
	 	.AD(difference_matrix[12][7]));

me_PE PE_ij184(	.clk(clk), 
 		.A(search[1711:1704]),
	 	.B(current[1607:1600]),
	 	.AD(difference_matrix[12][8]));

me_PE PE_ij185(	.clk(clk), 
 		.A(search[1719:1712]),
	 	.B(current[1615:1608]),
	 	.AD(difference_matrix[12][9]));

me_PE PE_ij186(	.clk(clk), 
 		.A(search[1727:1720]),
	 	.B(current[1623:1616]),
	 	.AD(difference_matrix[12][10]));

me_PE PE_ij187(	.clk(clk), 
 		.A(search[1735:1728]),
	 	.B(current[1631:1624]),
	 	.AD(difference_matrix[12][11]));

me_PE PE_ij188(	.clk(clk), 
 		.A(search[1743:1736]),
	 	.B(current[1639:1632]),
	 	.AD(difference_matrix[12][12]));

me_PE PE_ij189(	.clk(clk), 
 		.A(search[1751:1744]),
	 	.B(current[1647:1640]),
	 	.AD(difference_matrix[12][13]));

me_PE PE_ij190(	.clk(clk), 
 		.A(search[1759:1752]),
	 	.B(current[1655:1648]),
	 	.AD(difference_matrix[12][14]));

me_PE PE_ij191(	.clk(clk), 
 		.A(search[1767:1760]),
	 	.B(current[1663:1656]),
	 	.AD(difference_matrix[12][15]));

me_PE PE_ij208(	.clk(clk), 
 		.A(search[1783:1776]),
	 	.B(current[1671:1664]),
	 	.AD(difference_matrix[13][0]));

me_PE PE_ij193(	.clk(clk), 
 		.A(search[1791:1784]),
	 	.B(current[1679:1672]),
	 	.AD(difference_matrix[13][1]));

me_PE PE_ij194(	.clk(clk), 
 		.A(search[1799:1792]),
	 	.B(current[1687:1680]),
	 	.AD(difference_matrix[13][2]));

me_PE PE_ij195(	.clk(clk), 
 		.A(search[1807:1800]),
	 	.B(current[1695:1688]),
	 	.AD(difference_matrix[13][3]));

me_PE PE_ij196(	.clk(clk), 
 		.A(search[1815:1808]),
	 	.B(current[1703:1696]),
	 	.AD(difference_matrix[13][4]));

me_PE PE_ij197(	.clk(clk), 
 		.A(search[1823:1816]),
	 	.B(current[1711:1704]),
	 	.AD(difference_matrix[13][5]));

me_PE PE_ij198(	.clk(clk), 
 		.A(search[1831:1824]),
	 	.B(current[1719:1712]),
	 	.AD(difference_matrix[13][6]));

me_PE PE_ij199(	.clk(clk), 
 		.A(search[1839:1832]),
	 	.B(current[1727:1720]),
	 	.AD(difference_matrix[13][7]));

me_PE PE_ij200(	.clk(clk), 
 		.A(search[1847:1840]),
	 	.B(current[1735:1728]),
	 	.AD(difference_matrix[13][8]));

me_PE PE_ij201(	.clk(clk), 
 		.A(search[1855:1848]),
	 	.B(current[1743:1736]),
	 	.AD(difference_matrix[13][9]));

me_PE PE_ij202(	.clk(clk), 
 		.A(search[1863:1856]),
	 	.B(current[1751:1744]),
	 	.AD(difference_matrix[13][10]));

me_PE PE_ij203(	.clk(clk), 
 		.A(search[1871:1864]),
	 	.B(current[1759:1752]),
	 	.AD(difference_matrix[13][11]));

me_PE PE_ij204(	.clk(clk), 
 		.A(search[1879:1872]),
	 	.B(current[1767:1760]),
	 	.AD(difference_matrix[13][12]));

me_PE PE_ij205(	.clk(clk), 
 		.A(search[1887:1880]),
	 	.B(current[1775:1768]),
	 	.AD(difference_matrix[13][13]));

me_PE PE_ij206(	.clk(clk), 
 		.A(search[1895:1888]),
	 	.B(current[1783:1776]),
	 	.AD(difference_matrix[13][14]));

me_PE PE_ij207(	.clk(clk), 
 		.A(search[1903:1896]),
	 	.B(current[1791:1784]),
	 	.AD(difference_matrix[13][15]));

me_PE PE_ij224(	.clk(clk), 
 		.A(search[1919:1912]),
	 	.B(current[1799:1792]),
	 	.AD(difference_matrix[14][0]));

me_PE PE_ij209(	.clk(clk), 
 		.A(search[1927:1920]),
	 	.B(current[1807:1800]),
	 	.AD(difference_matrix[14][1]));

me_PE PE_ij210(	.clk(clk), 
 		.A(search[1935:1928]),
	 	.B(current[1815:1808]),
	 	.AD(difference_matrix[14][2]));

me_PE PE_ij211(	.clk(clk), 
 		.A(search[1943:1936]),
	 	.B(current[1823:1816]),
	 	.AD(difference_matrix[14][3]));

me_PE PE_ij212(	.clk(clk), 
 		.A(search[1951:1944]),
	 	.B(current[1831:1824]),
	 	.AD(difference_matrix[14][4]));

me_PE PE_ij213(	.clk(clk), 
 		.A(search[1959:1952]),
	 	.B(current[1839:1832]),
	 	.AD(difference_matrix[14][5]));

me_PE PE_ij214(	.clk(clk), 
 		.A(search[1967:1960]),
	 	.B(current[1847:1840]),
	 	.AD(difference_matrix[14][6]));

me_PE PE_ij215(	.clk(clk), 
 		.A(search[1975:1968]),
	 	.B(current[1855:1848]),
	 	.AD(difference_matrix[14][7]));

me_PE PE_ij216(	.clk(clk), 
 		.A(search[1983:1976]),
	 	.B(current[1863:1856]),
	 	.AD(difference_matrix[14][8]));

me_PE PE_ij217(	.clk(clk), 
 		.A(search[1991:1984]),
	 	.B(current[1871:1864]),
	 	.AD(difference_matrix[14][9]));

me_PE PE_ij218(	.clk(clk), 
 		.A(search[1999:1992]),
	 	.B(current[1879:1872]),
	 	.AD(difference_matrix[14][10]));

me_PE PE_ij219(	.clk(clk), 
 		.A(search[2007:2000]),
	 	.B(current[1887:1880]),
	 	.AD(difference_matrix[14][11]));

me_PE PE_ij220(	.clk(clk), 
 		.A(search[2015:2008]),
	 	.B(current[1895:1888]),
	 	.AD(difference_matrix[14][12]));

me_PE PE_ij221(	.clk(clk), 
 		.A(search[2023:2016]),
	 	.B(current[1903:1896]),
	 	.AD(difference_matrix[14][13]));

me_PE PE_ij222(	.clk(clk), 
 		.A(search[2031:2024]),
	 	.B(current[1911:1904]),
	 	.AD(difference_matrix[14][14]));

me_PE PE_ij223(	.clk(clk), 
 		.A(search[2039:2032]),
	 	.B(current[1919:1912]),
	 	.AD(difference_matrix[14][15]));

me_PE PE_ij240(	.clk(clk), 
 		.A(search[2055:2048]),
	 	.B(current[1927:1920]),
	 	.AD(difference_matrix[15][0]));

me_PE PE_ij225(	.clk(clk), 
 		.A(search[2063:2056]),
	 	.B(current[1935:1928]),
	 	.AD(difference_matrix[15][1]));

me_PE PE_ij226(	.clk(clk), 
 		.A(search[2071:2064]),
	 	.B(current[1943:1936]),
	 	.AD(difference_matrix[15][2]));

me_PE PE_ij227(	.clk(clk), 
 		.A(search[2079:2072]),
	 	.B(current[1951:1944]),
	 	.AD(difference_matrix[15][3]));

me_PE PE_ij228(	.clk(clk), 
 		.A(search[2087:2080]),
	 	.B(current[1959:1952]),
	 	.AD(difference_matrix[15][4]));

me_PE PE_ij229(	.clk(clk), 
 		.A(search[2095:2088]),
	 	.B(current[1967:1960]),
	 	.AD(difference_matrix[15][5]));

me_PE PE_ij230(	.clk(clk), 
 		.A(search[2103:2096]),
	 	.B(current[1975:1968]),
	 	.AD(difference_matrix[15][6]));

me_PE PE_ij231(	.clk(clk), 
 		.A(search[2111:2104]),
	 	.B(current[1983:1976]),
	 	.AD(difference_matrix[15][7]));

me_PE PE_ij232(	.clk(clk), 
 		.A(search[2119:2112]),
	 	.B(current[1991:1984]),
	 	.AD(difference_matrix[15][8]));

me_PE PE_ij233(	.clk(clk), 
 		.A(search[2127:2120]),
	 	.B(current[1999:1992]),
	 	.AD(difference_matrix[15][9]));

me_PE PE_ij234(	.clk(clk), 
 		.A(search[2135:2128]),
	 	.B(current[2007:2000]),
	 	.AD(difference_matrix[15][10]));

me_PE PE_ij235(	.clk(clk), 
 		.A(search[2143:2136]),
	 	.B(current[2015:2008]),
	 	.AD(difference_matrix[15][11]));

me_PE PE_ij236(	.clk(clk), 
 		.A(search[2151:2144]),
	 	.B(current[2023:2016]),
	 	.AD(difference_matrix[15][12]));

me_PE PE_ij237(	.clk(clk), 
 		.A(search[2159:2152]),
	 	.B(current[2031:2024]),
	 	.AD(difference_matrix[15][13]));

me_PE PE_ij238(	.clk(clk), 
 		.A(search[2167:2160]),
	 	.B(current[2039:2032]),
	 	.AD(difference_matrix[15][14]));

me_PE PE_ij239(	.clk(clk), 
 		.A(search[2175:2168]),
	 	.B(current[2047:2040]),
	 	.AD(difference_matrix[15][15]));

always @ (posedge clk)
begin
	add01 <=  ({difference_matrix[0][0][7:2], {1'b0,1'b0}}  + difference_matrix[0][1][7:0]) + ({difference_matrix[0][2][7:2], {1'b0,1'b0}}  + {difference_matrix[0][3][7:3], {1'b0,1'b0,1'b0}} );

	add02 <=  ({difference_matrix[0][4][7:1], {1'b0}}  | difference_matrix[0][5][7:0]) | ({difference_matrix[0][6][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[0][7][7:1], {1'b0}} );

	add03 <=  (difference_matrix[0][8][7:0] + difference_matrix[0][9][7:0]) + ({difference_matrix[0][10][7:2], {1'b0,1'b0}}  | difference_matrix[0][11][7:0]);

	add04 <=  (difference_matrix[0][12][7:0] | {difference_matrix[0][13][7:2], {1'b0,1'b0}} ) | (difference_matrix[0][14][7:0] + difference_matrix[0][15][7:0]);

	add05 <=  ({difference_matrix[1][0][7:1], {1'b0}}  + {difference_matrix[1][1][7:3], {1'b0,1'b0,1'b0}} ) + ({difference_matrix[1][2][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[1][3][7:1], {1'b0}} );

	add06 <=  ({difference_matrix[1][4][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[1][5][7:1], {1'b0}} ) + ({difference_matrix[1][6][7:3], {1'b0,1'b0,1'b0}}  + difference_matrix[1][7][7:0]);

	add07 <=  ({difference_matrix[1][8][7:1], {1'b0}}  + difference_matrix[1][9][7:0]) + ({difference_matrix[1][10][7:1], {1'b0}}  | {difference_matrix[1][11][7:1], {1'b0}} );

	add08 <=  ({difference_matrix[1][12][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[1][13][7:1], {1'b0}} ) + (difference_matrix[1][14][7:0] + {difference_matrix[1][15][7:1], {1'b0}} );

	add09 <=  ({difference_matrix[2][0][7:1], {1'b0}}  | {difference_matrix[2][1][7:1], {1'b0}} ) | ({difference_matrix[2][2][7:2], {1'b0,1'b0}}  | difference_matrix[2][3][7:0]);

	add10 <=  ({difference_matrix[2][4][7:3], {1'b0,1'b0,1'b0}}  + difference_matrix[2][5][7:0]) + (difference_matrix[2][6][7:0] + {difference_matrix[2][7][7:2], {1'b0,1'b0}} );

	add11 <=  ({difference_matrix[2][8][7:1], {1'b0}}  + difference_matrix[2][9][7:0]) + ({difference_matrix[2][10][7:1], {1'b0}}  + {difference_matrix[2][11][7:3], {1'b0,1'b0,1'b0}} );

	add12 <=  ({difference_matrix[2][12][7:3], {1'b0,1'b0,1'b0}}  | difference_matrix[2][13][7:0]) | (difference_matrix[2][14][7:0] + difference_matrix[2][15][7:0]);

	add13 <=  (difference_matrix[3][0][7:0] | {difference_matrix[3][1][7:1], {1'b0}} ) | (difference_matrix[3][2][7:0] + {difference_matrix[3][3][7:1], {1'b0}} );

	add14 <=  (difference_matrix[3][4][7:0] + difference_matrix[3][5][7:0]) + ({difference_matrix[3][6][7:1], {1'b0}}  + {difference_matrix[3][7][7:2], {1'b0,1'b0}} );

	add15 <=  (difference_matrix[3][8][7:0] + {difference_matrix[3][9][7:1], {1'b0}} ) + ({difference_matrix[3][10][7:3], {1'b0,1'b0,1'b0}}  + difference_matrix[3][11][7:0]);

	add16 <=  ({difference_matrix[3][12][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[3][13][7:1], {1'b0}} ) + ({difference_matrix[3][14][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[3][15][7:3], {1'b0,1'b0,1'b0}} );

	add17 <=  ({difference_matrix[4][0][7:1], {1'b0}}  | {difference_matrix[4][1][7:1], {1'b0}} ) | (difference_matrix[4][2][7:0] + difference_matrix[4][3][7:0]);

	add18 <=  ({difference_matrix[4][4][7:3], {1'b0,1'b0,1'b0}}  + difference_matrix[4][5][7:0]) + ({difference_matrix[4][6][7:2], {1'b0,1'b0}}  + difference_matrix[4][7][7:0]);

	add19 <=  (difference_matrix[4][8][7:0] | {difference_matrix[4][9][7:3], {1'b0,1'b0,1'b0}} ) | ({difference_matrix[4][10][7:2], {1'b0,1'b0}}  + {difference_matrix[4][11][7:2], {1'b0,1'b0}} );

	add20 <=  ({difference_matrix[4][12][7:2], {1'b0,1'b0}}  | difference_matrix[4][13][7:0]) | ({difference_matrix[4][14][7:2], {1'b0,1'b0}}  + difference_matrix[4][15][7:0]);

	add21 <=  ({difference_matrix[5][0][7:2], {1'b0,1'b0}}  | difference_matrix[5][1][7:0]) | ({difference_matrix[5][2][7:1], {1'b0}}  | difference_matrix[5][3][7:0]);

	add22 <=  ({difference_matrix[5][4][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[5][5][7:2], {1'b0,1'b0}} ) + (difference_matrix[5][6][7:0] + {difference_matrix[5][7][7:3], {1'b0,1'b0,1'b0}} );

	add23 <=  (difference_matrix[5][8][7:0] + {difference_matrix[5][9][7:2], {1'b0,1'b0}} ) + (difference_matrix[5][10][7:0] + difference_matrix[5][11][7:0]);

	add24 <=  ({difference_matrix[5][12][7:3], {1'b0,1'b0,1'b0}}  | {difference_matrix[5][13][7:1], {1'b0}} ) | ({difference_matrix[5][14][7:3], {1'b0,1'b0,1'b0}}  + difference_matrix[5][15][7:0]);

	add25 <=  ({difference_matrix[6][0][7:3], {1'b0,1'b0,1'b0}}  | {difference_matrix[6][1][7:3], {1'b0,1'b0,1'b0}} ) | ({difference_matrix[6][2][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[6][3][7:2], {1'b0,1'b0}} );

	add26 <=  (difference_matrix[6][4][7:0] + {difference_matrix[6][5][7:1], {1'b0}} ) + ({difference_matrix[6][6][7:3], {1'b0,1'b0,1'b0}}  + difference_matrix[6][7][7:0]);

	add27 <=  ({difference_matrix[6][8][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[6][9][7:3], {1'b0,1'b0,1'b0}} ) + ({difference_matrix[6][10][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[6][11][7:1], {1'b0}} );

	add28 <=  ({difference_matrix[6][12][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[6][13][7:3], {1'b0,1'b0,1'b0}} ) + (difference_matrix[6][14][7:0] | difference_matrix[6][15][7:0]);

	add29 <=  ({difference_matrix[7][0][7:3], {1'b0,1'b0,1'b0}}  | {difference_matrix[7][1][7:1], {1'b0}} ) | ({difference_matrix[7][2][7:2], {1'b0,1'b0}}  + {difference_matrix[7][3][7:1], {1'b0}} );

	add30 <=  ({difference_matrix[7][4][7:3], {1'b0,1'b0,1'b0}}  | difference_matrix[7][5][7:0]) | ({difference_matrix[7][6][7:1], {1'b0}}  | {difference_matrix[7][7][7:2], {1'b0,1'b0}} );

	add31 <=  (difference_matrix[7][8][7:0] | {difference_matrix[7][9][7:1], {1'b0}} ) | ({difference_matrix[7][10][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[7][11][7:2], {1'b0,1'b0}} );

	add32 <=  ({difference_matrix[7][12][7:2], {1'b0,1'b0}}  + {difference_matrix[7][13][7:1], {1'b0}} ) + ({difference_matrix[7][14][7:3], {1'b0,1'b0,1'b0}}  | {difference_matrix[7][15][7:1], {1'b0}} );

	add33 <=  ({difference_matrix[8][0][7:2], {1'b0,1'b0}}  + difference_matrix[8][1][7:0]) + ({difference_matrix[8][2][7:2], {1'b0,1'b0}}  + {difference_matrix[8][3][7:1], {1'b0}} );

	add34 <=  ({difference_matrix[8][4][7:2], {1'b0,1'b0}}  + {difference_matrix[8][5][7:2], {1'b0,1'b0}} ) + (difference_matrix[8][6][7:0] + {difference_matrix[8][7][7:3], {1'b0,1'b0,1'b0}} );

	add35 <=  (difference_matrix[8][8][7:0] | {difference_matrix[8][9][7:2], {1'b0,1'b0}} ) | (difference_matrix[8][10][7:0] | difference_matrix[8][11][7:0]);

	add36 <=  (difference_matrix[8][12][7:0] + {difference_matrix[8][13][7:2], {1'b0,1'b0}} ) + ({difference_matrix[8][14][7:3], {1'b0,1'b0,1'b0}}  | difference_matrix[8][15][7:0]);

	add37 <=  ({difference_matrix[9][0][7:2], {1'b0,1'b0}}  | {difference_matrix[9][1][7:3], {1'b0,1'b0,1'b0}} ) | (difference_matrix[9][2][7:0] | {difference_matrix[9][3][7:3], {1'b0,1'b0,1'b0}} );

	add38 <=  ({difference_matrix[9][4][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[9][5][7:3], {1'b0,1'b0,1'b0}} ) + ({difference_matrix[9][6][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[9][7][7:1], {1'b0}} );

	add39 <=  (difference_matrix[9][8][7:0] | difference_matrix[9][9][7:0]) | ({difference_matrix[9][10][7:2], {1'b0,1'b0}}  + {difference_matrix[9][11][7:3], {1'b0,1'b0,1'b0}} );

	add40 <=  ({difference_matrix[9][12][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[9][13][7:3], {1'b0,1'b0,1'b0}} ) + (difference_matrix[9][14][7:0] | difference_matrix[9][15][7:0]);

	add41 <=  ({difference_matrix[10][0][7:1], {1'b0}}  + {difference_matrix[10][1][7:3], {1'b0,1'b0,1'b0}} ) + ({difference_matrix[10][2][7:1], {1'b0}}  | difference_matrix[10][3][7:0]);

	add42 <=  ({difference_matrix[10][4][7:2], {1'b0,1'b0}}  + {difference_matrix[10][5][7:2], {1'b0,1'b0}} ) + ({difference_matrix[10][6][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[10][7][7:3], {1'b0,1'b0,1'b0}} );

	add43 <=  (difference_matrix[10][8][7:0] | {difference_matrix[10][9][7:2], {1'b0,1'b0}} ) | ({difference_matrix[10][10][7:2], {1'b0,1'b0}}  + {difference_matrix[10][11][7:2], {1'b0,1'b0}} );

	add44 <=  ({difference_matrix[10][12][7:1], {1'b0}}  + {difference_matrix[10][13][7:1], {1'b0}} ) + (difference_matrix[10][14][7:0] + difference_matrix[10][15][7:0]);

	add45 <=  (difference_matrix[11][0][7:0] + {difference_matrix[11][1][7:1], {1'b0}} ) + ({difference_matrix[11][2][7:2], {1'b0,1'b0}}  + {difference_matrix[11][3][7:3], {1'b0,1'b0,1'b0}} );

	add46 <=  (difference_matrix[11][4][7:0] | {difference_matrix[11][5][7:1], {1'b0}} ) | ({difference_matrix[11][6][7:2], {1'b0,1'b0}}  + {difference_matrix[11][7][7:1], {1'b0}} );

	add47 <=  (difference_matrix[11][8][7:0] | difference_matrix[11][9][7:0]) | ({difference_matrix[11][10][7:1], {1'b0}}  + {difference_matrix[11][11][7:3], {1'b0,1'b0,1'b0}} );

	add48 <=  ({difference_matrix[11][12][7:3], {1'b0,1'b0,1'b0}}  + difference_matrix[11][13][7:0]) + ({difference_matrix[11][14][7:1], {1'b0}}  + {difference_matrix[11][15][7:2], {1'b0,1'b0}} );

	add49 <=  (difference_matrix[12][0][7:0] + {difference_matrix[12][1][7:2], {1'b0,1'b0}} ) + (difference_matrix[12][2][7:0] + {difference_matrix[12][3][7:1], {1'b0}} );

	add50 <=  ({difference_matrix[12][4][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[12][5][7:2], {1'b0,1'b0}} ) + (difference_matrix[12][6][7:0] | {difference_matrix[12][7][7:3], {1'b0,1'b0,1'b0}} );

	add51 <=  (difference_matrix[12][8][7:0] + {difference_matrix[12][9][7:2], {1'b0,1'b0}} ) + ({difference_matrix[12][10][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[12][11][7:2], {1'b0,1'b0}} );

	add52 <=  ({difference_matrix[12][12][7:1], {1'b0}}  + {difference_matrix[12][13][7:2], {1'b0,1'b0}} ) + (difference_matrix[12][14][7:0] + {difference_matrix[12][15][7:3], {1'b0,1'b0,1'b0}} );

	add53 <=  (difference_matrix[13][0][7:0] + difference_matrix[13][1][7:0]) + ({difference_matrix[13][2][7:2], {1'b0,1'b0}}  + difference_matrix[13][3][7:0]);

	add54 <=  ({difference_matrix[13][4][7:1], {1'b0}}  + {difference_matrix[13][5][7:1], {1'b0}} ) + ({difference_matrix[13][6][7:2], {1'b0,1'b0}}  + {difference_matrix[13][7][7:2], {1'b0,1'b0}} );

	add55 <=  ({difference_matrix[13][8][7:2], {1'b0,1'b0}}  + {difference_matrix[13][9][7:1], {1'b0}} ) + ({difference_matrix[13][10][7:2], {1'b0,1'b0}}  | {difference_matrix[13][11][7:2], {1'b0,1'b0}} );

	add56 <=  ({difference_matrix[13][12][7:1], {1'b0}}  + difference_matrix[13][13][7:0]) + ({difference_matrix[13][14][7:3], {1'b0,1'b0,1'b0}}  | difference_matrix[13][15][7:0]);

	add57 <=  ({difference_matrix[14][0][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[14][1][7:3], {1'b0,1'b0,1'b0}} ) + (difference_matrix[14][2][7:0] | difference_matrix[14][3][7:0]);

	add58 <=  ({difference_matrix[14][4][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[14][5][7:1], {1'b0}} ) + ({difference_matrix[14][6][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[14][7][7:1], {1'b0}} );

	add59 <=  ({difference_matrix[14][8][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[14][9][7:1], {1'b0}} ) + ({difference_matrix[14][10][7:1], {1'b0}}  | difference_matrix[14][11][7:0]);

	add60 <=  (difference_matrix[14][12][7:0] + difference_matrix[14][13][7:0]) + (difference_matrix[14][14][7:0] + difference_matrix[14][15][7:0]);

	add61 <=  ({difference_matrix[15][0][7:3], {1'b0,1'b0,1'b0}}  | difference_matrix[15][1][7:0]) | ({difference_matrix[15][2][7:3], {1'b0,1'b0,1'b0}}  + {difference_matrix[15][3][7:3], {1'b0,1'b0,1'b0}} );

	add62 <=  ({difference_matrix[15][4][7:1], {1'b0}}  + {difference_matrix[15][5][7:2], {1'b0,1'b0}} ) + (difference_matrix[15][6][7:0] + difference_matrix[15][7][7:0]);

	add63 <=  ({difference_matrix[15][8][7:1], {1'b0}}  | {difference_matrix[15][9][7:3], {1'b0,1'b0,1'b0}} ) | ({difference_matrix[15][10][7:2], {1'b0,1'b0}}  | difference_matrix[15][11][7:0]);

	add64 <=  ({difference_matrix[15][12][7:2], {1'b0,1'b0}}  | {difference_matrix[15][13][7:3], {1'b0,1'b0,1'b0}} ) | ({difference_matrix[15][14][7:2], {1'b0,1'b0}}  | difference_matrix[15][15][7:0]);


end

always @ (posedge clk)
begin
	SAD01 <=  (add64 + (add52 | add56));

	SAD02 <=  ({add22[9:3], {1'b0,1'b0,1'b0}}  | (add01 | add05));

	SAD03 <=  ({add62[9:2], {1'b0,1'b0}}  | add54);

	SAD04 <=  ({add49[9:2], {1'b0,1'b0}}  | {add53[9:2], {1'b0,1'b0}} ) | (add57 | add61);

	SAD05 <=  ({add48[9:3], {1'b0,1'b0,1'b0}}  | ({add36[9:3], {1'b0,1'b0,1'b0}}  | add40));

	SAD06 <=  ({add35[9:1], {1'b0}}  | {add39[9:1], {1'b0}} ) | ({add43[9:3], {1'b0,1'b0,1'b0}}  + {add47[9:1], {1'b0}} );

	SAD07 <=  (add34 | {add38[9:2], {1'b0,1'b0}} ) | ({add42[9:1], {1'b0}}  + {add46[9:1], {1'b0}} );

	SAD08 <=  ({add33[9:1], {1'b0}}  | {add37[9:1], {1'b0}} ) | ({add41[9:1], {1'b0}}  + {add45[9:2], {1'b0,1'b0}} );

	SAD09 <=  ({add20[9:1], {1'b0}}  | {add24[9:3], {1'b0,1'b0,1'b0}} ) | ({add28[9:1], {1'b0}}  + {add32[9:3], {1'b0,1'b0,1'b0}} );

	SAD10 <=  ({add19[9:3], {1'b0,1'b0,1'b0}}  | {add23[9:2], {1'b0,1'b0}} ) | ({add27[9:1], {1'b0}}  + add31);

	SAD11 <=  ({add55[9:3], {1'b0,1'b0,1'b0}}  | add63);

	SAD12 <=  ({add29[9:3], {1'b0,1'b0,1'b0}}  | ({add17[9:3], {1'b0,1'b0,1'b0}}  | add21));

	SAD13 <=  (add08 | {add16[9:2], {1'b0,1'b0}} );

	SAD14 <=  ({add15[9:3], {1'b0,1'b0,1'b0}}  | ({add03[9:3], {1'b0,1'b0,1'b0}}  | add07));

	SAD15 <=  ({add06[9:3], {1'b0,1'b0,1'b0}}  | {add14[9:3], {1'b0,1'b0,1'b0}} );

	SAD16 <= (add30);

end

endmodule
