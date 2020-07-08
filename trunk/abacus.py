import sys
import os
import shutil
import subprocess
import logging
#import nsga2_alg
from operator import itemgetter

design_files=['me_sad_calculation.v']
path='/home/sreda/Tools/ABACUS/blockmatching'
ACC_THRESH=90
#n_args=len(sys.argv)
#design_files=[]
#for i in range(1, n_args-2):#
#	design_files.append(sys.argv[i])
#path=sys.argv[n_args-2]
#ACC_THRESH=sys.argv[n_args-1]
#design_name = sys.argv[1]
#path = sys.argv[2]
#ACC_THRESH = sys.argv[3]

sys.path.insert(1, path)
import qor

def dominates(a, b):
	assert (len(a[1]) == len(b[1]))
	found_criteria = False
	for i in range(len(a[1])):
		if a[1][i] < b[1][i]:
			return False
		if a[1][i] > b[1][i]:
			found_criteria = True
	return found_criteria


def check_dominated(indiv, pop):  # check if no element from pop (c_indiv) dominates indiv
	for c_indiv in pop:  # for each c_indiv test if c_indiv dominates indiv
		c_not_dominate = False  # c_indiv dominates indiv
		if dominates(c_indiv, indiv):
			return True
	return False


# [indiv,fitness,front,distance]
def find_non_dom_front(pop):
	new_pop = []
	rest_pop = []
	for i in pop:
		new_pop.append(i)
		# print new_pop #KN
		remove_i = False
		for j in new_pop[:-1]:
			# print "check dominition" #KN
			if dominates(i, j):
				new_pop.remove(j)
				rest_pop.append(j)
			if dominates(j, i):
				remove_i = True
		if remove_i:
			new_pop.remove(i)
			rest_pop.append(i)
	assert (len(new_pop) + len(rest_pop) == len(pop))

	return new_pop, rest_pop


def non_dom_sort_two_objs(pop):
	pop = sorted(pop, key=lambda indiv: indiv[1][1], reverse=True)
	pop = sorted(pop, key=lambda indiv: indiv[1][0], reverse=True)
	fronts = [[pop[0]]]
	e = 0
	for i in range(2, len(pop)):
		if check_dominated(pop[i], fronts[e]) == False:
			k = 0
			while (e - k >= 0 and check_dominated(pop[i], fronts[e - k]) == False):
				k += 1
			fronts[e - (k - 1)].append(pop[i])
		else:
			e += 1
			fronts.append([pop[i]])
	return fronts


# give the distance for all individuals in a population, pop contains pair indiv + fitness
# [indiv,fitness,front,distance]
def compute_distance(pop):
	if len(pop) > 1:
		l = len(pop)
		nb_objs = len(pop[0][1])
		print("Number of Objectives: %d" % nb_objs)
		for element in pop:
			assert (len(element) == 3)
			element.append(0)
		for obj in range(nb_objs):
			pop = sorted(pop, key=lambda indiv: indiv[1][obj])
			pop[0][3] = float("inf")
			pop[-1][3] = float("inf")
			for i in range(len(pop) - 2):
				assert (pop[i + 2][1][obj] - pop[i][1][obj] >= 0)
				pop[i + 1][3] += (pop[i + 2][1][obj] - pop[i][1][obj])
		return pop
	else:
		return pop


def nsga2(population):  # takes 2-uples of (indiv,fitness)
	print(population)
	new_gen_pop_len = 4
	assert len(population) > 0
	# assert len( population )%2 == 0
	if (len(population)) < 4:
		new_gen_pop_len = len(population)
	N = len(population)
	# N = len(population)/2
	# print N
	i = 0
	selected_pop = []
	rest_pop = population
	# sorting with respect to pareto fronts
	'''
    #KN
    print "--Original Input--"
    print rest_pop
    print "-----\n"
  
  '''
	# print rest_pop[0]
	if len(rest_pop) == 2:
		fronts = non_dom_sort_two_objs(rest_pop)
		# KN
		print("--Non Dom Sort Two Objects--")
		print(fronts)
		print("------\n")

		for front in fronts:
			if len(selected_pop) < N:
				selected_pop += [[element[0], element[1], i] for element in front]
				i += 1
	else:
		while len(selected_pop) < N:  # add the next pareto front
			new_front, rest_pop = find_non_dom_front(rest_pop)
			# print new_front
			# print rest_pop
			print("--New Front--")
			print(new_front)
			print("------\n")

			print("--Rest Population--")
			print(rest_pop)
			print("------\n")

			selected_pop += [[element[0], element[1], i] for element in new_front]
			i += 1
	# KN

	# getting the distances
	selected_pop = compute_distance(selected_pop)
	print("--Final Selection Pool with fitness, pareto front and distance--")
	print(selected_pop)
	print("------\n")

	selected_pop = sorted(selected_pop, key=itemgetter(3),
						  reverse=True)  # secondary objective : distance, to be maximized
	print("--Final Selection Pool after sorting, round 1")
	print(selected_pop)
	print("------\n")

	selected_pop = sorted(selected_pop, key=itemgetter(2),
						  reverse=False)  # primary objective : pareto front : to minimize
	print("--Final Selection Pool after sorting, round2")
	print(selected_pop)
	print("------\n")

	return [[element[0], element[1]] for element in selected_pop[0:new_gen_pop_len]]


