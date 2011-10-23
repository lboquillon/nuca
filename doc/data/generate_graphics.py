#FuDePAN 2011

import json
import numpy
import matplotlib.pyplot

def save_graphics (nuca_data, zip_data, bzip2_data, y_range, file_out, n, len_f, y_label, x_label):
	
	figure = matplotlib.pyplot.figure(figsize = (12, 10))
	t = numpy.arange (5, 5 * n, 5)
	x = numpy.arange (5, 5 * (n + 1), 5)
	
	ax = figure.add_subplot (111)
	ax.plot (t, nuca_data, 'o-', label = "NUCA", color = '#2E00B8')
	ax.plot (t, zip_data, 'o-', label = "Zip", color = '#0033CC')
	ax.plot (t, bzip2_data, 'o-', label = "Bzip2", color = '#FF3300')
	matplotlib.pyplot.yticks (y_range)
	
	ax.set_xlabel (x_label)
	ax.set_ylabel (y_label)
	ax.set_xticks (x)
	ax.set_xticklabels (map (lambda x : str (x), numpy.arange (len_f, len_f * n + 1, len_f)))
	ax.grid (True)
	
	ax.legend (('NUCA', 'Zip', 'Bzip2'), bbox_to_anchor=(1, 1), loc=1, borderaxespad=0)
	figure.autofmt_xdate()
	figure.savefig (file_out)

if __name__ == "__main__":
	data_in = json.load (open ("data_in.json"))
	data = json.load(open ("data.json"))
	
	sum_compr = { 'NUCA' : {}, 'Zip' : {}, 'Bzip2' : {} }
	count = { }
	
	dnuca = []
	dzip = []
	dbzip2 = []
	
	for chro, data_chro in data['data'].iteritems():
		for key, value in data_chro['compression'].iteritems():
			if count.has_key(key):
				count[key] = 1 + count[key]
			else:
				count[key] = 1
				
	for i in count:
		sum_compr['NUCA'][i] = float (0)
		sum_compr['Zip'][i] = float (0)
		sum_compr['Bzip2'][i] = float (0)
				
	for chro, data_chro in data['data'].iteritems():		
		for key in sorted (data_chro['compression'], key = lambda x : int (x)):
			
			sum_compr['NUCA'][key] = sum_compr['NUCA'][key] + data_chro['compression'][key]['NUCA']
			sum_compr['Zip'][key] = sum_compr['Zip'][key] + data_chro['compression'][key]['Zip']
			sum_compr['Bzip2'][key] = sum_compr['Bzip2'][key] + data_chro['compression'][key]['Bzip2']
			
			dnuca.append (data_chro['compression'][key]['NUCA'])
			dzip.append (data_chro['compression'][key]['Zip'])
			dbzip2.append (data_chro['compression'][key]['Bzip2'])

		save_graphics (dnuca, dzip, dbzip2, numpy.arange (0, 0.90, 0.03), chro + '_compression.png', 1 + len (dnuca), data_in['len'], 'PERCENT', 'LENGTH')
		
		del dnuca[:]
		del dzip[:]
		del dbzip2[:]
			
		for key in sorted (data_chro['execution_compression'], key = lambda x : int (x)):
			
			dnuca.append (data_chro['execution_compression'][key]['NUCA'])
			dzip.append (data_chro['execution_compression'][key]['Zip'])
			dbzip2.append (data_chro['execution_compression'][key]['Bzip2'])
		
		save_graphics (dnuca, dzip, dbzip2, numpy.arange (0, 60, 2), chro + '_execution_compression.png', 1 + len (dnuca), data_in['len'], 'SECONDS', 'LENGTH')
		
		del dnuca[:]
		del dzip[:]
		del dbzip2[:]
			
		for key in sorted (data_chro['execution_decompression'], key = lambda x : int (x)):
			
			dnuca.append (data_chro['execution_decompression'][key]['NUCA'])
			dzip.append (data_chro['execution_decompression'][key]['Zip'])
			dbzip2.append (data_chro['execution_decompression'][key]['Bzip2'])
			
		save_graphics (dnuca, dzip, dbzip2, numpy.arange (0, 60, 2), chro + '_execution_decompression.png', 1 + len (dnuca), data_in['len'], 'SECONDS', 'LENGTH')
		
		del dnuca[:]
		del dzip[:]
		del dbzip2[:]
		
	del dnuca[:]
	del dzip[:]
	del dbzip2[:]
	
	for key, value in count.iteritems():
		dnuca.append (sum_compr['NUCA'][key] / float (value))
		dzip.append (sum_compr['Zip'][key] / float (value))
		dbzip2.append (sum_compr['Bzip2'][key] / float (value))
		
	save_graphics (dnuca, dzip, dbzip2, numpy.arange (0, 0.90, 0.03), 'average_compression.png', len (dnuca) + 1, data_in['len'], 'PERCENT', 'LENGTH')
