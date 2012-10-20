/*
fasta_loader.h: Nucleotides Compression Algorithms
    Copyright (C) 2011 Leonardo Boquillon, FuDePAN

    This file is part of Nuca.

    Nuca is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Nuca is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Nuca. If not, see <http://www.gnu.org/licenses/>.

    NOTE: This file is in prototype stage, and is under active development.
*/

#ifndef FASTA_LOADER_H
#define FASTA_LOADER_H

#include <string>

#include "biopp-filer/bioppFiler.h"
#include "biopp/biopp.h"

template<class LowerLayer>
class FastaLoader : public LowerLayer
{
private:
    std::string fileName;
    std::string* sequenceName;
    typedef biopp::PseudoNucSequence Sequence;

public:
    typedef biopp::PseudoNucleotide DataType;

    FastaLoader()
    { }

    void setFastaFile(const std::string&);
    void setSeqNameString(std::string&);
    void run(bool&);
};


template<class LowerLayer>
void FastaLoader<LowerLayer>::setSeqNameString(std::string& seqName)
{
    sequenceName = &seqName;
}


template<class LowerLayer>
void FastaLoader<LowerLayer>::setFastaFile(const std::string& fastaFile)
{
    fileName = fastaFile;
}

template<class LowerLayer>
void FastaLoader<LowerLayer>::run(bool& multipleSequence)
{

    Sequence seq;
    bioppFiler::FastaParser<Sequence> loader(fileName);

    loader.getNextSequence(*sequenceName, seq);

    for (size_t i = 0; i < seq.length(); ++i)
        LowerLayer::receiveData(ConvertDataType<FastaLoader<LowerLayer>, LowerLayer>::convert(seq[i]));

    LowerLayer::receiveData(nuca::EndSeq);

    multipleSequence = loader.getNextSequence(*sequenceName, seq);
}

#endif
