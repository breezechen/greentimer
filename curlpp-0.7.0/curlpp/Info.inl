/*
 *    Copyright (c) <2002-2006> <Jean-Philippe Barrette-LaPierre>
 *    
 *    Permission is hereby granted, free of charge, to any person obtaining
 *    a copy of this software and associated documentation files 
 *    (cURLpp), to deal in the Software without restriction, 
 *    including without limitation the rights to use, copy, modify, merge,
 *    publish, distribute, sublicense, and/or sell copies of the Software,
 *    and to permit persons to whom the Software is furnished to do so, 
 *    subject to the following conditions:
 *    
 *    The above copyright notice and this permission notice shall be included
 *    in all copies or substantial portions of the Software.
 *    
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 *    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
 *    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 *    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef CURLPP_INFO_INL
#define CURLPP_INFO_INL

template< CURLINFO info, typename T >
void
cURLpp::Info< info, T >::get(cURLpp::Easy &handle, T &value)
{
   cURLpp::InfoTypeConverter< T >::get(handle, info, value);
}

template< CURLINFO info, typename T >
T
cURLpp::Info< info, T >::get(cURLpp::Easy &handle)
{
  T value;
  cURLpp::InfoTypeConverter< T >::get(handle, info, value);
  return value;
}

template< CURLINFO info, typename T >
void
cURLpp::NotAvailableInfo< info, T >::get(cURLpp::Easy &, T &)
{
  throw cURLpp::NotAvailable();
}

template< CURLINFO info, typename T >
T
cURLpp::NotAvailableInfo< info, T >::get(cURLpp::Easy &)
{
  throw cURLpp::NotAvailable();
}



template< typename T >
void
cURLpp::InfoGetter::get(cURLpp::Easy &handle, 
	                CURLINFO info,
 			T &value)
{
   handle.getInfo(info, value);
}


template< typename T >
void
cURLpp::InfoTypeConverter< T >::get(cURLpp::Easy &handle, 
				    CURLINFO info,
				    T &value)
{
  InfoGetter::get(handle, info, value);
}


#endif
