#pragma once
#include <stdio.h>
#include <cstring>
#include <vector>
#include <string.h>
#include <iostream>
using namespace std;

namespace yazi {
namespace serialize {

class DataStream
{
public:
    enum DataType  //定义变量类型
    {
            BOOL = 0,
            CHAR,
            INT32,
            INT64,
            FLOAT,
            DOUBLE,
            STRING,
            VECTOR,
            LIST,
            MAP,
            SET,
            CUSTOM
    };

    DataStream() {}
    ~DataStream() {}

    void show() const;

    void write(const char * data, int len);  //写入函数申明
    void write(bool value);
    void write(char value);
    void write(int32_t value);
    void write(int64_t value);
    void write(float value);
    void write(double value);
    void write(const char * value); //c语言
    void write(const string & value); //c++

    bool read(bool & value);
    bool read(char & value);
    bool read(int32_t & value);
    bool read(int64_t & value);
    bool read(float & value);
    bool read(double & value);
    bool read(string & value);

    DataStream & operator << (bool value);
    DataStream & operator << (char value);
    DataStream & operator << (int32_t value);
    DataStream & operator << (int64_t value);
    DataStream & operator << (float value);
    DataStream & operator << (double value);
    DataStream & operator << (const string & value);
    DataStream & operator << (const char * vualue);

