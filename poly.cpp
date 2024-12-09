#include "poly.h"
#include <thread>
#include <vector>
#include <mutex>

using namespace std;


polynomial::polynomial()
{
	p.push_back(std::pair<power, coeff>(0, 0));
}


polynomial::polynomial(const polynomial &other)
{
	p = std::vector<std::pair<power, coeff>>(other.p);
}

polynomial::polynomial(std::vector<std::pair<power, coeff>> _p)
{
	p = _p;
}

void polynomial::print() const
{
	auto iter = p.begin();
	while(iter != p.end())
	{
		std::cout<<iter->second<<"X^"<<iter->first<<" + ";
		iter++;
	}
	std::cout<<std::endl;
}

void polynomial::insertPair(std::pair<power, coeff> term)
{
	p.push_back(term);
}

void polynomial::pop()
{
	return;
}

polynomial& polynomial::operator=(const polynomial& other)
{
	p = std::vector<std::pair<power, coeff>>(other.p);
	return *this;
}

polynomial polynomial::operator+(const int i) const
{
	polynomial sum(*this);
	if(p.front().first == 0)
		sum.p.front().second += i;
	
	else(sum.p.emplace(p.begin(), 0, i));

	return sum;
}

polynomial polynomial::operator+(const polynomial& other) const
{
	auto iter1 = p.begin();
	auto iter2 = other.p.begin();

	polynomial sum;
	sum.p.clear();

	while(iter1 != p.end() && iter2 != other.p.end())
	{
		if(iter1->first < iter2->first && iter1->second != 0)
			sum.insertPair(*(iter1++));
		else if(iter2->first < iter1->first && iter2->second != 0)
			sum.insertPair(*(iter2++));
		else
		{
			if(iter1->second + iter2->second != 0)
				sum.insertPair(std::pair<power,coeff>(iter1->first, iter1->second + iter2->second));
			if(sum.p.size() == 0)
				sum.p.emplace_back(0,0);
			iter1++;
			iter2++;
		}
	}

	if(iter1 == p.end())
		sum.p.insert(sum.p.end(), iter2, other.p.end());
	else
		sum.p.insert(sum.p.end(), iter1, p.end());
	return sum;
}

polynomial polynomial::operator*(const int i)const
{
	if(i == 0)
	{
		polynomial product;
		return product;
	}

	polynomial product(*this);
	auto iter = product.p.begin();
	while(iter != product.p.end())
	{
		iter->second = iter->second * i;
		iter ++;
	}
	return product;
}

polynomial polynomial::operator*(const polynomial& other) const
{
	std::vector<std::thread> threads;
	int numThreads = 16;
	polynomial product;
	std::mutex mutex;
	product.p.clear();

	auto iter = p.begin();
	int threadSize = (p.size() + numThreads - 1) / numThreads;

	//testing out lambda
	auto mux = [&/*using ref instead of copy this time*/](auto start, auto end)
	{
		//trying with product local to thread so more computation is done before mutex is locked
		polynomial threadProduct;
		threadProduct.p.clear();

		auto tempIter = start;
		while(tempIter != end)
		{
			polynomial temp(other);
			for(auto& term: temp.p)
			{
				term.first += tempIter->first;
				term.second *= tempIter->second;
			}
			threadProduct = threadProduct + temp;
			tempIter++;
		}

		mutex.lock();
		product = product + threadProduct;
		mutex.unlock();
	};

	for(int i = 0; i <  numThreads; i++)
	{
		auto start = iter;
		int j = 0;
		while(j < threadSize && iter != p.end())
		{
			iter++;
			j++;
		}
		auto end = iter;
		threads.emplace_back(mux, start, end);
		if(iter == p.end())
			break;
	}

	for(auto& thread: threads)
	{
		thread.join();
	}

	return product;
}

polynomial polynomial::operator%(const polynomial& other) const
{

	if(other.find_degree_of() == 0 && other.p.front().second == 0)
		return polynomial(*this);
	
	polynomial r(*this);
	polynomial t;
	t.p.clear();

	while((r.p.back().second != 0) && r.p.back().first >= other.p.back().first)
	{
		t.insertPair(std::pair<power, coeff>(r.p.back().first - other.p.back().first, -1 * (r.p.back().second / other.p.back().second)));
		r = (r + (other * t));
		t.p.clear();
	}
	return r;
}


size_t polynomial::find_degree_of() const
{
	auto iter = p.end();
	while(iter != p.begin())
	{
		iter--;
		if(iter->second != 0)
			return iter->first;
	}
	return 0;
}


std::vector<std::pair<power, coeff>> polynomial::canonical_form() const
{
	std::vector<std::pair<power,coeff>> canon;
	auto iter = p.end();
	while (iter != p.begin())
	{
		iter--;
		if(iter->second != 0)
			canon.push_back(*iter);
	}
	if(canon.size() == 0)
		canon.emplace_back(0,0);
	return canon;
}

polynomial operator+(int i, const polynomial& other)
{
	return other + i;
}


polynomial operator*(int i, const polynomial& other)
{
	return other * i;
}