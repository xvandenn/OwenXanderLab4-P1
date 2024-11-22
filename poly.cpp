#include "poly.h"
#include <iostream>


polynomial::polynomial()
{
	p[0] = 0;
}


polynomial::polynomial(const polynomial &other)
{
	auto begin = other.p.begin();
	p[0] = 0;
	
	while(begin != other.p.end())
	{
		if(begin->second != 0)
			p[begin->first] = begin->second;
		begin++;
	}
	degree = other.degree;
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
	degree = other.degree;
	for(auto pair: other.p)
		p[pair.first] = pair.second;
	
	return *this;
}


polynomial polynomial::operator+(const polynomial& other)
{
	auto begin1 = p.begin();
	auto end1 = p.end();

	auto begin2 = other.p.begin();
	auto end2 = other.p.end();

	polynomial sum;
	while(begin1 != end1 && begin2 != end2)
	{
		if(begin1->first == begin2->first && begin1->second != -begin2->second)
		{
			sum.p[begin1->first] = begin1->second + begin2->second;
			begin1++;
			begin2++;
		}
		else if(begin1->first > begin2->first)
		{
			sum.p[begin2->first] = begin2->second;
			begin2++;
		}
		else
		{
			sum.p[begin1->first] = begin1->second;
			begin1++;
		}

	}
	return polynomial(sum);
}


polynomial polynomial::operator+(const int i) const
{
	const polynomial& ref = *this;
	polynomial sum = polynomial(ref);
	sum.p[0] += i;
	return polynomial(sum);
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
	return degree;
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