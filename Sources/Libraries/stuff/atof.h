#pragma once

template<typename iterator>
double myatof(iterator s)
{
	double sign(1), val(0);
	int d(0);
	if (s)
	{
		while (((*s<'0' || *s>'9') && *s != '-' && *s != '+' && *s != '.') && *(++s));
	}
	else
	{
		return 0.0;
	}

	sign*=(*s == '-' && ++s) ? -1.0 : ((*s == '+' && ++s), 1.0);
	while ((*s >= '0' && *s <= '9') || (*s == '.' && d < 1))
	{ 
		val += *s == '.' ? (++d, 0.0) : d > 0 ? ((++d, val*=10.0), *s-'0') :(val*=10.0, *s-'0');
		++s;
	}
	double den(1);
	if(d > 0) while(--d) den *= 10.0;
	return (val*sign) / den;
}