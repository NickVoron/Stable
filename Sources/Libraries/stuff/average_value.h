#pragma once

struct average_value
{
public:
	inline operator float() const { return average(); }
	inline void add(float val)
	{
		accum += val;
		++cnt;
	}

	inline void reset()
	{
		accum = 0.0f;
		cnt = 0;
	}

	inline int count() const { return cnt; }

private:

	inline float average() const { return (cnt > 0) ? (accum / cnt) : 0.0f; }
	int cnt = 0;
	float accum = 0.0f;
};