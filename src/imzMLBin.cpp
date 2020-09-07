/*************************************************************************
 *     rMSI - R package for MSI data processing
 *     Copyright (C) 2019 Pere Rafols Soler
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **************************************************************************/

#include "imzMLBin.h"
#include <stdexcept>
#include <Rcpp.h>

//#define __DEBUG__
#define MAX_MEMORY_MB 250 //Maxim usable memory

ImzMLBin::ImzMLBin(const char* ibd_fname,  unsigned int num_of_pixels,Rcpp::String Str_mzType, Rcpp::String Str_intType, bool continuous):
  Npixels(num_of_pixels), bContinuous(continuous)
{
  
  mzDataType = string2imzMLDatatype(Str_mzType);
  intDataType = string2imzMLDatatype(Str_intType);
  
#ifdef __DEBUG__
  Rcpp::Rcout << "ImzMLBin::ImzMLBin() entring... with " << Npixels << " num. of pixels...";
#endif
  
  switch(mzDataType)
  {
    case int32: 
    case float32:  
      mzDataPointBytes = 4;
      break;
      
    case int64:
    case float64:
      mzDataPointBytes = 8;
      break;

    default:
      throw std::runtime_error("ERROR: ImzMLBin class constructor: invalid mzType ");
      break;
  }
  
  switch(intDataType)
  {
    case int32: 
    case float32:  
      intDataPointBytes = 4;
      break;
      
    case int64:
    case float64:
      intDataPointBytes = 8;
      break;
      
    default:
      throw std::runtime_error("ERROR: ImzMLBin class constructor: invalid intType.\n");
      break;
  }
  
  imzLength = new unsigned int[Npixels];
  lmzOffset  = new unsigned long[Npixels];  
  iintLength = new unsigned int[Npixels];  
  lintOffset = new unsigned long[Npixels];  
    
#ifdef __DEBUG__
Rcpp::Rcout << "ImzMLBin() constructor end successfuly\n";
Rcpp::Rcout << "imzLength addr: " << imzLength << "\n";
Rcpp::Rcout << "lmzOffset addr: " << lmzOffset << "\n";
Rcpp::Rcout << "iintLength addr: " << iintLength << "\n";
Rcpp::Rcout << "lintOffset addr: " << lintOffset << "\n";
#endif
}

ImzMLBin::~ImzMLBin()
{
#ifdef __DEBUG__
  Rcpp::Rcout << "ImzMLBin() destructor start...";
#endif
  if(ibdFile.is_open())
  {
    ibdFile.close();
#ifdef __DEBUG__
    Rcpp::Rcout << "ibdFile closed...\n";
#endif
  }
  
#ifdef __DEBUG__
  Rcpp::Rcout << "imzLength addr: " << imzLength << "\n";
  Rcpp::Rcout << "lmzOffset addr: " << lmzOffset << "\n";
  Rcpp::Rcout << "iintLength addr: " << iintLength << "\n";
  Rcpp::Rcout << "lintOffset addr: " << lintOffset << "\n";
#endif
  
  delete[] imzLength;
  delete[] lmzOffset;
  delete[] iintLength;
  delete[] lintOffset;
  
#ifdef __DEBUG__
  Rcpp::Rcout << "ImzMLBin() destructor end successfuly\n";
#endif
}

bool ImzMLBin::get_continuous()
{
  return bContinuous;
}

unsigned int ImzMLBin::get_mzLength(unsigned int index)
{
  if(index >= Npixels)
  {
    throw std::runtime_error("ERROR: ImzMLBin class get_mzLength(): out of range.\n");
  }
  return imzLength[index];
}

unsigned long ImzMLBin::get_mzOffset(unsigned int index)
{
  if(index >= Npixels)
  {
    throw std::runtime_error("ERROR: ImzMLBin class get_mzOffset(): out of range.\n");
  }
  return lmzOffset[index];
}

