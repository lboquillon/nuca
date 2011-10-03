#FuDePAN 2011
#Before running your must compile nuca:
#g++ compression.cpp -o nuca -I../../nuca -DMILI_NAMESPACE -lbiopp -Wall -ansi

import os
import json
import shutil
import numpy
import matplotlib.pyplot
from subprocess import *

def suml (li, value):
	ret = []
	
	for i in li:
		ret.append (i + value)
		
	return ret

def save_graphics (nuca_data, zip_data, bzip2_data, file_out, seq):
	
	lendata = len(nuca_data)
	width = 0.30
	
	figure = matplotlib.pyplot.figure(figsize=(12, 10))
	
	ax = figure.add_subplot (111)
	nuca_rect = ax.bar (range (lendata), nuca_data, width, facecolor='blue', align='center')
	zip_rect = ax.bar (suml (range (lendata), width), zip_data, width, facecolor='red', align='center')
	bzip2_rect = ax.bar (suml (range (lendata), width * 2), bzip2_data, width, facecolor='green', align='center')
	matplotlib.pyplot.yticks (numpy.arange (0, 0.90, 0.03))
	ax.set_xticks (suml (range (lendata), width))
	ax.set_xticklabels (seq)
	ax.legend ((nuca_rect[0], zip_rect[0], bzip2_rect[0]), ("NUCA", "Zip", "Bzip2"))
	figure.autofmt_xdate()

	figure.savefig (file_out)
	
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

def nuca_out (l, filen):
	
	ret = calculate_percent(l, int(Popen(["./nuca", filen], stdout=PIPE).communicate()[0]))

	os.remove (filen)
	
	return ret

def zip_out (l, filen):
	
	Popen(["gzip", filen], stdout=PIPE).communicate()[0]
	
	ret = calculate_percent(l, os.path.getsize(filen + ".gz") - 1)
 	
	os.remove (filen + ".gz")
	
	return ret
	
def bzip2_out (l, filen):
	
	Popen(["bzip2", filen], stdout=PIPE).communicate()[0]
	
	ret = calculate_percent(l, os.path.getsize(filen + ".bz2") - 1)
	
	os.remove (filen + ".bz2")
	
	return ret
	
if __name__ == "__main__":
	data = json.load(open("data.json"))

	for chromosomes in data["chromosomes"]:
		
		nuca_l = []
		bzip2_l = []
		zip_l = []
		n = 1
		
		size = data["len"]
		filen = chromosomes["name"] + str(size)
		
		valid = True
		
		while valid:
			print "Len: " + str (size)
			
			valid = create_files (chromosomes["file"], size, filen, 3)
			
			nuca_l.append (nuca_out (size, filen + str(1)))
			zip_l.append (zip_out (size, filen + str(2)))
			bzip2_l.append (bzip2_out (size, filen + str(3)))
			
			print nuca_l, zip_l, bzip2_l
			size = size + data["len"]
			filen = chromosomes["name"] + str(size)
			n = n + 1
			
		save_graphics (nuca_l, zip_l, bzip2_l, chromosomes["out"], numpy.arange (data["len"], n * data["len"], data["len"]))
