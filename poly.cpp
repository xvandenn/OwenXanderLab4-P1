#include "poly.h"
#include <thread>

using namespace std;


polynomial::polynomial()
{
	p.push_back(std::pair<power, coeff>(0, 0));
}


polynomial::polynomial(const polynomial &other)
{
	p = std::list<std::pair<power, coeff>>(other.p);
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


polynomial& polynomial::operator=(const polynomial& other)
{
	p = std::list<std::pair<power, coeff>>(other.p);
	return *this;
}


polynomial polynomial::operator+(const polynomial& other) const
{
	return *this;
}

polynomial polynomial::operator*(const polynomial& other) const
{
	return *this;
}

polynomial polynomial::operator%(const polynomial& other) const
{
	// if(other.degree == 0 && other.p.at(0) == 0)
	// 	return polynomial(*this);
	
	// polynomial r(*this);
	// polynomial t;

	// while(r.degree != 0 && r.p.at(0) != 0 && r.degree >= other.degree)
	// {
	// 	t.p[r.degree - other.degree] = r.p[r.degree] / other.p.at(other.degree);
	// 	t.degree = r.degree - other.degree;
	// 	t = t * -1;
	// 	r = (r + (t * other));
	// 	t.p.clear();
	// }
	// r.degree = r.find_degree_of();
	// return polynomial(r);
	return *this;
}


size_t polynomial::find_degree_of() const
{
	return p.begin()->first;
}


std::vector<std::pair<power, coeff>> polynomial::canonical_form() const
{
	std::vector<std::pair<power,coeff>> canon;
	auto iter = p.begin();
	while (iter != p.end())
	{
		canon.push_back(*iter);
		iter++;
	}
	return canon;
}

polynomial operator+(int i, const polynomial& other)
{
	return other;
}


polynomial operator*(int i, const polynomial& other)
{
	return other;
}