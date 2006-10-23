// -*- C++ -*-
/*!
 * @file StringUtil.h
 * @brief String operation utility
 * @date $Date: 2006-10-23 07:41:20 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2003-2005
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: StringUtil.h,v 1.5 2006-10-23 07:41:20 n-ando Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 * Revision 1.4  2006/10/23 07:37:42  n-ando
 * Bug fix for split(). The problems that split() does not return empty
 * string as list of results string.
 * Reference manuals for functions were described.
 *
 * Revision 1.3  2006/10/17 10:11:09  n-ando
 * Some escape/unescape related bugs are fixed.
 * The following some functions were added.
 * - eraseHeadBlank(), eraseTailBlank(), replaceString()
 *
 * Revision 1.2  2006/10/13 03:51:37  n-ando
 * The "escape" and "unescape" functions were added.
 *
 * Revision 1.1  2006/09/20 08:49:08  n-ando
 * The first commit.
 *
 *
 */

#ifndef StringUtil_h
#define StringUtil_h

#include <string>
#include <algorithm>
#include <vector>


/*!
 * @if jp
 * @brief 文字列がエスケープされているか判断する
 *
 * 指定した文字がエスケープされていればtrue、されていなければfalseを返す。
 *
 * @param str エスケープされているかどうか判断する文字を含む文字列
 * @param pos エスケープされているかどうか判断する文字の位置
 * @return 指定した文字がエスケープされていれば true, それ以外は false
 *
 * @else
 * @brief Whether the character is escaped or not
 *
 * This operation returns true if the specified character is escaped, and
 * if the specified character is not escaped, it returns false
 *
 * @param str The string thath includes the character to be investigated.
 * @param pos The position of the character to be investigated.
 * @return true: the character is escaped, false: the character is not escaped.
 *
 * @endif
 */
bool isEscaped(const std::string& str, std::string::size_type pos)
{
  --pos;
  unsigned int i;
  for (i = 0; (pos >= 0) && str[pos] == '\\'; --pos, ++i) ;
  // If the number of \ is odd, delimiter is escaped.
  return (i % 2) == 1;
}


/*!
 * @if jp
 * @brief 文字列をエスケープするためのFanctor
 * @else
 * @brief A fanctor to escape string
 * @endif
 */
struct escape_functor
{
  escape_functor() {};
  void operator()(const char c)
  {
    if      (c == '\t')  str += "\\t";
    else if (c == '\n')  str += "\\n";
    else if (c == '\f')  str += "\\f";
    else if (c == '\r')  str += "\\r";
    //    else if (c == '\"')  str += "\\\"";
    //    else if (c == '\'')  str += "\\\'";
    else str.push_back(c);
  }
  std::string str;
};


/*!
 * @if jp
 * @brief 文字列をエスケープする
 *
 * 次の文字をエスケープシーケンスに変換する。<br>
 * HT -> "\t" <br>
 * LF -> "\n" <br>
 * CR -> "\r" <br>
 * FF -> "\f" <br>
 * シングルクオート、ダブルクオートについてはとくに処理はしない。
 *
 * @else
 *
 * @brief Escape string
 *
 * The following characters are converted. <br>
 * HT -> "\t" <br>
 * LF -> "\n" <br>
 * CR -> "\r" <br>
 * FF -> "\f" <br>
 * Single quote and dobule quote are not processed.
 *
 * @endif
 */
std::string escape(const std::string str)
{
  return for_each(str.begin(), str.end(), escape_functor()).str;
}


/*!
 * @if jp
 * @brief 文字列をアンエスケープためのFanctor
 * @else
 * @brief The functor to unescape string
 * @endif
 */
struct unescape_functor
{
  unescape_functor() : count(0) {};
  void operator()(char c)
  {
    if (c == '\\')
      {
	++count;
	if (!(count % 2))
	  {
	    str.push_back(c);
	  }
      }
    else
      {
	if (count > 0 && (count % 2))
	  {
	    count = 0;
	    if      (c == 't')  str.push_back('\t');
	    else if (c == 'n')  str.push_back('\n');
	    else if (c == 'f')  str.push_back('\f');
	    else if (c == 'r')  str.push_back('\r');
	    else if (c == '\"') str.push_back('\"');
	    else if (c == '\'') str.push_back('\'');
	    else str.push_back(c);
	  }
	else
	  {
	    count = 0;
	    str.push_back(c);
	  }
      }
  } 
  std::string str;
  int count;
};


/*!
 * @if jp
 * @brief 文字列のエスケープを戻す
 *
 * 次のエスケープシーケンスを文字に変換する。<br>
 * "\t" -> HT <br>
 * "\n" -> LF <br>
 * "\r" -> CR <br>
 * "\f" -> FF <br>
 * "\"" -> "  <br>
 * "\'" -> '  <br>
 *
 * @else
 *
 * @brief Unescape string
 *
 * The following characters are converted. <br>
 * "\t" -> HT <br>
 * "\n" -> LF <br>
 * "\r" -> CR <br>
 * "\f" -> FF <br>
 * "\"" -> "  <br>
 * "\'" -> '  <br>
 *
 * @endif
 */
