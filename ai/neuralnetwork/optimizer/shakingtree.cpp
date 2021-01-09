#include "shakingtree.h"
#include <algorithm>
#include <random>
#include <chrono>



Shakingtree::Shakingtree()
{

}


Shakingtree::~Shakingtree()
{
}



void Shakingtree::minimize()
{
	minimizeBasicLarger();
}

void Shakingtree::minimizeBasic()
{
	//edit: en fait on s'en fou du critère, on a juste a faire random et ça passe oklm
	mapParameters();

	int batch_size = 20;
	int weight_amplitude = 5;
	double s = getScore(TRAIN, batch_size);
	int i = rand() % _p.size();
	double oldp = _p[i]->weight();
	_p[i]->alterWeight(random(-weight_amplitude, weight_amplitude));
	double news = getScore(TRAIN, batch_size);
	if (news > s)
		_p[i]->alterWeight(oldp);
	return;
}

void Shakingtree::minimizeBasicLarger()
{
	//edit: en fait on s'en fou du critère, on a juste a faire random et ça passe oklm
	mapParameters();

	//init
	int batch_size = 100;
	int weight_amplitude = 5;
	int n_new_parameters = 5;// int(0.1 * _p_ids.size());
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(_p_ids.begin(), _p_ids.end(), std::default_random_engine(seed));
	
	//eval the score
	double s = getScore(TRAIN, batch_size);
	//change parameters
	vector<double> old_p;
	for (size_t j = 0; j < n_new_parameters; j++)
	{
		old_p.push_back(_p[_p_ids[j]]->weight());
		_p[_p_ids[j]]->alterWeight(random(-weight_amplitude, weight_amplitude));
	}
	double new_s = getScore(TRAIN, batch_size);
	//cout << "old score:" << s <<  " new score:" << new_s << " apply?" << (new_s < s) << endl;
	if (new_s > s)
		for (size_t j = 0; j < n_new_parameters; j++)
			_p[_p_ids[j]]->alterWeight(old_p[j]);
	return;
}


void Shakingtree::minimizeComplex()
{
	mapParameters();
	

	//PHASE1 : On calcule le score d'avant, on applique les transformations
	size_t EVALSIZE = 100;
	srand(_total_iter);
	float score = getScore(TRAIN, EVALSIZE);

	//double stepadjusted = _step;
	//double niter = static_cast<double>(_nogoodscore_iter);
	//if(_nogoodscore_iter > 0)
	//	stepadjusted = _step * sqrt((niter * (sin(niter / std::_Pi) + 1)));
	std::normal_distribution<double> rnorm(0, _step);

	vector<double> neww;
	neww.resize(_p.size());
	for (size_t i = 0; i < _p.size(); i++)
	{
		neww[i] = rnorm(_generator);

		//apply the delta
		_p[i]->shiftWeight(neww[i]);
	}

	//PHASE2 : On vérifie le différentiel en mémorisant le delta poids
	//evaluate score
	srand(_total_iter);
	float delta_score = getScore(TRAIN, EVALSIZE) - score;

	_delta_score.push_back(delta_score);
	_shift.push_back(neww);

	//PHASE2B : On annule le delta appliqué en phase1
	for (size_t i = 0; i < _p.size(); i++)
	{
		//apply the delta
		_p[i]->resetLastShift();
	}


	//PHASE3 : Si on a une mémoire suffisament grosse, on sait vers où ça doit bouger
	if (_shift.size() == _itmod)
	{

		//apply the delta averaged
		uint gscore = 0;
		for (size_t j = 0; j < _shift.size(); j++)
			if (_delta_score[j] < 0) // || _total_iter % 10 == 0)
			{
				for (size_t i = 0; i < _p.size(); i++)
				{
					float wsum = 0;
					wsum += _shift[j][i];
					_p[i]->shiftWeight(wsum*LEARNING_RATE);
				}
				gscore++;
			}
		
		//are we getting better? if no, change step
		if (gscore == 0)
			_nogoodscore_iter++;
		else
			_nogoodscore_iter = 0;

		//clear shift, delta score and increment iter
		_shift.clear();
		_delta_score.clear();
		_total_iter++;
	}


	return;
}


void Shakingtree::minimizeBasicPerLayer()
{
	mapParameters();

	
	vector<Edge*>& layer = _p2[rand()%_p2.size()];

	for (size_t i = 0; i < 1000; i++)
	{
		float s = getScore(TRAIN, 100);
		float neww = random(-7, 7);
		int i_edge = rand() % layer.size();
		float oldw = layer[i_edge]->weight();
		layer[i_edge]->alterWeight(neww);
		float news = getScore(TRAIN, 100);
		if (news > s)
			layer[i_edge]->shiftWeight(oldw);
	}

	return;
}



void Shakingtree::mapParameters()
{
	if (_p.size() == 0)
	{
		auto& w = _n->getEdges();
		for (size_t i = 0; i < w.size(); i++)
			for (size_t j = 0; j < w[i].size(); j++)
			{
				for (size_t k = 0; k < w[i][j].size(); k++)
					_p.push_back(w[i][j][k]);
				_p2.push_back(move(w[i][j]));
			}
		cout << _p.size() << " mapped parameters" << endl;
	}
	for (int i = 0; i < _p.size(); i++)  _p_ids.push_back(i);
}







