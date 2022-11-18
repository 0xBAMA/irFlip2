#include <iostream>
#include <vector>

#include "lodepng.h"

enum channel {
	REDCHANNEL = 0,
	GREENCHANNEL = 1,
	BLUECHANNEL = 2,
	ALPHACHANNEL = 3,
	REDINVERT = 4,
	GREENINVERT = 5,
	BLUEINVERT = 6,
	ALPHAINVERT = 7,
	SATURATEMIN = 8,
	SATURATEMAX = 9
};

unsigned imageWidth, imageHeight;
std::vector < unsigned char > inputImageData;
std::vector < unsigned char > outputImageData;

bool HasEnding ( std::string const &testString, std::string const &suffix ) {
	if ( testString.length() >= suffix.length() ) {
		return ( 0 == testString.compare (testString.length() - suffix.length(), suffix.length(), suffix ) );
	} else {
		return false;
	}
}

bool LoadImage ( std::string filename ) {
	unsigned error = lodepng::decode( inputImageData, imageWidth, imageHeight, filename.c_str() );
	if ( error ) {
		std::cout << "decode error during load(\" " + filename + " \") " << error << ": " << lodepng_error_text( error ) << std::endl;
		return false;
	}
	return true;
}

channel writeSwizzle[ 4 ];
bool TestSwizzle ( std::string specifiedSwizzle ) {
	if ( specifiedSwizzle.length() == 4 ) {
		for ( int i = 0; i < 4; i++ ) {
			switch ( specifiedSwizzle[ i ] ) {
				case 'R': writeSwizzle[ i ] = REDCHANNEL; break;
				case 'r': writeSwizzle[ i ] = REDINVERT; break;
				case 'G': writeSwizzle[ i ] = GREENCHANNEL; break;
				case 'g': writeSwizzle[ i ] = GREENINVERT; break;
				case 'B': writeSwizzle[ i ] = BLUECHANNEL; break;
				case 'b': writeSwizzle[ i ] = BLUEINVERT; break;
				case 'A': writeSwizzle[ i ] = ALPHACHANNEL; break;
				case 'a': writeSwizzle[ i ] = ALPHAINVERT; break;
				case '0': writeSwizzle[ i ] = SATURATEMIN; break;
				case '1': writeSwizzle[ i ] = SATURATEMAX; break;
				default: return false; break;
			}
		}
		return true;
	}
	return false;
}

void ChannelSwapAndWrite ( std::string filename ) {
	outputImageData.resize( inputImageData.size() );

	#define RED   i+0
	#define GREEN i+1
	#define BLUE  i+2
	#define ALPHA i+3

	// perform the specified swizzle
	int imageSize = imageWidth * imageHeight * 4;
	for ( int i = 0; i < imageSize; i += 4 ) {
		for ( int c = 0; c < 4; c++ ) {
			switch ( writeSwizzle[ c ] ) {
				case REDCHANNEL: outputImageData[ i + c ] = inputImageData[ RED ]; break;
				case REDINVERT: outputImageData[ i + c ] = 255 - inputImageData[ RED ]; break;
				case GREENCHANNEL: outputImageData[ i + c ] = inputImageData[ GREEN ]; break;
				case GREENINVERT:  outputImageData[ i + c ] = 255 - inputImageData[ GREEN ]; break;
				case BLUECHANNEL:  outputImageData[ i + c ] = inputImageData[ BLUE ]; break;
				case BLUEINVERT:  outputImageData[ i + c ] = 255 - inputImageData[ BLUE ]; break;
				case ALPHACHANNEL: outputImageData[ i + c ] = inputImageData[ ALPHA ]; break;
				case ALPHAINVERT:  outputImageData[ i + c ] = 255 - inputImageData[ ALPHA ]; break;
				case SATURATEMIN:  outputImageData[ i + c ] = 0; break;
				case SATURATEMAX:  outputImageData[ i + c ] = 255; break;
			}
		}
	}

	// write the swizzled data
	std::cout << "writing image... ";
	unsigned error = lodepng::encode( filename.c_str(), outputImageData, imageWidth, imageHeight );
	if ( error ) {
		std::cout << "encode error during save(\" " + filename + " \") " << error << ": " << lodepng_error_text( error ) << std::endl;
	} else {
		std::cout << "done." << std::endl;
	}
}

int main ( int argc, char const *argv[] ) {
	// call syntax is < filename > < desired swizzle >

	// swizzle uses primitives R, r, G, g, B, b, A, a, 0, 1
	// upper case is the channel value
	// lower case is 255 - channel value ( e.g. value invert )
	// special values 0, 1, for saturating to 0 and 255 respectively

	// ex:
	//	irFlip in1.png RRrA
	// will do input red, input red, inverted input red, input alpha

	std::string filename = std::string( argv[ 1 ] );
	std::string swizzle  = std::string( argv[ 2 ] );

	if ( HasEnding( filename, std::string( ".png" ) ) ) {
		std::cout << "input is png image" << std::endl;
		if ( TestSwizzle( swizzle ) ) {
			std::cout << "input swizzle " << swizzle << " read" << std::endl;
			if ( LoadImage( filename ) ) {
				std::cout << "image loaded..." << std::endl;
				std::string newFilename = filename.substr( 0, filename.size() - 4 ) + std::string("_") + swizzle + std::string( ".png" );
				ChannelSwapAndWrite( newFilename );
			} else {
				std::cout << "image load failed" << std::endl;
			}
		} else {
			std::cout << "bad swizzle - see README.md for details on usage" << std::endl;
		}
	} else {
		std::cout << "input image is not png" << std::endl;
	}

	return 0;
}
