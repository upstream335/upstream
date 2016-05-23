

#include "tinyxml2.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <string>

using namespace tinyxml2;
using namespace std;

int main( int argc, const char ** argv )
{
    XMLDocument* doc = new XMLDocument();
string command = "wget http://www.cs.csub.edu/~jhargreaves/upstream/highscore.xml";
system ( ( command ).c_str() );
doc->LoadFile( "highscore.xml" );
int errorID = doc->ErrorID();
const char *err = doc->ErrorName();
printf( "Test file loaded. ErrorID=%d , %s\n",  errorID,err );
doc->Print();
doc->SaveFile( "test.xml" );

}
