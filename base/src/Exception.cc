#include "Exception.hh"

using namespace JSNS2;

Exception::Exception() 
  : m_comment("")
{
}

Exception::Exception(const std::string& comment)
  : m_comment(comment)
{
}

Exception::~Exception()
{

}

const char* Exception::What() const
{
  return m_comment.c_str();
}
