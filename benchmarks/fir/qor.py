import time
import os
import numpy
import string

threshold=60

def output_to_wav(filename, approx_path, testbenchNo):
		f=open(filename)
		f1=open(approx_path+'/simulation/testcase'+str(testbenchNo)+'/converted.txt','w')

		data=[]
		data=f.readlines()

		numPoints = len(data);
		dataout = [0] * numPoints;

		f.close()

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


def data_compare(orig_path, approx_path):
	numTestCases=3
	status='success';
	originalData=[0]*numTestCases
	modifiedData=[0]*numTestCases
	groundtruthData=[0]*numTestCases

	for i in range(1,numTestCases+1):
		filename=orig_path+'/simulation/testcase'+str(i)+'/output.txt'
		if (os.path.exists(filename)):
			originalData[i-1]=output_to_wav(filename, orig_path, i)
		else:
			#print("No original file output.txt in output_test%d" %i)
			status=-1

	for i in range(1,numTestCases+1):
		filename=approx_path+'/simulation/testcase'+str(i)+'/output.txt'
		if (os.path.exists(filename)):
			modifiedData[i-1]=output_to_wav(filename, approx_path, i)
		else:
			#print("No original file output.txt in output_test%d" %i)
			status=-1
	
	#for j in range(1,numTestCases+1):
	#	filename=orig_path+'/simulation/testcase'+str(j)+'/output.txt'
	#	if (os.path.exists(filename)):
	#		output_to_wav(filename,j)
	#	else:
	#		#print("No modified file output.txt in output_test%d" %j)
	#		status='fail'

	for k in range(1,numTestCases+1):
		filename=orig_path+'/simulation/testcase'+str(k)+'/Ground_truth_train'+str(k)+'.txt'
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
	#	filename=approx_path+'/simulation/testcase'+str(k)+'/converted.txt'
	#	if (os.path.exists(filename)):
	#		f2=open(filename)
	#		tempconverted = []
	#		tempconverted=f2.readlines()
	#		print(tempconverted)
	#		modifiedData[k-1]=tempconverted
	#		f2.close()
		#else:
		#	print "no can do"


	avg_array=[None]*numTestCases

	groundtruthData=originalData
	for x in range(0, numTestCases):
		temp=[0]*len(groundtruthData[x])
		for i in range(0,len(groundtruthData[x])):
			temp[i]=float(groundtruthData[x][i])

		temp2=[0]*len(modifiedData[x])
		for i in range(0,len(modifiedData[x])):
			temp2[i]=float(modifiedData[x][i])

		diffsq = [pow((a - b),2) for a, b in zip(temp[0:len(groundtruthData[x])],temp2[0:len(modifiedData[x])])]
		avg = numpy.mean(diffsq)
		sigsq = [pow(a,2) for a in temp[0:len(groundtruthData[x])]]
		avg=100 - ((numpy.mean(diffsq))/(numpy.mean(sigsq)))*100
		avg_array[x]=avg
	#print(avg_array)

	avgAll = numpy.mean(avg_array)
	#print(avgAll)
	return avgAll, min(avg_array)