std::string unescape(const std::string str)
{
  return for_each(str.begin(), str.end(), unescape_functor()).str;

}


/*!
 * @if jp
 * @brief 文字列の先頭の空白文字を削除する
 * @else
 * @brief Erase the head blank characters of string
 * @endif
 */
void eraseHeadBlank(std::string& str)
{
  while (str[0] == ' ' || str[0] == '\t') str.erase(0, 1);
}


/*!
 * @if jp
 * @brief 文字列の末尾の空白文字を削除する
 * @else
 * @brief Erase the tail blank characters of string
 * @endif
 */
void eraseTailBlank(std::string& str)
{
  while ((str[str.size() - 1] == ' ' || str[str.size() - 1] == '\t') &&
	 !isEscaped(str, str.size() - 1))
    str.erase(str.size() - 1, 1);
}


/*!
 * @if jp
 * @brief 文字列を置き換える
 * @else
 * @brief Replace string
 * @endif
 */
void replaceString(std::string& str, const std::string from,
		const std::string to)
{
  std::string::size_type pos(0);
  
  while (pos != std::string::npos)
    {
      pos = str.find(from, pos);
      if (pos == std::string::npos) break;
      str.replace(pos, from.size(), to);
      pos += to.size();
    }
}


/*!
 * @if jp
 * @brief 文字列を分割文字で分割する
 * @else
 * @brief Split string by delimiter
 * @endif
 */
std::vector<std::string> split(const std::string& input,
			       const std::string& delimiter)
{
  typedef std::string::size_type size;
  std::vector<std::string> results;
  size delim_size = delimiter.size();
  size found_pos(0), begin_pos(0), pre_pos(0), substr_size(0);
  
  //  if (input.substr(0, delim_size) == delimiter)
  //    begin_pos = pre_pos = delim_size;
  
  while (1)
    {
    REFIND:
      found_pos = input.find(delimiter, begin_pos);
      if (found_pos == std::string::npos) 
	{
	  results.push_back(input.substr(pre_pos));
	  break;
	}
      /*
      if (isEscaped(input, found_pos))
	{
	  begin_pos = found_pos + delim_size;
	  goto REFIND;
	}
      */
      substr_size = found_pos - pre_pos;
      if (substr_size >= 0)
	{
	  std::string substr(input.substr(pre_pos, substr_size));
	  //	  eraseHeadBlank(substr);
	  results.push_back(substr);
	}
      begin_pos = found_pos + delim_size;
      pre_pos   = found_pos + delim_size;
    }
  return results;
}


/*!
 * @if jp
 * @brief 大文字に変換する Fanctor
 * @else
 * @brief A functor to convert to capital letter
 * @endif
 */
struct Toupper
{
  void operator()(char &c)
  {
    c = toupper(c);
  }
};


/*!
 * @if jp
 * @brief 与えられた文字列をbool値に変換する
 * @else
 * @brief Convert given string to bool value
 * @endif
 */
bool toBool(std::string str, std::string yes, std::string no, 
	    bool default_value = true)
{

  
  std::for_each(str.begin(), str.end(), Toupper());
  std::for_each(yes.begin(), yes.end(), Toupper());
  std::for_each(no.begin(),  no.end(),  Toupper());

  if (str.find(yes) != std::string::npos)
    return true;
  else if (!str.find(no) != std::string::npos)
    return false;
  else
    return default_value;
}


/*!
 * @if jp
 * @brief 与えられた文字列が絶対パスかどうかを判断する
 * @else
 * @brief Investigate whether the given string is absolute path or not
 * @endif
 */
bool isAbsolutePath(const std::string& str)
{
  // UNIX absolute path is begun from '/'
  if (str[0] == '/') return true;
  // Windows absolute path is begun from '[a-zA-Z]:\'
  if (isalpha(str[0]) && (str[1] == ':') && str[2] == '\\') return true;
  // Windows network file path is begun from '\\'
  if (str[0] == '\\' && str[1] == '\\') return true;

  return false;
}


/*!
 * @if jp
 * @brief 与えられた文字列がURLかどうかを判断する
 * @else
 * @brief Investigate whether the given string is URL or not
 * @endif
 */
bool isURL(const std::string& str)
{
  typedef std::string::size_type size;
  size pos;
  pos = str.find(":");
  if ((pos != 0) &&
      (pos != std::string::npos) &&
      (str[pos + 1] == '/') &&
      (str[pos + 2] == '/'))
    return true;
  return false;
}


#endif // StringUtil_h
