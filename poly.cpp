#include "poly.h"


polynomial::polynomial()
{
	p[0] = 0;
}


polynomial::polynomial(const polynomial &other)
{
	auto begin = other.p.begin();
	if(begin == other.p.end())
		p[0] = 0;
	
	while(begin != other.p.end())
	{
		p[begin->first] = begin->second;
		begin++;
	}
}


void polynomial::print() const
{}


polynomial& polynomial::operator=(const polynomial& other)
{
	return *this;
}


polynomial& polynomial::operator+(const polynomial& other)
{
	return *this;
}


polynomial& polynomial::operator*(const polynomial& other)
{
	return *this;
}


polynomial& polynomial::operator%(const polynomial& other)
{
	return *this;
}


size_t polynomial::find_degree_of()
{
	return 0;
}


std::vector<std::pair<power, coeff>> polynomial::canonical_form() const
{
	std::vector<std::pair<power, coeff>> canon;
	return canon;
}