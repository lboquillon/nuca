#include "nuca.h"
#include <iostream>
#include <fstream>

typedef RemoveNs<CompressionEventsCounterLayer<EndLayer> > Compressor;

int main(int argc, char* argv[])
{
	Compressor cmpr;
	std::ifstream is;
	char c;

	is.open (argv[1]);

	while (is.get (c))
		cmpr.receiveData (c);

	cmpr.receiveData (nuca::EndSeq);

	std::cout << (cmpr.getOutputNucNumber() / 4);

	return 0;
}
