#FuDePAN 2011
#Before running your must compile nuca:
#g++ compression.cpp -o nuca -I../../nuca -DMILI_NAMESPACE -lbiopp -Wall -ansi -O3

import os
import json
import shutil
import numpy
import time
import matplotlib.pyplot
from subprocess import *

def save_graphics (nuca_data, zip_data, bzip2_data, y_range, file_out, n, len_f):
	
	figure = matplotlib.pyplot.figure(figsize=(12, 10))
	t = numpy.arange (5, 5 * n, 5)
	x = numpy.arange (5, 5 * (n + 1), 5)
	
	ax = figure.add_subplot (111)
	ax.plot (t, nuca_data, 'o-', label = "NUCA")
	ax.plot (t, zip_data, 'o-', label = "Zip")
	ax.plot (t, bzip2_data, 'o-', label = "Bzip2")
	matplotlib.pyplot.yticks (y_range)
	
	ax.set_xticks (x)
	ax.set_xticklabels (map (lambda x : str (x), numpy.arange (len_f, len_f * n + 1, len_f)))
	ax.grid (True)
	
	ax.legend (('NUCA', 'Zip', 'Bzip2'), bbox_to_anchor=(1, 1), loc=1, borderaxespad=0)
	figure.autofmt_xdate()
	figure.savefig (file_out)
	
def graph_average (nuca_d, zip_d, bzip2_d, names):
	
	lendata = len(nuca_d)
	width = 0.30
        
	figure = matplotlib.pyplot.figure(figsize=(12, 10))
        
	ax = figure.add_subplot (111)
	nuca_rect = ax.bar (range (lendata), nuca_d, width, facecolor='blue', align='center')
	zip_rect = ax.bar (map (lambda x : x + width, range (lendata)), zip_d, width, facecolor='green', align='center')
	bzip2_rect = ax.bar (map (lambda x : x + (width * 2), range (lendata)), bzip2_d, width, facecolor='red', align='center')
	matplotlib.pyplot.yticks (numpy.arange (0, 0.90, 0.03))
	ax.set_xticks (map (lambda x : x + width, range (lendata)))
	ax.set_xticklabels (names)
	ax.grid (True)
	ax.legend ((nuca_rect[0], zip_rect[0], bzip2_rect[0]), ("NUCA", "Zip", "Bzip2"), bbox_to_anchor=(1, 1), loc=1, borderaxespad=0)
	figure.autofmt_xdate()

	figure.savefig ("average.png")
	
	
def create_files (orig, size, name, n):
	
	reader = open (orig, "r")
	writer = open (name, "w")
	
	chars = 0
	valid = True
	
	while (chars < size) and valid:
		t = reader.read (1)
		
		if t:
			t = str(t).upper()
			
			if t == "A" or t == "C" or t == "T" or t == "G" or t == "N":
				writer.write (t)
				chars = chars + 1
		else:
			valid = False
			
	reader.close()
	writer.close()
	
	for i in range (1, n + 1):
		shutil.copy (name, name + str(i))
		
	os.remove (name)

	return valid
	
def calculate_percent (orig, finall):
	return (1.0 - (float(finall) / orig))
	
def calculate_compresion (args, size_str, filen, fil_del):
	
	start_time = time.time()
	proc = Popen (args, stdout=PIPE)
	proc.wait()
	total_time = time.time() - start_time
	per = calculate_percent(size_str, os.path.getsize(filen) - 1)
	os.remove (filen)
	
	if (os.path.isfile (fil_del)):
		os.remove (fil_del)
	
	return (per, total_time)
	
if __name__ == "__main__":
	data = json.load(open("data.json"))
	
	nuca_list_average = []
	zip_list_average = []
	bzip2_list_average = []

	for chromosomes in data["chromosomes"]:
		
		nuca_cmpr = []
		bzip2_cmpr = []
		zip_cmpr = []
		nuca_exec = []
		bzip2_exec = []
		zip_exec = []
		nuca_average = 0
		zip_average = 0
		bzip2_average = 0
		
		n = 1
		
		size = data["len"]
		filen = chromosomes["name"] + str(size)
		
		valid = True
		
		while valid:
			print "Len: " + str (size)
			
			valid = create_files (chromosomes["file"], size, filen, 3)
			
			nuca_pair = calculate_compresion (["./nuca", filen + str(1), filen + str(1) + ".nuca"], size, filen + str(1) + ".nuca", filen + str(1))
			zip_pair = calculate_compresion (["gzip", filen + str (2)], size, filen + str (2) + ".gz", filen + str(2))
			bzip2_pair = calculate_compresion (["bzip2", filen + str (3)], size, filen + str (3) + ".bz2", filen + str(3))
			
			nuca_cmpr.append (nuca_pair[0])
			zip_cmpr.append (zip_pair[0])
			bzip2_cmpr.append (bzip2_pair[0])
			
			nuca_average = nuca_average + nuca_pair[0]
			zip_average = zip_average + zip_pair[0]
			bzip2_average = bzip2_average + bzip2_pair[0]
			
			nuca_exec.append (nuca_pair[1])
			zip_exec.append (zip_pair[1])
			bzip2_exec.append (bzip2_pair[1])
			
			size = size + data["len"]
			filen = chromosomes["name"] + str(size)
			n = n + 1
			
		save_graphics (nuca_cmpr, zip_cmpr, bzip2_cmpr, numpy.arange (0, 0.90, 0.03), chromosomes["out_compr"], n, data["len"])
		save_graphics (nuca_exec, zip_exec, bzip2_exec, numpy.arange (0, 60, 2), chromosomes["out_exec"], n, data["len"])
		
		nuca_list_average.append (nuca_average / float(n - 1))
		zip_list_average.append (zip_average / float(n - 1))
		bzip2_list_average.append (bzip2_average / float(n - 1))
		
	graph_average (nuca_list_average, zip_list_average, bzip2_list_average, [x["show"] for x in data["chromosomes"]])
