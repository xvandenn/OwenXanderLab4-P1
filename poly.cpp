#include "poly.h"
#include <thread>

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
}


polynomial polynomial::operator+(const polynomial& other) const
{
	auto begin1 = p.begin();
	auto end1 = p.end();

	auto begin2 = other.p.begin();
	auto end2 = other.p.end();

	polynomial sum;

	while(begin1 != end1)
	{
		if(begin1->second != 0)
			sum.p[begin1->first] = begin1->second;
		begin1++;
	}


	return polynomial(sum);
}


polynomial polynomial::operator+(const int i) const
{
	const polynomial& ref = *this;
	polynomial sum = polynomial(ref);
	sum.p[0] += i;
	sum.degree = sum.find_degree_of();
	return polynomial(sum);
}

polynomial polynomial::operator*(const polynomial& other) const
{
	polynomial temp;
	polynomial product;

	for(auto p1:p)
	{
		for(auto p2:other.p)
		{
			temp.p[p1.first + p2.first] = p1.second * p2.second;
			if(temp.degree < p1.first + p2.first)
				temp.degree = p1.first + p2.first;
		}
		product = product + temp;
		temp.p.clear();
	}
	product.degree = product.find_degree_of();
	return polynomial(product);
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
	auto itter = p.end();
	itter--;
	while(itter->second == 0 && itter != p.begin())
	{
		itter--;
	}
	return (size_t)itter->first;
}


std::vector<std::pair<power, coeff>> polynomial::canonical_form() const
{
	std::vector<std::pair<power, coeff>> canon;
	auto iter = p.end();
	iter--;
	while(iter != p.begin())
	{
		canon.push_back(*iter);
		iter--;
	}
	if(iter->second != 0 || canon.size() == 0)
		canon.push_back(*iter);
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