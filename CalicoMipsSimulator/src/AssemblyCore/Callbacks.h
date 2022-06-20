#pragma once
#include "Memory.h"
#include "HelperString.h"

int addCallback(int& rd, int& rt, int& rs, unsigned int shamt);
int subCallback(int& rd, int& rs, int& rt, unsigned int shamt);
int andCallback(int& rd, int& rs, int& rt, unsigned int shamt);
int orCallback(int& rd, int& rs, int& rt, unsigned int shamt);
int mulCallback(int& rd, int& rs, int& rt, unsigned int shamt);
int jrCallback(int& rd, int& rs, int& rt, unsigned int shamt);
int sltCallback(int& rd, int& rs, int& rt, unsigned int shamt);
int sllCallback(int& rd, int& rs, int& rt, unsigned int shamt);
int srlCallback(int& rd, int& rs, int& rt, unsigned int shamt);
int addiCallback(int& rt, int& rs, int& immediate);
int andiCallback(int& rt, int& rs, int& immediate);
int oriCallback(int& rt, int& rs, int& immediate);
int sltiCallback(int& rt, int& rs, int& immediate);
int beqCallback(int& rt, int& rs, int& immediate);
int bneCallback(int& rt, int& rs, int& immediate);
int lwCallback(int& rt, int& rs, int& immediate);
int swCallback(int& rt, int& rs, int& immediate);
int muliCallback(int& rt, int& rs, int& immediate);
int luiCallback(int& rt, int& rs, int& immediate);
int lbCallback(int& rt, int& rs, int& immediate);
int sbCallback(int& rt, int& rs, int& immediate);
int jCallback(int& address);
int jalCallback(int& address);