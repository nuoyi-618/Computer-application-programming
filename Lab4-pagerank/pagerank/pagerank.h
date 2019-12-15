#pragma once
#ifndef Pagerank_h
#define Pagerank_h
#define MAX 340000
#define URLIDLEN 6
#define ELL_COLUMN 0
#define COO_LENGTH 3100000
#define TOPNUM 10

struct dict
{
	char word[100];
};
struct Top
{
	int ID;
	float Pagerank;
}top[11];

extern int coo_row[COO_LENGTH];
extern int coo_column[COO_LENGTH];
extern float coo_values[COO_LENGTH];

extern float pagerank[MAX];


void CalPageRank();
void findTop10(struct dict* URLs);
void getNumforCoo(int source, int dest);
void getValueforCoo();
#endif Pagerank_h 