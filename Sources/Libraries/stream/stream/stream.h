#pragma once

#include <type_traits>

namespace stream
{

//
class istream
{
protected:
	virtual ~istream();

public:

	//��������� ������ � ������, ����� �� ��������� ������ ������ ��� ���������� ������� size()
	//���������� ���������� ������� ����������� � ������������� ������, ������� ����� ����������
	//�� ����������� �������� size, ������ ������ ���� ����������� ��������������
	virtual std::size_t read(void* buf, std::size_t size) = 0;

	virtual void	skip(std::size_t bytes)		   = 0;	// ���������� ���������� ���������� ����
	virtual void	rollback(std::size_t bytes)	   = 0;	// ���������� ����� �� ���������� ���������� ����
	virtual bool	good() const				   = 0; // ����� �� ������ �� ������
	virtual std::size_t size() const			   = 0; // ������ ������ ������(��� ����������� ������ ����� �� �������������� ���������� �������� ������)
	virtual std::size_t left() const			   = 0; // ���������� ���������� ������ � ������

	template<class T>
	istream& operator >>(T& v)
	{
 		static_assert( std::is_trivially_copyable<T>::value, "type must be trivially_copyable");
 		static_assert( std::is_trivially_destructible<T>::value, "type must be trivially_destructible");
		read(&v, sizeof(T));
		return *this;
	}
};



//
class ostream
{
protected:
	virtual ~ostream();

public:

	virtual void write(const void* buf, std::size_t size)	= 0;
	virtual std::size_t size() const						= 0;

	template<class T>
	ostream& operator <<(const T& v)
	{
		//static_assert(std::is_trivially_copyable<T>::value, "type must be trivially_copyable");
		write( &v, sizeof(v) );
		return *this;
	}
}; 

}

class serializer
{
public:
	serializer(stream::ostream& os_) :os(&os_) {}
	serializer(stream::istream& is_) :is(&is_) {}

	template<class Value>
	serializer& operator<<(Value& value)
	{
		if (os)
			*os << value;
		else
			*is >> value;

		return *this;
	}

private:
	serializer() {}

	stream::ostream* os = 0;
	stream::istream* is = 0;
};
