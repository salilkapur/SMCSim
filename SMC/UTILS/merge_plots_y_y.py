#!/usr/bin/python
# Plots two statistics versus each other, generates both GNUPLOT and CSV output formats
import sys
##################
BLUE = '\033[94m'
GREEN = '\033[92m'
YELLOW = '\033[93m'
NOCOLOR = '\033[0m'
##################

base = sys.argv[1]
p1 = sys.argv[2]
p2 = sys.argv[3]

pf1 = open( base + "/plot_" + p1 + ".gnu", "r" )
pf2 = open( base + "/plot_" + p2 + ".gnu", "r" )

of = open( base + "/plot_" + p1 + "_vs_" + p2 + ".gnu", "w" )
of2 = open( base + "/plot_" + p1 + "_vs_" + p2 + ".csv", "w" )

#########################################
l1 = []
l2 = []
title = ""
index = 0;

for line in pf1:
	tokens = line.split()
	t = tokens[len(tokens)-1]
	l1.append(t);
	
for line in pf2:
	tokens = line.split()
	t = tokens[len(tokens)-1]
	l2.append(t);
	
i = 0;
for t in l2:
	of.write( str(t) + " " + str(l1[i]) + "\n");
	i += 1
of.close();

i = 0;
for t in l2:
	of2.write( str(t) + ", " + str(l1[i]) + "\n");
	i += 1
of2.close();
