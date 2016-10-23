import time
import os
import numpy
import string

threshold=60
DESIGN_DIR=os.environ['DESIGN_DIR']
DESIGN_NAME=os.environ['DESIGN_NAME']

def output_to_wav(filename, testbenchNo, node):
		f=open(filename)
		f1=open('%s/simulation_%d/testcase%d/converted.txt'%(DESIGN_DIR,node,testbenchNo),'w')

		data=[]
		data=f.readlines()

		numPoints = len(data);
		dataout = [0] * numPoints;

		f.close()
		#print len(data[1])

		#for m in range(53, numPoints-26):
		#	sum=0
		#	for i in range(2, len(data[1])-3):
		#		if (data[m][1]=='1'):
		#			for q in range(2,len(data[m])-1):
		#				if (data[m][q]=='0'):
		#					#data[m][q]=1
		#					string.replace(data[m][1],'0','1')
		#				elif(data[m][q]=='1'):
		#					string.replace(data[m][q],'1','0')
		#			#print data[m]
		#
		#			data[m]=int(data[m][1:len(data[m])-3],2)+1
		#			data[m]=numpy.binary_repr(data[m]).zfill(16)
		#
		#
		#		sum=sum+int(data[m][i])*pow(2,-(i-1))
		#		if (data[m][2]=='1'):
		#			sum = -sum

		for i in range(53, numPoints-26):
		#for i in range(0, 2):
			temp=data[i]
			#print temp

			tempx=[]
			temparray=''

			if (temp[1]=='1'):
				#print "I am here"
				for q in range (2, len(data[i])-3):
					if (temp[q]=='1'):
						#data[m][q]=1
						tempx='0'
					elif(temp[q]=='0'):
						tempx='1'
					temparray+=tempx
				#print temparray
				data[i]=int(temparray[1:len(temparray)],2)+1
				data[i]=numpy.binary_repr(data[i]).zfill(14)

			#print data[i]
			#print (len(data[i]))
			sum=0
			if (data[i][1]!='x'):
				if (len(data[i])>18):
					for j in range(1, len(data[i])-3):
						sum=sum+int(data[i][j])*pow(2,-(j-1))
				else:
					#for j in range(0, len(data[i])):
					for j in range(1, len(data[i])-2):
						#print data[i][j]
						sum=sum+int(data[i][j])*pow(2,-(j+1))

				if (len(data[i])<=18):
					sum = -sum


			#print sum
			dataout[i-1]=sum
			#dataout[i-53]=sum
			f1.write('%s\n'%sum)

			#print sum
		f1.close()
		#for n in range(numPoints-26,numPoints):
		#	dataout[n]=dataout[numPoints-25];

		#print "success"
		return dataout


def dataCompare(generation, selNo, vLogFile, numTestCases, node):

	status='success';
	originalData=[0]*numTestCases
	modifiedData=[0]*numTestCases
	groundtruthData=[0]*numTestCases

	#for i in range(1,numTestCases+1):
		#filename='../Original/output_test%d/output.txt'%i
		#if (os.path.exists(filename)):
		#	originalData[i-1]=output_to_wav(filename)
		#else:
		#	#print("No original file output.txt in output_test%d" %i)
		#	status=-1

	
	for j in range(1,numTestCases+1):
		filename='%s/simulation_%d/testcase%d/output.txt'%(DESIGN_DIR,node, j)
		if (os.path.exists(filename)):
			output_to_wav(filename,j, node)
		else:
			#print("No modified file output.txt in output_test%d" %j)
			status='fail'

	for k in range(1,numTestCases+1):
		filename='%s/simulation_%d/testcase%d/Ground_truth_train%d.txt'%(DESIGN_DIR,node,k,k)
		if (os.path.exists(filename)):
			f1 = open(filename)
			temp = []
			temp = f1.readlines()
			groundtruthData[k-1]=temp
			f1.close()
			#print "success"
		else:
			#print("No file output.txt in SRC_test%d" %k)
			status='fail'
		filename='%s/simulation_%d/testcase%d/converted.txt'%(DESIGN_DIR, node,k)
		if (os.path.exists(filename)):
			f2=open('%s/simulation_%d/testcase%d/converted.txt'%(DESIGN_DIR, node,k))
			tempconverted = []
			tempconverted=f2.readlines()
			modifiedData[k-1]=tempconverted
			f2.close()
		#else:
		#	print "no can do"

	avg_array=[None]*numTestCases

	for x in range(0, numTestCases):
		temp=[0]*len(groundtruthData[x])
		for i in range(0,len(groundtruthData[x])):
			temp[i]=float(groundtruthData[x][i])

		temp2=[0]*len(modifiedData[x])
		for i in range(0,len(modifiedData[x])):
			temp2[i]=float(modifiedData[x][i])

		diffsq = [pow((a - b),2) for a, b in zip(temp[25:len(groundtruthData[x])],temp2[0:len(modifiedData[x])])]

		avg = numpy.mean(diffsq)
        	sigsq = [pow(a,2) for a in temp[25:len(groundtruthData[x])]]
        	#print "error: %g" %numpy.mean(diffsq)
        	#print "orig. signal: %g"%numpy.mean(sigsq)

        	avg=100 - ((numpy.mean(diffsq))/(numpy.mean(sigsq)))*100
		avg_array[x]=avg
	print avg_array

	avgAll = numpy.mean(avg_array)
	print avgAll

	if (min(avg_array)<=threshold):
		print "Accuracy not accepted. Discard and repeat Generation%d File%d"%(generation,vLogFile)
		status='fail'
		return status

	dump_file='%s/Population/node%d.txt'%(DESIGN_DIR,node)
	if(os.path.exists(dump_file)):
		f2=open(dump_file,'a')
		f2.write('%s_G%d_S%d_F%d\t\t%g\t\t%g\t\t'%(DESIGN_NAME, generation, selNo, vLogFile, avgAll, min(avg_array)))
		f2.close()
		print "Successfully wrote to FilesInfo_G%d.txt" %generation
	else:
		f2=open(dump_file,'w')
		f2.write('%s_G%d_S%d_F%d\t\t%g\t\t%g\t\t'%(DESIGN_NAME, generation, selNo, vLogFile, avgAll, min(avg_array)))
		f2.close()
		print "Successfully wrote to FilesInfo_G%d.txt" %generation
	return status

def main(generation, selNo, vLogFile, numTestCases, node):

	start_time=time.clock()
	status=dataCompare(generation,selNo,vLogFile,numTestCases, node)
	end_time=time.clock()-start_time

	if (status=='success'):
		#print("--Data Comparison and Quality of Solution Analysis Complete--")
		print("Elapsed Runtime: %gs" %end_time)
		print status
	else:
		#print "Flow failed"
		print status
