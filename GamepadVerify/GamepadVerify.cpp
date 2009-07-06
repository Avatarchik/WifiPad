#include <stdio.h>
#include <stdexcept>
#include "NullImage.h"
#include "FileDataSource.h"
#include "GamePad.h"

int main(int argc,char *argv[])
{
	using namespace WifiPad;
	
	if(argc != 2) {
		fprintf(stderr,"Usage: %s <file>\n",argv[0]);
		return -1;
	}
	
	try {
		FileDataSource source(argv[1]);
		GamePad gamepad(source,0);
		return 0;
	} catch(const std::runtime_error& e) {
		printf("%s",e.what());
	}
	return -1;
}