def synthesize_design(path, cwd):
	area=0
	os.chdir(path)
	syn_results=subprocess.run(['yosys', '-s', 'yosys.script'], capture_output=True, text=True)
	#logging.debug(syn_results.replace('\\n', '\n'))
	logging.debug(syn_results.stdout)
	area = 0
	for line in syn_results.stdout.splitlines():
		fields = line.strip().split()
		if len(fields) > 0:
			if fields[0] == 'Chip' and fields[1]=='area':
				area += float(fields[5])
	os.chdir(cwd)
	return area

def merge_files(design_name, source):
	data=''
	for file in source:
		with open(file) as fp:
			data += fp.read()
	with open (design_name+'.v', 'w') as fp:
		fp.write(data)

cwd=os.getcwd()

print('Running ABACUS v1.1 flow using ' );
os.chdir(path)
file=design_files[0]
design_name = file[0:len(file) - 2]
logging.basicConfig(filename=design_name+'.log',level=logging.DEBUG)

# copy files from original to source
logging.debug('moving original design to SRC folder')
source = os.listdir(path+'/Original')
for file in source:
	if '.v' in file:
		shutil.copy(path+'/Original/'+file, path+'/SRC/'+file)

# synthesize the original design 
print('Synthesizing original design')
original_area=synthesize_design(path, cwd)
logging.debug('original design area ' + str(original_area))


print('Simulating original design')
sim_results=subprocess.run([path+'/sim.script', path+'/simulation', path+'/Original'], capture_output=True, text=True)
logging.debug(sim_results.stdout)

Num_Generation = 5
Num_perGen = 10
MAX_SEL = 3
#source = os.listdir(path+'/Original')
for file in design_files:
	design_name=file[0:len(file)-2]
	shutil.copy(path+'/Original/'+file, path+'/SRC/'+design_name+'1.v')
#shutil.move(path+'/SRC/'+design_name+'.v', path+'/SRC/'+design_name+'1.v')

Generation = 1
while Generation <= Num_Generation:
	print('-------------------')
	print('Generation '+str(Generation))
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
			status = 1
			for file in design_files:
				design_name = file[0:len(file) - 2]
				print('Approximating the design')
				abacus_out=subprocess.run([cwd+'/ABACUS/ABACUS', '-V', design_name+str(SelNo)+'.v', '-A'], capture_output=True, text=True)
				logging.debug(abacus_out.stdout)
				if os.path.exists(design_name+'_fromAST.v'):
					source = []
					folder_content = os.listdir('.')
					for f in folder_content:
						if '_fromAST.v' in f:
							source.append(f)
					merge_files(design_name, source)
				else:
					status=-1
			os.chdir(path+'/simulation')
			print('Simulating the approximate design')
			sim_results=subprocess.run([path+'/sim.script', path+'/simulation', path+'/SRC'], capture_output=True, text=True)
			logging.debug(sim_results.stdout)
			mean_acc, min_acc = qor.data_compare(path+'/Original', path+'/SRC')
			if mean_acc < ACC_THRESH:
				print('approximate design with large error - skipping')
				continue
			print('Synthesizing approximate design')
			approx_area=synthesize_design(path, cwd)
			if approx_area == 0:
				print('synthesis failed - skipping')
				continue
			approx_fname=design_name+'_G'+str(Generation)+'_S'+str(SelNo)+'_F'+str(VlogFile)
			print('Success, writing down approximate design ', approx_fname, 'in Population folder')
			design_tuple=(approx_fname, (mean_acc, 100*(original_area-approx_area)/original_area))
			gen_results.append(design_tuple)
			gen_file.write(approx_fname+'\t'+str(mean_acc)+ '\t'+ str(min_acc) +'\t'+ str(100*(original_area-approx_area)/original_area)+'\n')
			os.chdir(path+'/SRC')
			shutil.move(design_name+'.v', path+'/Population/'+approx_fname+'.v')
			VlogFile = VlogFile + 1
		SelNo = SelNo + 1
	print(gen_results)
	result = nsga2(gen_results)
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
