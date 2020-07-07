from operator import itemgetter

#indiv as a pair (indiv,fitness) if true a>b else a>/b
def dominates(a,b):
  assert(len(a[1])==len(b[1]))
  found_criteria=False
  for i in range(len(a[1])):
    if a[1][i]<b[1][i]:
      return False
    if a[1][i]>b[1][i]:
      found_criteria=True
  return found_criteria

def check_dominated(indiv,pop): #check if no element from pop (c_indiv) dominates indiv
  for c_indiv in pop:#for each c_indiv test if c_indiv dominates indiv
    c_not_dominate=False #c_indiv dominates indiv
    if dominates(c_indiv,indiv):
      return True
  return False

#[indiv,fitness,front,distance]
def find_non_dom_front(pop):
  new_pop=[]
  rest_pop=[]
  for i in pop:
    new_pop.append(i)
    #print new_pop #KN
    remove_i=False
    for j in new_pop[:-1]:
        #print "check dominition" #KN
        if dominates(i,j):
          new_pop.remove(j)
          rest_pop.append(j)          
        if dominates(j,i):
          remove_i=True
    if remove_i:
      new_pop.remove(i)
      rest_pop.append(i)
  assert(len(new_pop)+len(rest_pop)==len(pop))
 

  return new_pop, rest_pop

def non_dom_sort_two_objs(pop):
  pop=sorted(pop,key=lambda indiv: indiv[1][1],reverse=True)
  pop=sorted(pop,key=lambda indiv: indiv[1][0],reverse=True)
  fronts=[[pop[0]]]
  e=0
  for i in range(2,len(pop)):
    if check_dominated(pop[i],fronts[e])==False:
      k=0
      while( e-k>=0 and check_dominated(pop[i],fronts[e-k])==False):
          k+=1
      fronts[e-(k-1)].append(pop[i])
    else:
      e+=1
      fronts.append([pop[i]])
  return fronts

#give the distance for all individuals in a population, pop contains pair indiv + fitness
#[indiv,fitness,front,distance]
def compute_distance(pop):
  if len(pop)>1:
    l=len(pop)
    nb_objs=len(pop[0][1])
    print("Number of Objectives: %d" %nb_objs)
    for element in pop:
      assert(len(element)==3)
      element.append(0)
    for obj in range(nb_objs):
      pop=sorted(pop,key=lambda indiv: indiv[1][obj])
      pop[0][3]=float("inf")
      pop[-1][3]=float("inf")
      for i in range(len(pop)-2):
        assert(pop[i+2][1][obj]-pop[i][1][obj]>=0)
        pop[i+1][3]+=(pop[i+2][1][obj]-pop[i][1][obj])
    return pop
  else:
    return pop

def nsga2( population ): #takes 2-uples of (indiv,fitness)
  print(population)
  new_gen_pop_len=4
  assert len( population ) > 0
  #assert len( population )%2 == 0
  if (len(population)) < 4:
    new_gen_pop_len=len(population)
  N = len(population)
  #N = len(population)/2
  #print N
  i = 0
  selected_pop = []
  rest_pop=population
#sorting with respect to pareto fronts
  '''
  #KN
  print "--Original Input--"
  print rest_pop
  print "-----\n"
  
'''
  #print rest_pop[0]
  if len(rest_pop)==2:
    fronts=non_dom_sort_two_objs(rest_pop)
    #KN
    print("--Non Dom Sort Two Objects--")
    print(fronts)
    print("------\n")

    for front in fronts:
      if len( selected_pop )< N :
        selected_pop+=[[element[0],element[1],i] for element in front]
        i+=1
  else:
    while len( selected_pop ) < N: #add the next pareto front
      new_front,rest_pop=find_non_dom_front(rest_pop)
      #print new_front
      #print rest_pop
      print("--New Front--")
      print(new_front)
      print("------\n")

      print("--Rest Population--")
      print(rest_pop)
      print("------\n")
          
      selected_pop+=[[element[0],element[1],i] for element in new_front]
      i+=1
    #KN

#getting the distances
  selected_pop=compute_distance(selected_pop)
  print("--Final Selection Pool with fitness, pareto front and distance--")
  print(selected_pop)
  print("------\n")

  selected_pop=sorted(selected_pop,key=itemgetter(3),reverse=True) #secondary objective : distance, to be maximized
  print("--Final Selection Pool after sorting, round 1")
  print(selected_pop)
  print("------\n")
 
  selected_pop=sorted(selected_pop,key=itemgetter(2),reverse=False) #primary objective : pareto front : to minimize
  print("--Final Selection Pool after sorting, round2")
  print(selected_pop)
  print("------\n")
  
  return [[element[0],element[1]] for element in selected_pop[0:new_gen_pop_len]] 
								#KN was selected_pop[0:len(selected_pop)/2]
    
