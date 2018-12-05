#include"db2.h"
#include<exception>
#include<fstream>
#include<iostream>
#include"unknown.h"

int main()
try
{
}
catch(std::exception const&ex)
{
	std::cerr<<ex.what()<<'\n';
	return 1;
}