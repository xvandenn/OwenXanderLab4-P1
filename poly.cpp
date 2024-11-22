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
		if(begin->first > degree && begin->second != 0)
			degree = begin->first;
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
	degree = 0;
	for(auto pair: other.p)
	{
		p[pair.first] = pair.second;
		if(pair.first > degree)
			degree = pair.first;
	}
	
	return *this;
}


polynomial polynomial::operator+(const polynomial& other) const
{
	auto begin1 = p.begin();
	auto end1 = p.end();

	auto begin2 = other.p.begin();
	auto end2 = other.p.end();

	polynomial sum;
	while(begin1 != end1 && begin2 != end2)
	{
		if(begin1->first == begin2->first)
		{
			if(begin1->second != -begin2->second)
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

	while(begin1 != end1)
	{
		sum.p[begin1->first] = begin1->second;
		begin1++;
	}
	while(begin2 != end2)
	{
		sum.p[begin2->first] = begin2->second;
		begin2++;
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
	return polynomial(product);
}

polynomial polynomial::operator*(const int i) const
{
	polynomial product;
	product = product + i;
	product = product * *this;
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
	return polynomial(r);
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


polynomial operator*(int i, const polynomial& other)
{
	return other * i;
}