unsigned int ImzMLBin::get_intLength(unsigned int index)
{
  if(index >= Npixels)
  {
    throw std::runtime_error("ERROR: ImzMLBin class get_intLength(): out of range.\n");
  }
  return iintLength[index];
}

unsigned long ImzMLBin::get_intOffset(unsigned int index)
{
  if(index >= Npixels)
  {
    throw std::runtime_error("ERROR: ImzMLBin class get_intOffset(): out of range.\n");
  }
  return lintOffset[index];
}

void ImzMLBin::set_mzLength(Rcpp::NumericVector* mzLength_vector)
{
  for(unsigned int i = 0; i < Npixels; i++)
  {
    imzLength[i] = (unsigned int)(*mzLength_vector)[i];
  }
}

void ImzMLBin::set_mzOffset(Rcpp::NumericVector* mzOffset_vector)
{
  for(unsigned int i = 0; i < Npixels; i++)
  {
    lmzOffset[i] = (unsigned long)(*mzOffset_vector)[i];
  }
}

void ImzMLBin::set_intLength(Rcpp::NumericVector* intLength_vector)
{
  for(unsigned int i = 0; i < Npixels; i++)
  {
    iintLength[i] = (unsigned int)(*intLength_vector)[i];
  }
}

void ImzMLBin::set_intOffset(Rcpp::NumericVector* intOffset_vector)
{
  for(unsigned int i = 0; i < Npixels; i++)
  {
    lintOffset[i] = (unsigned long)(*intOffset_vector)[i];
  }
}

unsigned int ImzMLBin::get_mzEncodingBytes()
{
  return mzDataPointBytes;
}

unsigned int ImzMLBin::get_intEncodingBytes()
{
  return intDataPointBytes;
}

ImzMLBin::imzMLDataType ImzMLBin::string2imzMLDatatype(Rcpp::String data_type)
{
  imzMLDataType dataType;
  if( data_type == "float" )
  {
    dataType = imzMLDataType::float32;
  }
  else if( data_type == "double" )
  {
    dataType = imzMLDataType::float64;
  }
  else if( data_type == "int" )
  {
    dataType = imzMLDataType::int32;
  }
  else if( data_type == "long" )
  {
    dataType = imzMLDataType::int64;
  }
  else
  {
    throw std::runtime_error("ERROR: string2imzMLDatatype() invalid imzML datatype.\n");
  }
  
  return dataType;
}

template<typename T> 
void ImzMLBin::covertBytes2Double(char* inBytes, double* outPtr, unsigned int N)
{
  //First, copy the data to an intermediate vector with the desired type
  T* auxBuffer = new T[N];
  memcpy(auxBuffer, inBytes, sizeof(T)*N);
  
  //Finally, move the data to the double pointer
  for(int i = 0; i < N; i++)
  {
    outPtr[i] = (double)auxBuffer[i];
  }
  
  delete[] auxBuffer;
}

ImzMLBinRead::ImzMLBinRead(const char* ibd_fname, unsigned int num_of_pixels, Rcpp::String Str_mzType, Rcpp::String Str_intType, bool continuous):
  ImzMLBin(ibd_fname, num_of_pixels, Str_mzType, Str_intType, continuous)
{
#ifdef __DEBUG__
  Rcpp::Rcout << "ImzMLBinRead() constructor start...\nibdfile is:"<<  ibd_fname << "\n";
#endif
  ibdFile.open(ibd_fname, std::fstream::in | std::ios::binary);
  if(!ibdFile.is_open())
  {
    throw std::runtime_error("ERROR: ImzMLBinRead could not open the imzML ibd file.\n"); 
  }
#ifdef __DEBUG__
  Rcpp::Rcout << "ImzMLBinRead() constructor end successfuly\n";
#endif
}

ImzMLBinRead::~ImzMLBinRead()
{
  //Empty desctructor
}