    DataStream & operator >> (bool & value);
    DataStream & operator >> (char & value);
    DataStream & operator >> (int32_t & value);
    DataStream & operator >> (int64_t & value);
    DataStream & operator >> (float & value);
    DataStream & operator >> (double & value);
    DataStream & operator >> (string & value);

private:
    void reserve(int len);

private:
    std::vector<char> m_buf;
    int m_pos=0; //记录解码位置,要初始化；
};
    void DataStream::reserve(int len)
    {
            int size = m_buf.size();
    int cap = m_buf.capacity();
    if (size + len >cap)
    {
        while (size + len > cap )
        {
            if (cap == 0)
            {
                cap = 1;
            }
            else
            {
                cap *= 2;
            }
        }
        m_buf.reserve(cap); //扩容
    }
    }

void DataStream::write(const char * data, int len)
{
    reserve(len);
    int size = m_buf.size();
    m_buf.resize(size + len);
    std::memcpy(&m_buf[size],data,len);

}
void DataStream::write(bool value)
{
    char type = DataType::BOOL;
    write((char *)&type,sizeof(char));
    write((char *)&value,sizeof(char));
}                                      //实现bool类型的编码
void DataStream::write(char value)
{
    char type = DataType::CHAR;
    write((char *)&type,sizeof(char));
    write((char *)&value,sizeof(char));
}
void DataStream::write(int32_t value)
{
    char type = DataType::INT32;
    write((char *)&type, sizeof(char));
    write((char *)&value,sizeof(int32_t));
}
void DataStream::write(int64_t value)
{
    char type = DataType::INT64;
    write((char *)&type, sizeof(char));
    write((char *)&value,sizeof(int64_t));
}
void DataStream::write(float value)
{
    char type = DataType::FLOAT; 
    write((char *)&type, sizeof(char));
    write((char *)&value,sizeof(float));
}
void DataStream::write(double value)
{
    char type = DataType::DOUBLE;
    write((char *)&type, sizeof(char));
    write((char *)&value,sizeof(double));
}
void DataStream::write(const string & value)
{
    char type = DataType::STRING;
    write((char *)&type,sizeof(char));
    int len = value.size();
    write(len);
    write(value.data(),len);
}
void DataStream::write(const char * value)
{
    char type = DataType::STRING;
    write((char *)&type,sizeof(char));
    int len = strlen(value);
    write(len);
    write(value,len);
}

bool DataStream::read(char & value)
{
    if (m_buf[m_pos] != DataType::CHAR)
        {
            return false;
        }
    ++m_pos;
    value = m_buf[m_pos]; 
    ++m_pos;
    return true;   
}
bool DataStream::read(bool & value)
{
    if (m_buf[m_pos] != DataType::BOOL)
    {
        return false;
    }
    ++m_pos;
    value = m_buf[m_pos];
    ++m_pos;
    return true;
}
bool DataStream::read(int32_t & value)
{
    if(m_buf[m_pos] != DataType::INT32)
    {
        return false;
    }
    ++m_pos;
    value = *((int32_t *)(&m_buf[m_pos]));
    m_pos +=4;
    return true;
}
bool DataStream::read(int64_t & value)
{
    if(m_buf[m_pos] != DataType::INT64)
    {
        return false;
    }
    ++m_pos;
    value = *((int64_t *)(&m_buf[m_pos]));
    m_pos +=8;
    return true;
}
bool DataStream::read(float & value)
{
    if(m_buf[m_pos] != DataType::FLOAT)
    {
        return false;
    }
    ++m_pos;
    value = *((float *)(&m_buf[m_pos]));
    m_pos +=4;
    return true;
}
bool DataStream::read(double & value)
{
    if(m_buf[m_pos] != DataType::DOUBLE)
    {
        return false;
    }                     //为什么还要检查是不是其类型？
    ++m_pos;
    value = *((double *)(&m_buf[m_pos]));
    m_pos +=8;
    return true;
}
bool DataStream::read(string & value)
{
    ++m_pos;    
    int len;
    read(len);
    value.assign((char *)&(m_buf[m_pos]),len);
    m_pos += len;
    return true;
}


void DataStream::show() const
{
    int size = m_buf.size();
    std::cout<<"data size = " << size << std::endl;
    int i=0;
    while(i < size)
    {
        switch ((DataType)m_buf[i])
        {
        case DataType::BOOL:
            if ((int)m_buf[++i] == 0) // ++i使m_buf读到下一个位置；
            {
                std::cout<<"false";
            }
            else
            {
                std::cout<<"true";
            }
            i++;  //跳出当前的循环，同时使系统读下一个变量；
            break;
        case DataType::CHAR:
            std::cout << m_buf[++i];
            ++i;
            break;
        case DataType::INT32:
            std::cout << *((int32_t *)(&m_buf[++i]));  //将m_buf转为int32_t的指针，再直接引用这个指针；
            i +=4;
            break;
        case DataType::INT64:
            std::cout << *((int64_t *)(&m_buf[++i]));
            i +=8;
            break;
        case DataType::FLOAT:
            std::cout << *((float *)(&m_buf[++i]));
            i +=4;
            break;
        case DataType::DOUBLE:
            std::cout<< *((double *)(&m_buf[++i]));
            i +=8;
            break;
        case DataType::STRING:
            if ((DataType)m_buf[++i] == DataType::INT32)
            {
                int len = *((int32_t *)(&m_buf[++i]));
                i += 4;
                std::cout << string(&m_buf[i], len);
                i += len;
            }
            else
            {
                throw std::logic_error("parse string error");
            }
            break;
        default:
            break;
        }
    }
    
}

DataStream & DataStream::operator << (bool value)
{
    write(value);
    return *this;
}
DataStream & DataStream::operator << (char value)
{
    write(value);
    return *this;
}
DataStream & DataStream::operator << (int32_t value)
{
    write(value);
    return *this;
}
DataStream & DataStream::operator << (int64_t value)
{
    write(value);
    return *this;
}
DataStream & DataStream::operator << (float value)
{
    write(value);
    return *this;
}
DataStream & DataStream::operator << (double value)
{
    write(value);
    return *this;
}
DataStream & DataStream::operator << (const string & value)
{ 
    write(value);
    return *this;
}
DataStream & DataStream::operator << (const char * value)
{ 
    write(value);
    return *this;
}
DataStream & DataStream::operator >> (bool & value)
{
    read(value);
    return *this;
}
DataStream & DataStream::operator >> (char & value)
{
    read(value);
    return *this;
}
DataStream & DataStream::operator >> (int32_t & value)
{
    read(value);
    return *this;
}
DataStream & DataStream::operator >> (int64_t & value)
{
    read(value);
    return *this;
}
DataStream & DataStream::operator >> (float & value)
{
    read(value);
    return *this;
}
DataStream & DataStream::operator >> (double & value)
{
    read(value);
    return *this;
}
DataStream & DataStream::operator >> (string & value)
{ 
    read(value);
    return *this;
}
}
}



// question :1.扩容和实际存储空间之间的关系；
//           2.