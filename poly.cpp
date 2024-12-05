#include "poly.h"
#include <thread>
#include <mutex>

using namespace std;


polynomial::polynomial()
{
	p[0] = 0;
}


polynomial::polynomial(const polynomial &other)
{
	p = std::unordered_map(other.p);
	if(p.size() == 0)
		p[0] = 0;
	degree = other.degree;
}


void polynomial::print() const
{
	auto iter = p.begin();
	while(iter != p.end())
	{
		if(iter->second != 0)
			std::cout<<iter->second<<"X^"<<iter->first<<" + ";
		iter++;
	}
}


polynomial& polynomial::operator=(const polynomial& other)
{
	p = std::unordered_map(other.p);
	if(p.size() == 0)
		p[0] = 0;
	degree = other.degree;
	return *this;
}


polynomial polynomial::operator+(const polynomial& other) const
{
	auto begin = other.p.begin();
	auto end = other.p.end();

	polynomial sum(*this);

	while(begin != end)
	{
		if(sum.p.find(begin->first) != sum.p.end())
		{
			if(sum.p[begin->first] == -begin->second)
				sum.p.erase(begin->first);
			else
				sum.p[begin->first] += begin->second;
		}
		else
			sum.p[begin->first] = begin->second;
		begin++;
	}
	if (other.degree > sum.degree)
		sum.degree = other.degree;

	if(sum.p.empty() || sum.p.find(sum.degree) == sum.p.end() || sum.p[sum.degree] == 0)
	{
		sum.degree = 0;
		for(auto iter:sum.p)
		{
			if(iter.first > sum.degree)
				sum.degree = iter.first;
		}
	}
	return sum;
}


polynomial polynomial::operator+(const int i) const
{
	polynomial sum(*this);
	if(sum.p[0])
		sum.p[0] += i;
	else
		sum.p[0] = i;

	return sum;
}

polynomial polynomial::operator*(const polynomial& other) const
{
	std::vector<std::thread> threads;
	polynomial product;
	std::mutex mutex;

	auto iter = p.begin();
	int threadSize = (p.size() + 999) / 1000;

	//testing out lambda
	auto mux = [&/*using ref instead of copy this time*/](auto start, auto end)
	{
		//trying with product local to thread so more computation is done before mutex is locked
		polynomial threadProduct;

		auto tempIter = start;
		while(tempIter != end)
		{
			polynomial temp;
			for(auto term: other.p)
			{
				temp.p[term.first + tempIter->first] = term.second * tempIter->second;
			}
			threadProduct = threadProduct + temp;
			tempIter++;
		}

		mutex.lock();
		product = product + threadProduct;
		mutex.unlock();
	};

	for(int i = 0; i < 1000; i++)
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
		if(thread.joinable())
			thread.join();
	}

	return product;
}

polynomial polynomial::operator*(const int i) const
{
	polynomial product;
	product = product + i;
	product = (product * *this);
	return polynomial(product);
}


polynomial polynomial::operator%(const polynomial& other) const
{
	if(other.degree == 0 && other.p.at(0) == 0)
		return polynomial(*this);
	
	polynomial r(*this);
	polynomial t;

	while(r.degree != 0 && r.p.at(0) != 0 && r.degree >= other.degree)
	{
		t.p[r.degree - other.degree] = r.p[r.degree] / other.p.at(other.degree);
		t.degree = r.degree - other.degree;
		t = t * -1;
		r = (r + (t * other));
		t.p.clear();
	}
	r.degree = r.find_degree_of();
	return polynomial(r);
}


size_t polynomial::find_degree_of() const
{
	return degree;
}


std::vector<std::pair<power, coeff>> polynomial::canonical_form() const
{
	std::vector<std::pair<power, coeff>> canon(p.begin(), p.end());
	std::sort(canon.begin(), canon.end());
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