void ImzMLBinRead::readDataCommon(unsigned long offset, unsigned int N, double* ptr, unsigned int dataPointBytes, imzMLDataType dataType)
{
  unsigned int byteCount = N*dataPointBytes;
  char* buffer = new char [byteCount];
  
  ibdFile.seekg(offset);
  if(ibdFile.eof())
  {
    throw std::runtime_error("ERROR: ImzMLBinRead reached EOF seeking the imzML ibd file.\n"); 
  }
  if(ibdFile.fail() || ibdFile.bad())
  {
    throw std::runtime_error("FATAL ERROR: ImzMLBinRead got fail or bad bit condition seeking the imzML ibd file.\n"); 
  }
  
  ibdFile.read (buffer, byteCount);
  if(ibdFile.eof())
  {
    throw std::runtime_error("ERROR: ImzMLBinRead reached EOF reading the imzML ibd file.\n"); 
  }
  if(ibdFile.fail() || ibdFile.bad())
  {
    throw std::runtime_error("FATAL ERROR: ImzMLBinRead got fail or bad bit condition reading the imzML ibd file.\n"); 
  }
  
  switch(dataType)
  {
  case int32:
    covertBytes2Double<int>(buffer, ptr, N);
    break;
    
  case float32:  
    covertBytes2Double<float>(buffer, ptr, N);
    break;
    
  case int64:
    covertBytes2Double<long>(buffer, ptr, N);
    break;
    
  case float64:
    //covertBytes2Double<double>(buffer, ptr, N); If double there is no need of intermediate conversion
    memcpy(ptr, buffer, sizeof(double)*N);
    break;
  }
  
  delete[] buffer;
}

void ImzMLBinRead::readUUID(char* uuid)
{
  ibdFile.seekg(0);
  if(ibdFile.eof())
  {
    throw std::runtime_error("ERROR: ImzMLBinRead reached EOF seeking the imzML ibd file.\n"); 
  }
  if(ibdFile.fail() || ibdFile.bad())
  {
    throw std::runtime_error("FATAL ERROR: ImzMLBinRead got fail or bad bit condition seeking the imzML ibd file.\n"); 
  }
  
  ibdFile.read (uuid, 16);
  if(ibdFile.eof())
  {
    throw std::runtime_error("ERROR: ImzMLBin reached EOF reading the imzML ibd file.\n"); 
  }
  if(ibdFile.fail() || ibdFile.bad())
  {
    throw std::runtime_error("FATAL ERROR: ImzMLBin got fail or bad bit condition reading the imzML ibd file.\n"); 
  }
}

void ImzMLBinRead::readMzData(unsigned long offset, unsigned int N, double* ptr )
{
  readDataCommon(offset, N, ptr, mzDataPointBytes, mzDataType);
}

void ImzMLBinRead::readIntData(unsigned long offset, unsigned int N, double* ptr )
{
  readDataCommon(offset, N, ptr, intDataPointBytes, intDataType);
}

ImzMLBinWrite::ImzMLBinWrite(const char* ibd_fname,  unsigned int num_of_pixels, Rcpp::String Str_mzType, Rcpp::String Str_intType, bool continuous) :
  ImzMLBin(ibd_fname, num_of_pixels, Str_mzType, Str_intType, continuous)
{
  
  ibdFile.open(ibd_fname, std::fstream::out | std::ios::binary); //TODO do I want ot truncate the file or just modify it?
  if(!ibdFile.is_open())
  {
    throw std::runtime_error("Error: ImzMLBinRead could not open the imzML ibd file.\n");
  }
}

ImzMLBinWrite::~ImzMLBinWrite()
{
  //Empty desctructor
}   

void ImzMLBinWrite::writeMzData(unsigned long offset, unsigned int N, double* ptr )
{
  //TODO checkout fstream doc: http://www.cplusplus.com/reference/fstream/fstream/
  // seek operation is diferent to the ifstream!
    throw std::runtime_error("TODO: Not implemented yet, sorry :(");
}

