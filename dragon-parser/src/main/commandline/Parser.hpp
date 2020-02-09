#ifndef PARSER 
#define PARSER

#include <vector> 
#include <stdio.h>
#include <map>
#include <algorithm>
#include <optional>
#include <string>
#include <iostream>
#include <fstream>

#include "TokenList.hpp"
#include "../parsing/Namespace.hpp"
#include "../parsing/Method.hpp"
#include "../parsing/DeclaredArg.hpp"
#include "../parsing/statements/Stmt.hpp"

using namespace std;

const string FLAG_DEBUG = "-debug";
const string FLAG_HELP = "-help";
const string FLAG_TEST = "-test";

int main(int argc, char** argv);

void build_ast_file(string tokensFile, string astJsonFile, bool debug);


void main_inner(string tokensFile, bool debug);

TokenList makeTokenListByCallingLexer 	(string file, bool debug);
TokenList readTokensFromTokensFile 		(string tokensFile, bool debug);
TokenList makeTokenList 				(string code,bool debug);

void printHelp();


#endif