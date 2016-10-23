#!/usr/bin/python
import string
import os
import numpy
import math

thresh = 10
design_name=os.environ['DESIGN_NAME']
design_dir = os.environ['DESIGN_DIR']
'''
def decode_hex(string):
    try:
        s_base2 = bin(int(string, 16))[2:].zfill(16)
        if (s_base2[0] == '1'):
            s_base2= s_base2.replace("0","x")
            s_base2= s_base2.replace("1","0")
            s_base2= s_base2.replace("x","1")
            num = -int(s_base2,2)-1
        else:
            num = int(s_base2,2)
    except:
        num = 'NaN'
    return num
'''
def data_compare(Generation, SelNo, VlogFile, Numtestcases, node):
	pop_dir = ('%s/Population' % design_dir)
	if (len(design_name)==0 or len(pop_dir)==0 or len(design_dir)==0):
		print("Cannot read environment variable\n")
		print "fail"
		return
	else:
		print("DESIGN_NAME: %s\n"% design_name)
	fft_mse = numpy.zeros((Numtestcases,1))
	for k in range (1, Numtestcases +1):
		out_txt = ('%s/Original/testcase%d/outfft.txt'% (design_dir, k))
		if(os.path.exists(out_txt)):
			y_fft_orig = open(out_txt).readlines()
		else:
			print "Cannot locate simulation output files for original circuit!\n"
			print "fail"
			return
		y_fft_Re_org = [0] * len(y_fft_orig)
		y_fft_Im_org = [0] * len(y_fft_orig)
		for i in range(len(y_fft_orig)):
			element = y_fft_orig[i]
			m=int(element[0:4],16)
			y_fft_Re_org[i] = ((m+0x8000)&0xFFFF) - 0x8000
			m=int(element[4:8],16)
			y_fft_Im_org[i] = ((m+0x8000)&0xFFFF) - 0x8000
			#print "%d %d %d" % (i, y_fft_Re_org[i], y_fft_Im_org[i])
		for m in range(1,len(y_fft_Re_org)+1):
			if (y_fft_Re_org[m-1]=='NaN' or y_fft_Im_org[m-1]=='NaN'):
				print "Not a Number"
				print "fail"
				return
		#y_ifft_org= [sum(x) for x in zip(y_fft_Re_org , [1j* x for x in y_fft_Im_org])]

		out_txt = ('%s/simulation_%d/testcase%d/outfft.txt'% (design_dir,node, k))
		if (os.path.exists(out_txt)):
			y_fft = open(out_txt).readlines()
		else:
			print "Cannot locate simulation output files for modified circuit!\n"
			print "fail"
			return
		y_fft_Re = [0] * len(y_fft)
		y_fft_Im = [0] * len(y_fft)
		for n in range(len(y_fft)):
			element = y_fft[n]
			#print "%d" % n
			x = int(element[0:4], 16)
			y_fft_Re[n] = ((x+0x8000)&0xFFFF) - 0x8000
			x = int(element[4:8], 16)
			y_fft_Im[n] = ((x+0x8000)&0xFFFF) - 0x8000
		for m in range(1,len(y_fft_Re)+1):
			if (y_fft_Re[m-1]=='NaN' or y_fft_Im[m-1]=='NaN'):
				print "Not a Number"
				print "fail"
				return
		#y_ifft_mod= [sum(x) for x in zip(y_fft_Re , [1j* x for x in y_fft_Im])]
        # sum the real and the immagonary parts:
        #data_1 = [sum(x) for x in zip(y_fft_Re , [1j* h for h in y_fft_Im])]
        # take the inverse fft:
        #y_ifft_mod = numpy.fft.ifft(data_1)
        # subtract original from modifies:
		amplitude_mod = [math.sqrt(math.pow(i,2) + math.pow(j,2)) for i, j in zip(y_fft_Re, y_fft_Im)]
		amplitude_org = [math.sqrt(math.pow(i,2) + math.pow(j,2)) for i, j in zip(y_fft_Re_org, y_fft_Im_org)]
		#print amplitude_org
		#print amplitude_mod
		data_2 = [i - j for i, j in zip(amplitude_mod, amplitude_org)]
		#print data_2
        # take the real part:
		#data_3 = [g.real for g in data_2]
        # square it:
		diff_square= [x*x for x in data_2]
		#print diff_square
        # take the square of real part of y_ifft_orig:
		#data_4 = [x*x for x in [l.real for l in y_ifft_org]]
		data_4 = [x*x for x in amplitude_org]
		fft_mse[k-1] = (numpy.mean(diff_square)/numpy.mean(data_4))*100
		print('MSE: %f\n'% fft_mse[k-1])
		if (fft_mse[k-1] >= thresh):
			print "fail"
			return
	fft_accuracy = 100 - fft_mse
	filename = ('%s/node%d.txt'% (pop_dir, node))
	if (os.path.exists(filename)):
		fid = open(filename, 'a')
		fid.write('%s_G%d_S%d_F%d\t\t'% (design_name, Generation, SelNo, VlogFile))
		fid.write('%0.4g\t\t%0.4g\t\t'% (numpy.mean(fft_accuracy), min(fft_accuracy)))
		print "success"
		fid.close()
	else:
		fid = open(filename, 'w')
		fid.write('%s_G%d_S%d_F%d\t\t'% (design_name, Generation, SelNo, VlogFile))
		fid.write('%0.4g\t\t%0.4g\t\t'% (numpy.mean(fft_accuracy), min(fft_accuracy)))
		print "success"
		fid.close()
	return
def main(Generation, SelNo, VlogFile, NumtestCases, node):
    data_compare(Generation, SelNo, VlogFile, NumtestCases, node)
