#!/usr/bin/python
import os
import math
import numpy
def QoS (Original_data, Modified_data, Ground_truth, Generation, SelNo, VlogFile):
    DESIGN_NAME = os.environ['DESIGN_NAME']
    DESIGN_DIR = os.environ['DESIGN_DIR']
    Real_thresh = 400     # was 370
    status = 'success'
    org_error = [0] * len(Original_data)
    mod_error = [0] * len(Modified_data)
    rel_error = [0] * len(Modified_data)
    for i in range (1, len(Original_data)+1):
        L = min (len(Original_data[i-1]), len(Modified_data[i-1]))
    # Accuracy of original svm.v
    # org_error = 0
    temp = 0
    for n in range (1, len(Original_data)+1):
        for i in range(1, L+1):
            temp = temp + abs(float(Original_data[n-1][i-1]) - float(Ground_truth[n-1][i-1]))
        org_error[n-1]= temp
        temp = 0
        print ('False classification given by original svm.v is: %d\n Accuracy: %0.4g %%\n\n' % (org_error[n-1], (1000 - org_error[n-1])/10))

    # Accuracy of modified svm.v
    # mod_error =0
    temp=0
    for n in range (1, len(Modified_data)+1):
        for i in range(1, L+1):
            temp = temp + abs(float(Modified_data[n-1][i-1]) - float(Ground_truth[n-1][i-1]))
        mod_error[n-1]= temp
        temp = 0
        print ('False classification given by modified svm.v is: %d\n Accuracy: %0.4g %%\n\n' % (mod_error[n-1], (1000 - mod_error[n-1])/10))

    # Count the number of mismatches
    # rel_error =0
    temp = 0
    for n in range (1, len(Original_data)+1):
        for i in range (1, L+1):
            temp = temp + abs(float(Original_data[n-1][i-1])- float(Modified_data[n-1][i-1]))
        rel_error[n-1] = temp
        temp =0
        print ('The number of mismatches between two versions of svm.v is: %d\nRelative accuracy: %0.4g %%\n\n' % (rel_error[n-1], (1000-rel_error[n-1])/10))

    if (max(mod_error) >= Real_thresh):
        print "Accuracy not acceptable, discard and repeat Generation %d  Selection %d File %d" % (Generation, SelNo, VlogFile)
        status = 'fail'
        return status

    file_to_write = '%s/Population/FilesInfo_G%d.txt' % (DESIGN_DIR, Generation)
    if(os.path.exists(file_to_write)):
        fid = open(file_to_write,'a')
        fid.write('%s_G%d_S%d_F%d\t\t' % (DESIGN_NAME, Generation, SelNo, VlogFile))
        fid.write('%0.4g\t\t\t' % (((L-(numpy.mean(mod_error)))/L) * 100))
        fid.write('%0.4g\t\t' % (((L-max(mod_error))/float(L)) * 100))
        fid.close()
    else:
        fid = open(file_to_write, 'w')
        fid.write('%s_G%d_S%d_F%d\t\t' % (DESIGN_NAME, Generation, SelNo, VlogFile))
        print "mod_error: ", mod_error
        print "mean: ", numpy.mean(mod_error)
        print "max: ", max(mod_error)
        fid.write('%0.4g\t\t\t' % (((L-(numpy.mean(mod_error)))/L) * 100))
        fid.write('%0.4g\t\t' % (((L-max(mod_error))/float(L)) * 100))
        fid.close()

        print "Failed to open FilesInfo_G%d.txt, no data collected..." % Generation
        status = 'fail'
    return status

def dataCompare(Generation, SelNo, VlogFile, NumtestCases):
    status = 'success'
    DESIGN_DIR = os.environ['DESIGN_DIR'];
    Original_data = [0] * NumtestCases
    Modified_data = [0] * NumtestCases
    Ground_truth = [0] * NumtestCases
    for i in range(1, NumtestCases + 1):
        Original = '%s/Original/testcase%d/out_labels.txt' % (DESIGN_DIR, i)
        Modified = '%s/simulation/testcase%d/out_labels.txt' % (DESIGN_DIR, i)
        Groundtruth = '%s/simulation/testcase%d/Ground_truth.txt' % (DESIGN_DIR, i)

        if (os.path.exists(Original)):
            data = open (Original)
            temp = []
            temp = data.readlines()
            Original_data[i-1] = temp
        '''else:
            print "No output file for original design. Data analysis break..."
            status = 'fail'
        '''
        if (os.path.exists(Modified)):
            data = open(Modified)
            temp = []
            temp = data.readlines()
            Modified_data[i-1] = temp
        '''else:
            print "No output file fof Modified design. Data analysis break..."
            status = 'fail'
        '''
        if (os.path.exists(Groundtruth)):
            data = open(Groundtruth)
            temp = []
            temp = data.readlines()
            Ground_truth[i-1] = temp
        else:
            print "No Ground truth design. Data analysis break..."
            status = 'fail'
    if (status == 'success'):
        try:
            status = QoS (Original_data, Modified_data, Ground_truth, Generation, SelNo, VlogFile)
        except:
            status = 'fail'
    print "status:\n ", status
def main(Generation, SelNo, VlogFile, NumtestCases):
    dataCompare(Generation, SelNo, VlogFile, NumtestCases)
