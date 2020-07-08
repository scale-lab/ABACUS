#!/usr/bin/python
# This is the file needed to determine the QoS of each variant. More specifically, this code needs to
# compare the accuracy of the modified files against the accuracy of the original input and report the
# result.
import string
import time
import os
import numpy
import math
import re
#import ast

def extractNumber(List):
    temp = []
    str1 = '\n'.join(List)
    Re = re.findall(r'\d+', str1)
    temp = sorted(Re, key=len)[-1]
    return temp
def QoS(Original, Modified):
    im_width = 352
    im_height = 288
    status = 'success'
    rel_acc = [0] * len(Modified)
    #ModifiedDB = [0] * len(Modified)
    for i in range (0, len(Modified)):
        #ModifiedDB[i] = 10 * (math.log10(math.pow(255,2)/(Modified[i]/(im_width * im_height))))
		# L = min(len(Original[i]),len(Modified[i]))
        # Accuracy of original design:
        print("Mean square error for entire frame given by original design is: " , Original[i])
        # Accuracy of modified design:
        print("Mean square error for entire frame given by modified design is: " , Modified[i])
        # count the number of mismatches:
        rel_acc[i] = 100 - ((abs(float(Original[i] - Modified[i])) / Original[i]) * 100)
        print("Accuracy of modified design is : %0.4g %%\n\n" % rel_acc[i])

    return numpy.mean(rel_acc), numpy.min(rel_acc)

def data_compare(orig_path, approx_path):
    status = 'success'
    Num_testCases = 4
    Original_MSE = [0] * Num_testCases
    Modified_MSE = [0] * Num_testCases
    for i in range (1, Num_testCases + 1):
        Original=orig_path+'/simulation/testcase'+str(i)+'/psnr.txt'
        Modified=approx_path+'/simulation/testcase'+str(i)+'/psnr.txt'
        # read original data from file:
        if (os.path.exists(Original)):
            Data_1 = open(Original)
            temp_1 = []
            temp_1 = Data_1.readlines()
            Original_MSE [i-1] = int(extractNumber(temp_1))
        # read modified data from file:
        if (os.path.exists(Modified)):
            Data_2 = open(Modified)
            temp_2 = []
            temp_2 = Data_2.readlines()
            Modified_MSE [i-1] = int(extractNumber(temp_2))
        else:
            print("No output file for design testcase. Data analysis break...")
            status = 'fail'
    return QoS(Original_MSE, Modified_MSE)
#print dataCompare(0,0,1,1,4)

