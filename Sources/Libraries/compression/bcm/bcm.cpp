


#ifdef __GNUC__
#define _FILE_OFFSET_BITS 64
#define _fseeki64 fseeko64
#define _ftelli64 ftello64
#endif



#include "divsufsort.h" 

#include "stream/library.include.h"


typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long long ulonglong;

const char magic[]="BCM1";


class Encoder
{
public:
	uint code;
	uint low;
	uint high;

	Encoder()
		: code(0), low(0), high(-1)
	{}

	void Encode(int bit, uint p, stream::ostream& os)
	{
		const uint mid=low+((ulonglong(high-low)*(p<<14))>>32);

		if (bit)
			high=mid;
		else
			low=mid+1;

		while ((low^high)<(1<<24))
		{
			os << (unsigned char)(low >> 24);
			low<<=8;
			high=(high<<8)|255;
		}
	}

	void Flush(stream::ostream& os)
	{
		for (int i=0; i<4; ++i)
		{
			os << (unsigned char)(low >> 24);
			low<<=8;
		}
	}

	void Init(stream::istream& is)
	{
		for (int i = 0; i < 4; ++i)
		{
			unsigned char in; is >> in;
			code = (code << 8) | in;
		}			
	}

	int Decode(uint p, stream::istream& is)
	{
		const uint mid=low+((ulonglong(high-low)*(p<<14))>>32);

		const int bit=(code<=mid);
		if (bit)
			high=mid;
		else
			low=mid+1;

		while ((low^high)<(1<<24))
		{
			unsigned char in; is >> in;
			code=(code<<8)|in;
			low<<=8;
			high=(high<<8)|255;
		}

		return bit;
	}
};

template<int RATE>
class Counter
{
public:
	int p;

	Counter()
		: p(1<<15)
	{}

	void Update(int bit)
	{
		if (bit)
			p+=(p^65535)>>RATE;
		else
			p-=p>>RATE;
	}
};

class CM: public Encoder
{
public:
	Counter<2> counter0[256];
	Counter<4> counter1[256][256];
	Counter<6> counter2[2][256][17];
	int c1;
	int c2;
	int run;

	CM()
		: c1(0), c2(0), run(0)
	{
		for (int i=0; i<2; ++i)
		{
			for (int j=0; j<256; ++j)
			{
				for (int k=0; k<17; ++k)
					counter2[i][j][k].p=(k-(k==16))<<12;
			}
		}
	}

	void Put(int c, stream::ostream& os)
	{
		if (c1==c2)
			++run;
		else
			run=0;
		const int f=(run>2);

		int ctx=1;
		while (ctx<256)
		{
			const int p0=counter0[ctx].p;
			const int p1=counter1[c1][ctx].p;
			const int p2=counter1[c2][ctx].p;
			const int p=(p0+p0+p0+p0+p1+p1+p1+p2)>>3;

			const int idx=p>>12;
			const int x1=counter2[f][ctx][idx].p;
			const int x2=counter2[f][ctx][idx+1].p;
			const int ssep=x1+(((x2-x1)*(p&4095))>>12);

			const int bit=((c&128)!=0);
			c+=c;
			Encoder::Encode(bit, p+ssep+ssep+ssep, os);

			counter0[ctx].Update(bit);
			counter1[c1][ctx].Update(bit);
			counter2[f][ctx][idx].Update(bit);
			counter2[f][ctx][idx+1].Update(bit);

			ctx+=ctx+bit;
		}

		c2=c1;
		c1=byte(ctx);
	}

	int Get(stream::istream& is)
	{
		if (c1==c2)
			++run;
		else
			run=0;
		const int f=(run>2);

		int ctx=1;
		while (ctx<256)
		{
			const int p0=counter0[ctx].p;
			const int p1=counter1[c1][ctx].p;
			const int p2=counter1[c2][ctx].p;
			const int p=(p0+p0+p0+p0+p1+p1+p1+p2)>>3;

			const int idx=p>>12;
			const int x1=counter2[f][ctx][idx].p;
			const int x2=counter2[f][ctx][idx+1].p;
			const int ssep=x1+(((x2-x1)*(p&4095))>>12);

			const int bit=Encoder::Decode(p+ssep+ssep+ssep, is);

			counter0[ctx].Update(bit);
			counter1[c1][ctx].Update(bit);
			counter2[f][ctx][idx].Update(bit);
			counter2[f][ctx][idx+1].Update(bit);

			ctx+=ctx+bit;
		}

		c2=c1;
		return c1=byte(ctx);
	}
};



