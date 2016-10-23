#from evolve import  evolve
from nsga2 import nsga2,find_non_dom_front,compute_distance,non_dom_sort_two_objs
from random import random
from math import exp
from pylab import *
from scipy import io
from operator import itemgetter
import shutil
import os

def RGBToHTMLColor(rgb_tuple):
    """ convert an (R, G, B) tuple to #RRGGBB """
    hexcolor = '#%02x%02x%02x' % rgb_tuple
    # that's it! '%02x' means zero-padded, 2-digit hex values
    return hexcolor

#testing input from matlab generated text file
CUR_DIR=os.environ['CUR_DIR']
DESIGN_DIR=os.environ['DESIGN_DIR']
DESIGN_NAME=os.environ['DESIGN_NAME']

f= open('%s/mat2py_new_nodist.txt' % CUR_DIR,'r')
pop2=[]
popmat=[]
popmat=f.readlines()
#for line in f:
for i in range(0, len(popmat)):
	pop2.append(eval(popmat[i]))
f.close

print "---TEST MATLAB INPUT---"
#print (popmat,)
#pop2=[]
#pop2.append(popmat[1])
print pop2
print "---END---\n\n"

result=[]
result2=result
result = nsga2(pop2)
print "-- FINAL RESULT--"
print [items[0] for items in result]
fitness_rank=[0.6*items[1][0]+0.4*items[1][1] for items in result]
final_result=[[items[0], items[1], fitness_rank[idx]] for idx, items in enumerate(result)]
fitness_sorted_result = sorted(final_result, key=itemgetter(2), reverse=True)
print fitness_sorted_result
for idx,items in enumerate(fitness_sorted_result):
	srcFilename='%s/Population/%s.v' % (DESIGN_DIR, items[0])
	destFilename='%s/SRC/%s%d.v' % (DESIGN_DIR, DESIGN_NAME, (idx+2))
	print srcFilename
	print destFilename
	shutil.copyfile(srcFilename, destFilename)
print "--END--\n"
