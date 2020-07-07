import sys
import os
import shutil
import subprocess
import string
import nsga2_alg
from operator import itemgetter

design_name='blockmatching'
path='/home/sreda/Tools/ABACUS/blockmatching'
ACC_THRESH=90
#design_name = sys.argv[1]
#path = sys.argv[2]
#ACC_THRESH = sys.argv[3]

sys.path.insert(1, path)
import qor

def get_config_var(config, var_name):
	for i in range(len(config)):
		if config[i].startswith(var_name+'='):
			veri=config[i].replace(var_name+'=', '')
			veri=veri.strip(' "\n')
			break
	return veri

def synthesize_design(path, cwd):
	area=0
	os.chdir(path)
	syn_results=subprocess.run(['yosys', '-s', 'yosys.script'], capture_output=True, text=True)
	area = -1			
	for line in syn_results.stdout.splitlines():
	    fields = line.strip().split()
	    if len(fields) > 0:
	    	if fields[0] == 'Chip' and fields[1]=='area':
	    		area = fields[5]
	os.chdir(cwd)
	return float(area)

cwd=os.getcwd()

print('Running ABACUS v1.2 flow using ' + design_name + ' config file');
os.chdir(path)
#config_file = open(design_name + '.config2')
#config = config_file.readlines()

# copy files from original to source
source = os.listdir(path+'/Original')
for file in source:
	if '.v' in file:
		shutil.copy(path+'/Original/'+file, path+'/SRC/'+file)

# synthesize the original design 
print('Synthesizing original design')
original_area=synthesize_design(path, cwd)


print('Simulating original design')
sim_results=subprocess.run([path+'/sim.script', path+'/simulation', path+'/Original'], capture_output=False, text=True)

Num_Generation = 5
Num_perGen = 10
MAX_SEL = 3
source = os.listdir(path+'/Original')
for file in source:
	if '.v' in file:
		shutil.copy(path+'/Original/'+file, path+'/SRC/'+file)
shutil.move(path+'/SRC/'+design_name+'.v', path+'/SRC/'+design_name+'1.v')

Generation = 1
while Generation <= Num_Generation:
	gen_file = open(path+'/Population/FilesInfo_G'+str(Generation)+'.txt', 'w')
	gen_results=[];
	if Generation == 1:
		NUM_SEL = 1
	else:
		NUM_SEL = MAX_SEL
	SelNo = 1
	while SelNo <= NUM_SEL:
		VlogFile = 1	
		while VlogFile <= Num_perGen:
			os.chdir(path+'/SRC')	
			print('Working with design '+design_name+str(SelNo)+'.v')
			subprocess.run([cwd+'/ABACUS/ABACUS', '-V', design_name+str(SelNo)+'.v', '-A'])
			if os.path.exists(design_name+'_fromAST.v'):
				shutil.move(design_name+'_fromAST.v', design_name+'.v')
				os.chdir(path+'/simulation')
				sim_results=subprocess.run([path+'/sim.script', path+'/simulation', path+'/SRC'], capture_output=False, text=True)
				mean_acc, min_acc = qor.data_compare(path+'/Original', path+'/SRC')
				if mean_acc < ACC_THRESH:
					print('design with large error - skipping')
					continue
				approx_area=synthesize_design(path, cwd)
				if approx_area == -1:
					print('synthesis failed - skipping')
					continue
				approx_fname=design_name+'_G'+str(Generation)+'_S'+str(SelNo)+'_F'+str(VlogFile)
				design_tuple=(approx_fname, (mean_acc, 100*(original_area-approx_area)/original_area))
				gen_results.append(design_tuple)
				gen_file.write(approx_fname+'\t'+str(mean_acc)+ '\t'+ str(min_acc) +'\t'+ str(100*(original_area-approx_area)/original_area)+'\n')
				os.chdir(path+'/SRC')
				shutil.move(design_name+'.v', path+'/Population/'+approx_fname+'.v')
				VlogFile = VlogFile + 1
		SelNo = SelNo + 1
	print(gen_results)
	result=nsga2_alg.nsga2(gen_results)
	Generation = Generation + 1 
	gen_file.close()
	print('-- FINAL RESULT--')
	print([items[0] for items in result])
	fitness_rank=[0.6*items[1][0]+0.4*items[1][1] for items in result]
	final_result=[[items[0], items[1], fitness_rank[idx]] for idx, items in enumerate(result)]
	fitness_sorted_result = sorted(final_result, key=itemgetter(2), reverse=True)
	print(fitness_sorted_result)
	for idx,items in enumerate(fitness_sorted_result):
		srcFilename='%s/Population/%s.v' % (path, items[0])
		destFilename='%s/SRC/%s%d.v' % (path, design_name, (idx+2))
		print(srcFilename)
		print(destFilename)
		shutil.copyfile(srcFilename, destFilename)
	print('--END--\n')
