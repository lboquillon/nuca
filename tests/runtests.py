# FuDePan 2011

from glob import glob
from os import system
from os import path
from os import remove

for i in glob('*_test.cpp'):
	t = i[:-4] + '_compile.cpp'
	h = open (i, 'r')
	w = open (t, 'w')
	w.write (h.read() + '\nint main(int argc, char **argv) { ::testing::InitGoogleTest(&argc, argv); return RUN_ALL_TESTS();}')
	h.close()
	w.close()
	system ('g++ ' + t + ' -o ' + i[:-4] + ' -lgtest -lpthread -lbiopp -DMILI_NAMESPACE')
	remove (t)
	
	if path.exists (i[:-4]):
		system ('./' + i[:-4])
		remove (i[:-4])	
	
	
