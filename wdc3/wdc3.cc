#include"db2.h"
#include<exception>
#include<fstream>
#include<iostream>

int main()
try
{	
}
catch(std::exception const&ex)
{
	std::cerr<<ex.what()<<'\n';
	return 1;
}