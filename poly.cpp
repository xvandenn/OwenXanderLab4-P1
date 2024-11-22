#include "poly.h"
#include <iostream>


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
		if(begin->second != 0)
			p[begin->first] = begin->second;
		begin++;
	}
}


void polynomial::print() const
{
	auto iter = p.end();
	iter--;

	while(iter != p.begin())
	{
		
		if(iter->second != 0)
			std::cout<<iter->second<<"X^"<<iter->first<<" + ";
		iter--;
	}

	if(iter->second != 0)
	{
		std::cout<<iter->second;
		if(iter->first != 0)
			std::cout<<"X^"<<iter->first;
		std::cout<<std::endl;
	}
	else
		std::cout<<"0"<<std::endl;

}


polynomial& polynomial::operator=(const polynomial& other)
{
	p.clear();
	for(auto pair: other.p)
		p[pair.first] = pair.second;
	
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
	auto iter = p.end();
	iter--;
	while(iter != p.begin())
	{
		canon.push_back(*iter);
		iter--;
	}
	canon.push_back(*iter);
	return canon;
}