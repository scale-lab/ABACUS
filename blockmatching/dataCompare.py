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
DESIGN_NAME = os.environ['DESIGN_NAME']
DESIGN_DIR = os.environ['DESIGN_DIR']
Thresh = 20

def extractNumber(List):
    temp = []
    str1 = '\n'.join(List)
    Re = re.findall(r'\d+', str1)
    temp = sorted(Re, key=len)[-1]
    return temp
def QoS(Original, Modified, base, Generation, SelNo, VlogFile):
    im_width = 352
    im_height = 288
    status = 'success'
    rel_acc = [0] * len(Modified)
    #ModifiedDB = [0] * len(Modified)
    for i in range (0, len(Modified)):
        #ModifiedDB[i] = 10 * (math.log10(math.pow(255,2)/(Modified[i]/(im_width * im_height))))
		# L = min(len(Original[i]),len(Modified[i]))
        # Accuracy of original design:
        print "Mean square error for entire frame given by original design is: " , Original[i]
        # Accuracy of modified design:
        print "Mean square error for entire frame given by modified design is: " , Modified[i]
        # count the number of mismatches:
        rel_acc[i] = 100 - ((abs(float(Original[i] - Modified[i])) / Original[i]) * 100)
        print "Accuracy of modified design is : %0.4g %%\n\n" % rel_acc[i]

    if (min(rel_acc)<=Thresh):
        print "Accuracy not acceptable, discard and repeat Generation%d File%d" % (Generation,VLogFile)
        status = 'fail'
        return status

    file_to_write = '%s/Population/FilesInfo_G%d.txt' % (DESIGN_DIR, base)
    if(os.path.exists(file_to_write)):
        fid = open(file_to_write,'a')
        fid.write('%s_G%d_S%d_F%d\t\t' % (DESIGN_NAME, Generation, SelNo, VlogFile))
        fid.write('%0.4g\t\t%0.4g\t\t\n' % (numpy.mean(rel_acc), min(rel_acc)))
        fid.close()
    else:
        fid = open(file_to_write, 'w')
        fid.write('# Name \t\t\t Relative Accuracy \t\t Min Accuracy \n')
        fid.write('%s_G%d_S%d_F%d\t\t' % (DESIGN_NAME, Generation, SelNo, VlogFile))

        if (base == 0):
            fid.write('%0.4g\t\t%0.4g\t\t\n' % (numpy.mean(rel_acc), min(rel_acc)))
        else:
            fid.write('%0.4g\t\t%0.4g\t\t'% (numpy.mean(rel_acc), min(rel_acc)))
        fid.close()

def dataCompare(base, Generation, SelNo, VlogFile, Num_testCases):
    status = 'success'
    Original_MSE = [0] * Num_testCases
    Modified_MSE = [0] * Num_testCases
    for i in range (1, Num_testCases + 1):
        Original='%s/Original/testcase%d/psnr.txt' % (DESIGN_DIR, i)
        Modified='%s/simulation/testcase%d/psnr.txt' % (DESIGN_DIR, i)
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
    if (status == 'success'):
        try:
            QoS(Original_MSE, Modified_MSE,base, Generation, SelNo, VlogFile)
        except:
            status = 'fail'
    return status
#print dataCompare(0,0,1,1,4)
def main(base, Generation, SelNo, VlogFile, Num_testCases):

    start_time=time.clock()

    status=dataCompare(base, Generation, SelNo, VlogFile, Num_testCases)
    end_time=time.clock()-start_time

    if (status=='success'):
        #print("--Data Comparison and Quality of Solution Analysis Complete--")
        print("Elapsed Runtime: %gs" %end_time)
        print status
    else:
        #print "Flow failed"
        print status
#print main(0,0,1,1,4)