void compress(int b, stream::istream& is, stream::ostream& os)
{
	CM cm;

	byte* buf;
	buf=(byte*)calloc(b, 5);
	if (!buf)
	{
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}

	os << magic[0] << magic[1] << magic[2] << magic[3];

	int n;
	while ( (n=is.read(buf, b)) > 0 )
	{
		const int p=divbwt(buf, buf, (int*)&buf[b], n);
		if (p<1)
		{
			perror("Divbwt failed");
			exit(1);
		}

		cm.Put(n>>24, os);
		cm.Put(n>>16, os);
		cm.Put(n>>8, os);
		cm.Put(n, os);
		cm.Put(p>>24, os);
		cm.Put(p>>16, os);
		cm.Put(p>>8, os);
		cm.Put(p, os);

		for (int i=0; i<n; ++i)
			cm.Put(buf[i], os);

		if(!is.good())
			break;
	}

	cm.Put(0, os); 
	cm.Put(0, os);
	cm.Put(0, os);
	cm.Put(0, os);

	cm.Flush(os);

}

void decompress(stream::istream& is, stream::ostream& os)
{
	CM cm;

	byte* buf;
	char mgc[4];
	is >> mgc[0] >> mgc[1] >> mgc[2] >> mgc[3];

	if ((mgc[0] !=magic[0])
		||(mgc[1] !=magic[1])
		||(mgc[2] !=magic[2])
		||(mgc[3] !=magic[3]))
	{
		fprintf(stderr, "Not in BCM format\n");
		exit(1);
	}

	cm.Init(is);

	int b=0;

	for (;;)
	{
		const int n= (cm.Get(is)<<24)
					|(cm.Get(is)<<16)
					|(cm.Get(is)<<8)
					|(cm.Get(is));
		if (n==0)
			break;
		if (b==0)
		{
			buf=(byte*)calloc(b=n, 5);
			if (!buf)
			{
				fprintf(stderr, "Out of memory\n");
				exit(1);
			}
		}
		const int p= (cm.Get(is)<<24)
					|(cm.Get(is)<<16)
					|(cm.Get(is)<<8)
					|(cm.Get(is));
		if ((n<1)||(n>b)||(p<1)||(p>n))
		{
			fprintf(stderr, "File corrupted\n");
			exit(1);
		}
		
		int t[257]={0};
		for (int i=0; i<n; ++i)
			++t[(buf[i]=cm.Get(is))+1];
		for (int i=1; i<256; ++i)
			t[i]+=t[i-1];
		int* next=(int*)&buf[b];
		for (int i=0; i<n; ++i)
			next[t[buf[i]]++]=i+(i>=p);

		for (int i=p; i!=0;)
		{
			i=next[i-1];
			os << buf[i - (i >= p)];
		}
	}
}

int impl_bcm_cq(int sourceSize)
{
	return sourceSize * 5;
}

mem::mem_desc load_file(const char* fname)
{
	stream::ifstream<> is(fname);
	mem::mem_desc md;
	mem::allocate(md, is.size());
	is.read(md.data, md.len);

	return md;
}

int impl_bcm_c(const unsigned char* source, unsigned char* dest, int sourceSize)
{	
	int block_size = 20 << 20; 

	stream::dmemstream is; is.write(source, sourceSize);
	stream::dmemstream os;
	compress(block_size, is, os);

	int size = os.size();
	os.read(dest, size);

	return size;
}

int impl_bcm_dc(const unsigned char* source, unsigned char* dest, int compressedSize, int originalSize)
{
	stream::dmemstream is; is.write(source, compressedSize);
	stream::dmemstream os;

	decompress(is, os);
	ENFORCE(originalSize == os.size());

	os.read(dest, originalSize);

	return originalSize;
}