void ImzMLBinWrite::writeIntData(unsigned long offset, unsigned int N, double* ptr )
{
  //TODO checkout fstream doc: http://www.cplusplus.com/reference/fstream/fstream/
  // seek operation is diferent to the ifstream!
  throw std::runtime_error("TODO: Not implemented yet, sorry :(");
}

//' Cload_imzMLSpectra
//' Load spectra into a Matrix object interpolating to the common mass axis when necessary.
//' @param ibdFname: full path to the ibd file.
//' @param continuous: true if imzML data is in continuous mode
//' @param mass: data common mass axis.
//' @param mz_dataTypeString: mass encoding data type name.
//' @param int_dataTypeString: mass encoding data type name.
//' @param offsets: subset of the run data of the imzML parsed file.
// [[Rcpp::export]]
Rcpp::NumericMatrix Cload_imzMLSpectra(const char* ibdFname,  bool continuous, Rcpp::NumericVector mass,
                                       Rcpp::String mz_dataTypeString, Rcpp::String int_dataTypeString, 
                                       Rcpp::DataFrame offsets)
{
  if( ((offsets.nrows() * mass.length() * sizeof(double))/ (1024 * 1024 )) > MAX_MEMORY_MB )
  {
    throw std::runtime_error("Error in Cload_imzMLSpectra(): loading data required too much memory.");
  }
  
  Rcpp::NumericMatrix m_spc(offsets.nrows(), mass.length());
  double *intData_ptr; //Temporaly place to store intensity data
  
  try
  {
    ImzMLBinRead myReader(ibdFname, offsets.nrow(), mz_dataTypeString, int_dataTypeString, continuous);
    for( int i = 0; i < offsets.nrows(); i++)
    {
      if(continuous)
      {
        intData_ptr = new double[mass.length()];
        
        myReader.readIntData( (unsigned long)((Rcpp::as<Rcpp::NumericVector>(offsets["intOffset"]))[i]), 
                              (unsigned int)((Rcpp::as<Rcpp::NumericVector>(offsets["intLength"]))[i]), 
                              intData_ptr);
        
        for(int j=0; j < mass.length(); j++)
        {
          m_spc(i,j) = intData_ptr[j];
        }
    
        delete[] intData_ptr;
      }
      else
      {
        //TODO read mass and intensity and interpolate to common mass axis to be able to put spectra in the matrix
        throw std::runtime_error("TODO: Not implemented yet, sorry :(");
      }
    }
  }
  catch(std::runtime_error &e)
  {
    delete[] intData_ptr;
    Rcpp::Rcout << "Catch Error: "<< e.what() << "\n";
  }
  
  return m_spc;
}

///DEBUG METHODS////////////////////////////////////////////////////////////////////////

//' Testing the imzMLreader
//' testingimzMLBinRead
//' @param ibdFname: full path to the ibd file.
//' @param NPixels: Total number of pixels in the image.
//' @param N: number of elemetns (or data point to read).
//' @param offset: offset in bytes at which the reading operation is started.
//' @param read_mz: if true m/z data is readed, otherwise intensities are readed.
//' @param continuous: true if imzML data is in continuous mode
// [[Rcpp::export(name=".debug_imzMLBinReader")]]
Rcpp::NumericVector testingimzMLBinRead(const char* ibdFname, unsigned int NPixels, unsigned int N, unsigned long offset, Rcpp::String dataTypeString, bool read_mz, bool continuous)
{
  Rcpp::NumericVector x(N);
  try
  {
    ImzMLBinRead myReader(ibdFname, NPixels, dataTypeString, dataTypeString, continuous);
    if(read_mz)
    {
      myReader.readMzData(offset, N, x.begin());  
    }
    else
    {
      myReader.readIntData(offset, N, x.begin());
    }
    
  }
  catch(std::runtime_error &e)
  {
    Rcpp::Rcout << "Catch Error: "<< e.what() << "\n";
  }

  return x;
}