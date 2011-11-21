#FuDePAN 2011
#Before running your must compile nuca:
#g++ compression.cpp -o nuca -I../../nuca -DMILI_NAMESPACE -lbiopp -Wall -ansi -O3

import os
import json
import shutil
import time
from subprocess import *

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
	
def calculate_compression (args, size_str, filen, fil_del):
	
	start_time = time.time()
	proc = Popen (args, stdout=PIPE)
	proc.wait()
	total_time = time.time() - start_time
	per = calculate_percent(size_str, os.path.getsize(filen) - 1)
	
	if (os.path.isfile (fil_del)):
		os.remove (fil_del)
	
	return (per, total_time)
	
def calculate_decompression (args, filen, delete_extra = ""):
	
	start_time = time.time()
	proc = Popen (args, stdout=PIPE)
	proc.wait()
	total_time = time.time() - start_time
	
	if os.path.isfile (filen):
		os.remove (filen)
		
	if delete_extra != "":
		os.remove (delete_extra)
		
	return total_time
	
	
if __name__ == "__main__":
	data_in = json.load(open("data_in.json"))
	
	data = { }
	data['data'] = { }

	for chromosomes in data_in["chromosomes"]:

		data['data'][chromosomes['name']] = { }
		data['data'][chromosomes['name']]['compression'] = {}
		data['data'][chromosomes['name']]['execution_compression'] = {}
		data['data'][chromosomes['name']]['execution_decompression'] = {}
		
		size = data_in["len"]
		filen = chromosomes["name"] + str(size)
		
		valid = True
		
		while valid:
			data['data'][chromosomes['name']]['compression'][str(size)] = {}
			data['data'][chromosomes['name']]['execution_compression'][str(size)] = {}
			data['data'][chromosomes['name']]['execution_decompression'][str(size)] = {}
						
			valid = create_files (chromosomes["file"], size, filen, 3)
			
			nuca_pair = calculate_compression (["./nuca", "-c", filen + str(1), filen + str(1) + ".nuca"], size, filen + str(1) + ".nuca", filen + str(1))
			zip_pair = calculate_compression (["gzip", filen + str (2)], size, filen + str (2) + ".gz", filen + str(2))
			bzip2_pair = calculate_compression (["bzip2", filen + str (3)], size, filen + str (3) + ".bz2", filen + str(3))
			
			data['data'][chromosomes['name']]['execution_decompression'][str(size)]['NUCA'] = calculate_decompression (["./nuca", "-d", filen + str(1) + ".nuca", filen + str(1)], filen + str(1) + ".nuca", filen + str(1))
			data['data'][chromosomes['name']]['execution_decompression'][str(size)]['Zip'] = calculate_decompression (["gunzip", filen + str (2) + ".gz"], filen + str(2))
			data['data'][chromosomes['name']]['execution_decompression'][str(size)]['Bzip2'] = calculate_decompression (["bunzip2", filen + str (3) + ".bz2"], filen + str(3))
			
			data['data'][chromosomes['name']]['compression'][str(size)]['NUCA'] = nuca_pair[0]
			data['data'][chromosomes['name']]['compression'][str(size)]['Zip'] = zip_pair[0]
			data['data'][chromosomes['name']]['compression'][str(size)]['Bzip2'] = bzip2_pair[0]
			
			data['data'][chromosomes['name']]['execution_compression'][str(size)]['NUCA'] = nuca_pair[1]
			data['data'][chromosomes['name']]['execution_compression'][str(size)]['Zip'] = zip_pair[1]
			data['data'][chromosomes['name']]['execution_compression'][str(size)]['Bzip2'] = bzip2_pair[1]
			
			size = size + data_in["len"]
			filen = chromosomes["name"] + str(size)
			
	f = open ('data.json', 'w')
	f.write (json.dumps (data, indent = 4, sort_keys = True))
	f.close()
