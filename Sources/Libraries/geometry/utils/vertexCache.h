#pragma once

template<int size>
class VertexCache
{
public:
	VertexCache(){ clear();	}

	int at(int index) {	return entries[index]; }
	void set(int index, int value) { entries[index] = value; }

	void clear()
	{
		for(int i = 0; i < numEntries; i++)
			entries[i] = -1;
	}

	void copy(VertexCache* inVcache)
	{
		for(int i = 0; i < numEntries; i++)
			inVcache->Set(i, entries[i]);
	}

	bool inCache(int entry)
	{
		for(int i = 0; i < numEntries; i++)
		{
			if(entries[i] == entry)
				return true;
		}

		return false;
	}


	int addEntry(int entry)
	{
		int removed;

		removed = entries[numEntries - 1];

		//push everything right one
		for(int i = numEntries - 2; i >= 0; i--)
		{
			entries[i + 1] = entries[i];
		}

		entries[0] = entry;

		return removed;
	}
private:
	int entries[size];
	static const int numEntries = size;
};
