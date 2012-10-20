/*
fasta_writer.h: Nucleotides Compression Algorithms
    Copyright (C) 2011 Leonardo Boquillon and Daniel Gutson, FuDePAN

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

#ifndef FASTA_WRITER_H
#define FASTA_WRITER_H

#include <string>
#include "biopp-filer/bioppFiler.h"

template<class UpperLayer, class LowerLayer>
class ConvertDataType;

template<class LowerLayer>
class FastaWriter : public LowerLayer
{
private:
    std::string seqString;
    std::string seqName;
    std::string fileName;

public:
    typedef unsigned char DataType;

    FastaWriter()
    {}

    void setFastaFileOut(const std::string&);
    void setSeqName(const std::string&);
    void receiveData(DataType);
    void end();
};

template<class LowerLayer>
inline void FastaWriter<LowerLayer>::setFastaFileOut(const std::string& file)
{
    fileName = file;
}

template<class LowerLayer>
inline void FastaWriter<LowerLayer>::setSeqName(const std::string& name)
{
    seqName = name;
}

template<class LowerLayer>
inline void FastaWriter<LowerLayer>::receiveData(DataType buffer)
{
    seqString += buffer;
    LowerLayer::receiveData(ConvertDataType<FastaWriter<LowerLayer>, LowerLayer>::convert(buffer));
}

template<class LowerLayer>
inline void FastaWriter<LowerLayer>::end()
{
    bioppFiler::FastaSaver<biopp::PseudoNucSequence> fs(fileName);
    fs.saveNextSequence(seqName, biopp::PseudoNucSequence(seqString));
    LowerLayer::end();
}


#endif

