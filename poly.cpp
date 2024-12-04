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
	p = std::list<std::pair<power, coeff>>(other.p);
}

polynomial::polynomial(std::list<std::pair<power, coeff>> _p)
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
	p.pop_front();
}

polynomial& polynomial::operator=(const polynomial& other)
{
	p = std::list<std::pair<power, coeff>>(other.p);
	return *this;
}

polynomial polynomial::operator+(const int i) const
{
	polynomial sum(*this);
	if(p.front().first == 0)
		sum.p.front().second += i;
	
	else(sum.p.push_front(std::pair<power, coeff>(0,i)));

	return sum;
}

polynomial polynomial::operator+(const polynomial& other) const
{
	auto iter1 = p.begin();
	auto iter2 = other.p.begin();

	polynomial sum;
	sum.pop();

	while(iter1 != p.end() && iter2 != other.p.end())
	{
		if(iter1->first < iter2->first)
			sum.insertPair(*(iter1++));
		else if(iter2->first < iter1->first)
			sum.insertPair(*(iter2++));
		else
		{
			sum.insertPair(std::pair<power,coeff>(iter1->first, iter1->second + iter2->second));
			if(sum.p.back().second == 0 && sum.p.back().first != 0)
				sum.p.pop_back();
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
	polynomial product;
	std::mutex mutex;
	product.pop();

	//testing out lambda
	auto mux = [&/*using ref instead of copy this time*/](const std::pair<size_t, int>& multiplicand)
	{
		polynomial temp(other);
		for(auto& term: temp.p)
		{
			term.first += multiplicand.first;
			term.second *= multiplicand.second;
		}
		mutex.lock();
		product = product + temp;
		mutex.unlock();
	};

	for(const auto& multiplicand : p)
	{
		threads.emplace_back(mux, multiplicand);
	}

	for(auto& thread: threads)
	{
		thread.join();
	}

	return product;
}

polynomial polynomial::operator%(const polynomial& other) const
{
	if(other.p.back().first == 0 && other.p.back().second == 0)
		return polynomial(*this);
	
	polynomial r(*this);
	polynomial t;
	t.pop();

	while((r.p.back().second != 0 || r.p.back().first != 0) && r.p.back().first >= other.p.back().first)
	{
		t.insertPair(std::pair<power, coeff>(r.p.back().first - other.p.back().first, -1 * (r.p.back().second / other.p.back().second)));
		r = (r + (t * other));
		t.pop();
	}
	return r;
}


size_t polynomial::find_degree_of() const
{
	return p.begin()->first;
}


std::vector<std::pair<power, coeff>> polynomial::canonical_form() const
{
	std::vector<std::pair<power,coeff>> canon;
	auto iter = p.end();
	while (iter != p.begin())
	{
		iter--;
		canon.push_back(*iter);
	}
	if(canon.back().second == 0)
		canon.pop_back();
	if(canon.size() == 0)
		canon.push_back(std::pair<power, coeff>(0,0));
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