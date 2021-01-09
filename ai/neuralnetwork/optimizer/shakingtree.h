#pragma once
// CD

#include "optimizer.h"
#include <random>

class Shakingtree : public Optimizer
{
public:
	Shakingtree();
	~Shakingtree();

	void minimize();

	void minimizeBasic();

	void minimizeBasicLarger();

	void minimizeComplex();

	void minimizeBasicPerLayer();

	void mapParameters();

private:
	default_random_engine _generator;
	vector<Edge*> _p;
	vector<vector<Edge*>> _p2;
	vector<uint> _p_ids;

	vector<vector<double>> _shift;
	vector<double> _delta_score;
	int _itmod = 10; //state how much test you want to accumulate before accepting the delta score
	double _step = 0.05;

	uint _total_iter = 0;
	uint _nogoodscore_iter = 0;


};

