/**
 * \file sparam.hpp
 * Defines some special parameters base on xparam.
 *
 * Copyright 2012 PDNSoft Co. (www.pdnsoft.com)
 * \author hamid jafarian (hamid.jafarian@pdnsoft.com)
 *
 * sparam is part of PParam.
 *
 * PParam is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PParam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PParam.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _PDN_SPARAM_HPP_
#define _PDN_SPARAM_HPP_

#include <openssl/md5.h>

#include <stdio.h>
#include <uuid/uuid.h>
#include <string>
#include <ctime>

#include "xparam.hpp"

namespace pparam
{

/**
 * \class UUIDParam
 * \author Hamid Jafarian(hamid.jafarian@pdnsoft.com)
 * Defines uuid parameter.
 */
class UUIDParam : public XSingleParam
{
public:
	UUIDParam(const string &name) : XSingleParam(name)
	{
		uuid_generate(uuid);
	}
	UUIDParam &operator = (const UUIDParam &);
	XParam &operator = (const string &) throw (Exception);
	XParam &operator = (const XParam &) throw (Exception);
	string value() const;
	void set_value(const string &_uuid)
	{
		*this = _uuid;
	}
	void regenerate()
	{
		uuid_generate(uuid);
	}
private:
	uuid_t uuid;
};

/**
 * \class CryptoParam
 * \author Hamed Haji Hussaini
 * \brief Definea cryptography parameter
 */
class CryptoParam : public XTextParam
{
public:
	CryptoParam(const string &name) : XTextParam(name)
	{
	}
	virtual XParam &operator = (const string &text)
	{
		val = text;
		
		return *this;
	}
	virtual XParam &operator = (const char *text)
	{
		val = text;

		return *this;
	}
	void encrypt_md5()
	{
		*this = md5(get_value());
	}

private:
	string md5(const string &text);
};

/**
 * \class BoolEnum
 * \author Hamid Jafarian(hamid.jafarian@pdnsoft.com)
 * Defines different values for bool parameter.
 */
class Bool
{
public:
	enum {
		YES,
		NO,
		ON,
		OFF,
		ENABLE,
		DISABLE,
		ENABLED,
		DISABLED,
		UP,
		DOWN,
		SET,
		UNSET,
		MAX
	};

	static const string typeString[MAX];
};

/**
 * \class BoolParam
 * \author Hamid Jafarian(hamid.jafarian@pdnsoft.com)
 * Defines a bool parameter.
 */
class BoolParam : public XEnumParam<Bool>
{
public:
	typedef XEnumParam<Bool>	_XEnumParam;

	BoolParam(const string &pname) : XEnumParam<Bool>(pname, Bool::YES)
	{
	}
	bool is_enable() const
	{
		return ! is_disable();
	}
	bool is_disable() const
	{
		return get_value() % 2;
	}
	void yes()
	{
		enable(Bool::YES);
	}
	void no()
	{
		disable(Bool::NO);
	}
	void on()
	{
		enable(Bool::ON);
	}
	void off()
	{
		disable(Bool::OFF);
	}
	void enable(int val = Bool::ENABLE)
	{
		set_value((val + (val % 2)) % Bool::MAX);
	}
	void disable(int val = Bool::DISABLE)
	{
		set_value((val + !(val % 2)) % Bool::MAX);
	}
	void enabled()
	{
		enable(Bool::ENABLED);
	}
	void disabled()
	{
		disable(Bool::DISABLED);
	}
	void up()
	{
		enable(Bool::UP);
	}
	void down()
	{
		disable(Bool::DOWN);
	}
	void set()
	{
		enable(Bool::SET);
	}
	void unset()
	{
		disable(Bool::UNSET);
	}
	virtual BoolParam &operator=(const bool &value);
	virtual BoolParam &operator=(const XInt &value) throw (Exception);
	virtual bool operator==(const bool &value);
	virtual bool operator==(const XInt &value);
};

/**
 * \class DateParam
 * \author Hamed Haji Hussaini(hajihussaini@pdnsoft.com)
 *
 * Defines Date parameter
 */
class DateParam : public XSingleParam
{
public:
	DateParam(const string &name) : XSingleParam(name)
	{
		year = month = day = 0;
	}
	DateParam(const DateParam &date) : XSingleParam(date.get_pname())
	{
		year = date.year;
		month = date.month;
		day = date.day;
	}
	DateParam &operator = (const DateParam &) throw (Exception);
	XParam &operator = (const string &) throw (Exception);
	XParam &operator = (const XParam &) throw (Exception);
	bool operator < (const DateParam &);
	bool operator == (const DateParam &);
	bool operator != (const DateParam &);
	bool operator > (const DateParam &);
	bool operator >= (const DateParam &);
	bool operator <= (const DateParam &);
	DateParam operator + (const DateParam &);
	long operator - (DateParam &);
	unsigned short get_year() const
	{
		return year;
	}
	void set_year(unsigned short _year)
	{
		year = _year;
	}
	unsigned short get_month() const
	{
		return month;
	}
	void set_month(unsigned short _month)
	{
		month = _month;
	}
	unsigned short get_day() const
	{
		return day;
	}
	void set_day(unsigned short _day)
	{
		day = _day;
	}
	unsigned short get_weekday();
	void get_date(unsigned short &,unsigned short &,unsigned short &) const;
	void set_date(unsigned short,unsigned short,unsigned short);
	bool isValid();
	string value() const;
	string formattedValue(const string format) const;
	void now();
	unsigned char daysOfMonth() const;

private:
	bool isLeapYear() const;
	unsigned short daysOfYear() const;
	unsigned long daysOfDate();

private:
	unsigned short	year;
	unsigned short	month;
	unsigned short	day;
};

/**
 * \class TimeParam
 * \author Hamed Haji Hussaini(hajihussaini@pdnsoft.com)
 *
 * Defines Time parameter
 */
class TimeParam : public XSingleParam
{
public:
	TimeParam(const string &name) : XSingleParam(name)
	{
		hour = minute = second = 0;
	}
	TimeParam(const TimeParam &time) : XSingleParam(time.get_pname())
	{
		hour = time.hour;
		minute = time.minute;
		second = time.second;
	}
	TimeParam &operator = (const TimeParam &) throw (Exception);
	XParam &operator = (const string &) throw (Exception);
	XParam &operator = (const XParam &) throw (Exception);
	bool operator < (const TimeParam &);
	bool operator == (const TimeParam &);
	bool operator != (const TimeParam &);
	bool operator > (const TimeParam &);
	bool operator >= (const TimeParam &);
	bool operator <= (const TimeParam &);
	TimeParam operator + (const TimeParam &);
	long operator - (const TimeParam &);
	TimeParam add(const TimeParam &,unsigned char &);
	unsigned short get_hour() const
	{
		return hour;
	}
	void set_hour(unsigned short _hour)
	{
		hour = _hour;
	}
	unsigned short get_minute() const
	{
		return minute;
	}
	void set_minute(unsigned short _minute)
	{
		minute = _minute;
	}
	unsigned int get_second() const
	{
		return second;
	}
	void set_second(unsigned int _second)
	{
		second = _second;
	}
	void get_time(unsigned short &,unsigned short &,unsigned int &) const;
	void set_time(unsigned short,unsigned short,unsigned int);
	bool isValid();
	string value() const;
	string formattedValue(const string format) const;
	void now();

private:
	unsigned long secondsOfTime() const;

private:
	unsigned short	hour;
	unsigned short	minute;
	unsigned int	second;
};

/**
 * \class DateTime
 * \author Hamed Haji Hussaini(hajihussaini@pdnsoft.com)
 * \author Ali Esmaeilpour(esmaeilpur@pdnsoft.com)
 * ]\brief Defines Date & Time parameter
 */
class DateTime: public XSingleParam
{
public:
	DateTime(const string &name) :
		XSingleParam(name), date("date"), time("time")
	{

	}
	DateParam &get_date()
	{
		return date;
	}
	void set_date(DateParam &_date)
	{
		date = _date;
	}
	unsigned short get_year() const
	{
		return date.get_year();
	}
	void set_year(unsigned short year)
	{
		date.set_year(year);
	}
	unsigned short get_month() const
	{
		return date.get_month();
	}
	void set_month(unsigned short month)
	{
		date.set_month(month);
	}
	unsigned short get_day() const
	{
		return date.get_day();
	}
	void set_day(unsigned short day)
	{
		date.set_day(day);
	}
	unsigned short get_weekday()
	{
		return date.get_weekday();
	}
	TimeParam &get_time()
	{
		return time;
	}
	void set_time(TimeParam &_time)
	{
		time = _time;
	}
	unsigned short get_hour() const
	{
		return time.get_hour();
	}
	void set_hour(unsigned short hour)
	{
		time.set_hour(hour);
	}
	unsigned short get_minute() const
	{
		return time.get_minute();
	}
	void set_minute(unsigned short minute)
	{
		time.set_minute(minute);
	}
	unsigned int get_second() const
	{
		return time.get_second();
	}
	void set_second(unsigned int second)
	{
		time.set_second(second);
	}
	XParam &operator = (const string &strdate);
	XParam &operator = (const XParam &idate);
	XParam &operator = (DateTime &idate);
	bool operator < (DateTime &dateTime);
	bool operator > (DateTime &dateTime);
	bool isValid();
	virtual string value() const;
	string formattedValue(const string dateFormat,const string timeFormat,
				const char separator = 'T') const;
	/**
	 * Set this parameter to current date/time.
	 */
	void now();
	DBFieldTypes getDataType() const { return DBDATETIME; }
private:
	DateParam date;
	TimeParam time;
};

/**
 * \class IPType
 * \breif This class enables IPParam to be used in XISetParam
 * \author Hamed Haji Hussaini(hajihussaini@pdnsoft.com)
 *
 * This class creates children of IPParam according with IP or type which
 * is set by children
 */
class IPParam;

class IPType : public XTextParam
{
public:
	enum Version {
		IPv4 = 4,
		IPv6 = 6,
		MAX
	};

	IPType() : XTextParam("ip")
	{
	}
	int get_version()
	{
		return version;
	}
	void set_type(string parentName,Version _version)
	{
		set_pname(parentName);
		version = _version;
	}
	IPParam *newT() throw (Exception);
	virtual XParam &operator = (const XmlNode *node) throw (Exception);
	
private:
	Version	version;
};

/**
 * \class IPParam
 * \brief Peresent an IP address
 * \author Ali Esmaeilpour(esmaeilpur@pdnsoft.com)
 *
 * This class reperesent IP (Internet Protocol) address.
 * currently, its only a base for IPv4Param and IPv6Param classes.
 */
class IPParam: public XSingleParam
{
public:
	typedef	IPType	Type;

	IPParam() : XSingleParam("IPParam")
	{
		address[0] = address[1] = address[2] = address[3] = address[4] =
			address[5] = address[6] = address[7] = 0;
		netmask = 32;
		containNetmask = false;
	}
	/**
	 * Constructor of IPParam class.
	 * \param name [in] specifies name of this Parameter
	 */
	IPParam(const string &name) :
		XSingleParam(name)
	{
		address[0] = address[1] = address[2] = address[3] = address[4] =
			address[5] = address[6] = address[7] = 0;
		netmask = 32;
		containNetmask = false;
	}
	bool key(string &_key)
	{
		_key = value();
		return true;
	}
	virtual void type(Type &_type) const
	{ }
	virtual XParam &operator = (const string &value)
	{ return *this; }
	virtual XParam &operator = (const char *value)
	{ return *this; }
	virtual XParam &operator = (const XParam &param)
	{ return *this; }
	virtual string value() const
	{ return ""; }
	/**
	 * get parts of IP
	 * \param partid [in] part id to retrive
	 */
	int getPart(int partid) const
	{
		return address[partid];
	}
	/**
	 * Get IP Version of this object.
	 */
	virtual IPType::Version getIPVersion() const
	{return IPType::IPv4; }
	string getFamily() const
	{
		switch (getIPVersion()) {
		case IPType::IPv4:
			return "inet";
			break;
		case IPType::IPv6:
			return "inet6";
			break;
		default:
			return "inet";
			break;
		};
	}
	virtual string getBroadcast() const
	{
		return "";
	}
	virtual string getUnicast() const
	{
		return "";
	}
	/**
	 * Set IP address using a string.
	 */
	virtual void set(const string &iIP)
	{ }
	/**
	 * Set IP address of this instance using a XParam object.
	 * IP and Netmask will be copied.
	 */
	virtual void set(const XParam &iIP)
	{ }
	/**
	 * Set IP part of Address using a string.
	 * \param [in] iIP the string that cotains IP address
	 */
	virtual void setAddress(const string &iIP)
	{ }
	/**
	 * get only IP Address part of this address
	 * \return return IP address in a string
	 */
	virtual string getAddress() const
	{ return ""; }
	/**
	 * set only Netmask.
	 * \param [in] iNetmask Netmask of address.
	 */
	virtual void setNetmask(const unsigned int &iNetmask)
	{ }
	/**
	 * set Netmask part of Address using a String.
	 * \param iIP [in] String that reperesent an Netmask
	 */
	virtual void setNetmask(const string &iNetmask)
	{
	}
	/**
	 * get Netmask part
	 * \return returns Netmask, default value if 
	 * nothing assigned to Netmask
	 */
	virtual int getNetmask() const
	{ return 32; }
	/**
	 * Fet Netmask part in string format.
	 * \return returns Netmask, default value if
	 * nothing assigned to Netmask
	 */
	string getNetmaskString() const;
	/**
	 * check if theres Netmask associated with this Address
	 * \return return True if this Address contain Netmask, otherwise false
	 */
	bool haveNetmask() const
	{
		return containNetmask;
	}
	/**
	 * check if the given IP is accessible through this IP
	 * \param IPAddress [in] the IP that will check accessibility for.
	 * \return return true if given IP is accessible through this IP
	 */
	virtual bool checkNetworkAvailability(string IPAddress) const
	{ return false; }
	/**
	 * dtermine input string as IPv4 or IPv6 and
	 * returns corresponding object as an IPParam pointer.
	 */
	static IPParam* getIP(string name, string iIP);
	int get_netmask() const
	{
		return netmask;
	}
protected:
	Type	ipType;
	int address[8]; /**< parts of IP Address */
	int netmask; /**< Netmask part */
	bool containNetmask; /** indicates existence of Netmask */

	/**
	 * split a string using specified character to an array of string
	 * \param str [in] source string to split
	 * \param splitter [in] character that will be used to split 'str'
	 * \param count [out] get number of parts of string produced
	 * \return get resulting array of strings
	 */
	string *split(string str, char splitter, int &count);
	/**
	 * Converts a compact form of netmask( 32bit numeric ) to
	 * simple form ( 0~32 ).
	 * \param [in] exNetmask compact form of netmask (32bit)
	 * \param [out] simpleForm Simple form of netmask (0~32)
	 * \return true if exNetmask was in correct form and
	 * converted successfully, otherwise false
	 */
	bool convertCompactNetmaskToSimple(unsigned int &exNetmask,
		int &simpleForm);
	/**
	 * Converts a simple form ( 0~32 ) to compact form of netmask
	 * ( 32bit numeric ).
	 * \param [in] simpleForm Simple form of netmask (0~32)
	 * \param [out] exNetmask compact form of netmask (32bit)
	 */
	void convertSimpleNetmaskToCompact(const int &simpleForm,
		unsigned int &exNetmask) const;
	/**
	 * validate string to containe only specified characters.
	 */
	bool validateString(const string &str, string &allowed);
	/**
	 * check if any characters of 'charList' exist in str string
	 */
	bool stringContain(string &str, string &charList);
};

/**
 * \brief Reperesent a IPv4Param address
 * \author Ali Esmaeilpour(esmaeilpur@pdnsoft.com)
 * \class IPv4Param
 *
 * This class reperesent 4th version of IP (Internet Protocol) address.
 */
class IPv4Param: public IPParam
{
public:
	/**
	 * Constructor of IPv4Param class.
	 * by default :
	 * IP Address is '0.0.0.0'
	 * and without Netmask ( containNetmask = false )
	 * \param name [in] specifies name of this Parameter
	 */
	IPv4Param(const string &name) :
		IPParam(name)
	{
		ipType.set_type(get_pname(), IPType::IPv4);

		address[0] = address[1] = address[2] = address[3] = 0;
		netmask = 32;
		containNetmask = false;
	}
	virtual void type(Type &_type) const
	{
		_type.set_type(get_pname(), IPType::IPv4);
	}
	/**
	 * Get IP Version of this object.
	 */
	virtual IPType::Version getIPVersion() const
	{
		return IPType::IPv4;
	}
	/**
	 * Set IP address of this instance using another IPv4 object.
	 * IP and Netmask will be copied.
	 */
	IPv4Param &operator =(const IPv4Param &iIP);
	/**
	 * Set IP part of Address using a 32-bit number
	 */
	IPv4Param &operator =(const unsigned int &iIP);
	/**
	 * Set IP address using a string.
	 * input string can be in various formats, for example :
	 * "192.168.0.1"
	 * "3232235521"
	 * "192.168.0.1/24"
	 * "192.168.0.1/255.255.255.0"
	 * "3232235521/4294967040
	 */
	IPv4Param &operator =(const string &iIP);
	IPv4Param &operator =(const char *iIP)
	{
		string	ip;

		ip.assign(iIP);
		*this = ip;

		return *this;
	}
	/**
	 * Set IP address of this instance using a XParam object.
	 * IP and Netmask will be copied.
	 */
	XParam &operator =(const XParam &iIP);
	/**
	 * Set IP address of this instance using another IPv4 object.
	 * IP and Netmask will be copied.
	 */
	void set(const IPv4Param &iIP) throw (Exception);
	/**
	 * Set IP part of Address using a 32-bit number
	 */
	void set(const unsigned int &iIP);
	/**
	 * Set IP address using a string.
	 * input string can be in various formats, for example :
	 * "192.168.0.1"
	 * "3232235521"
	 * "192.168.0.1/24"
	 * "192.168.0.1/255.255.255.0"
	 * "3232235521/4294967040
	 */
	void set(const string &iIP) throw (Exception);
	/**
	 * Set IP address of this instance using a XParam object.
	 * IP and Netmask will be copied.
	 */
	void set(const XParam &iIP) throw (Exception);
	/**
	 * set IP address and netmask.
	 * set IP useing seprated numeric parts and netmask
	 * using its simple form (0~32)
	 * \param [in] part1,part2,part3,part4 parts of IP
	 * \param [in] netmask netmask in simple form
	 */
	void set(int part1, int part2, int part3, int part4, int netmask);
	/**
	 * set IP address and netmask.
	 * set IP and Netmask useing seprated numeric parts.
	 * \param [in] addressPart1,addressPart2,addressPart3,addressPart4
	 * parts of IP
	 * \param [in] netmaskPart1,netmaskPart2,netmaskPart3,netmaskPart4
	 * parts of netmask
	 */
	void set(int addressPart1, int addressPart2, int addressPart3,
		int addressPart4, int netmaskPart1, int netmaskPart2,
		int netmaskPart3, int netmaskPart4);
	/**
	 * set only IP address using its compact form ,32bit numeric.
	 * \param [in] iIP compact form of IP Address
	 */
	void setAddress(const unsigned int &iIP);
	/**
	 * Set IP part of Address using a string.
	 * input string can be in normal 'xxx.xxx.xxx.xxx' format or
	 * a 32 bit number.
	 * \param [in] iIP the string that cotains IP address
	 */
	void setAddress(const string &iIP) throw (Exception);
	/**
	 * set only IP Address using its numeric parts.
	 * \param [in] part1,part2,part3,part4 parts of IP
	 */
	void setAddress(int part1, int part2, int part3, int part4)
		throw (Exception);
	/**
	 * get only IP Address part of this address
	 * \return return IP address in a string
	 */
	string getAddress() const;
	/**
	 * get IP part of Address as a compact 32 bit number
	 * \return get part of Address as a compact 32 bit number
	 */
	unsigned int getAddressCompact() const;
	/**
	 * get parts of IP address as an Array
	 * \param [out] parts A pointer to An array to fill up.
	 */
	void getAddressParts(int *parts);
	/**
	 * set only Netmask.
	 * \param [in] iIP netmask in simple form. in range of 0 - 32 .
	 */
	void setNetmask(const unsigned int &iIP) throw (Exception);
	/**
	 * set Netmask part of Address using a String.
	 * given string can be in one of these forms :
	 * - xxx.xxx.xxx.xxx
	 * - 32 bit numeric
	 * - 0~32
	 * \param iIP [in] String that reperesent an Netmask
	 */
	void setNetmask(const string &iIP) throw (Exception);
	/**
	 * set only Netmask using its numeric parts.
	 * \param [in] part1,part2,part3,part4 parts of Netmask
	 */
	void setNetmask(int part1, int part2, int part3, int part4)
		throw (Exception);
	/**
	 * get Netmask part
	 * \return returns Netmask in simple form (0-32), 32 if theres
	 * nothing assigned to Netmask
	 */
	int getNetmask() const;
	/**
	 * get only Netmask part of this address in compact form (32-bit Number)
	 * \return return Netmask in Compact form as an unsigned integer
	 */
	unsigned int getNetmaskCompact() const;
	/**
	 * get only Netmask part of this address in extended form
	 * ( "xxx.xxx.xxx.xxx" string )
	 * \return return Netmask in a string
	 */
	string getNetmaskExtended();
	/**
	 * get IP address and netmask ( if exist ). in form of
	 * "xxx.xxx.xxx.xxx/xx"
	 * \return returns IP address and Netmask in a string.
	 */
	string value() const;
	/**
	 * check if the given IP is accessible through this IP
	 * \param IPAddress [in] the IP that will check accessibility for.
	 * \return return true if given IP is accessible through this IP
	 */
	bool checkNetworkAvailability(string IPAddress) const;
	/**
	 * check if the given IP is accessible through this IP
	 * \param IPAddress [in] the IP that will check accessibility for.
	 * \return return true if given IP is accessible through this IP
	 */
	bool checkNetworkAvailability(IPv4Param IPAddress) const;

private:

};

/**
 * \brief Reperesent a IPv6Param address
 * \author Ali Esmaeilpour(esmaeilpur@pdnsoft.com)
 * \class IPv6Param
 *
 * This class reperesent 6th version of IP (Internet Protocol) address.
 */
class IPv6Param: public IPParam
{
public:
	/**
	 * Constructor of IPv6Param class.
	 * by default :
	 * IP Address is '::'
	 * and without Netmask ( haveNetmask() = false , 128 by default)
	 * \param name [in] specifies name of this Parameter
	 */
	IPv6Param(const string &name) :
		IPParam(name)
	{
		ipType.set_type(get_pname(), IPType::IPv6);

		address[0] = address[1] = address[2] = address[3] = address[4] =
			address[5] = 0;
		netmask = 128;
		containNetmask = false;
	}
	virtual void type(Type &_type) const
	{
		_type.set_type(get_pname(), IPType::IPv6);
	}
	/**
	 * Get IP Version of this object.
	 */
	virtual IPType::Version getIPVersion() const
	{
		return IPType::IPv6;
	}
	/**
	 * Set IP address of this instance using another IPv6 object.
	 * IP and Netmask will be copied.
	 */
	IPv6Param &operator =(const IPv6Param &iIP);
	/**
	 * Set IP address of this instance using an IPv4 object
	 * by Stateless IP/ICMP Translation method.
	 * IP and Netmask will be copied.
	 */
	IPv6Param &operator =(const IPv4Param &iIP);
	/**
	 * Set IP address using a string.
	 * input string must be in standard format, for example :
	 * "2001:0db8:0000:0000:0000:ff00:0042:8329"
	 * "2001:db8::ff00:42:8329"
	 * "::1"
	 * "::/128"
	 * "fe80::a0::502/64"
	 * or a valid IPv4 string
	 */
	IPv6Param &operator =(const string &iIP);
	IPv6Param &operator =(const char *iIP)
	{
		string	ip;

		ip.assign(iIP);
		*this = ip;

		return *this;
	}
	/**
	 * Set IP address of this instance using a XParam object.
	 * IP and Netmask will be copied.
	 */
	XParam &operator =(const XParam &iIP);
	/**
	 * Set IP address of this instance using another IPv6 object.
	 * IP and Netmask will be copied.
	 */
	void set(const IPv6Param &iIP) throw (Exception);
	/**
	 * Set IP address of this instance using an IPv4 object
	 * by Stateless IP/ICMP Translation method.
	 * IP and Netmask will be copied.
	 */
	void set(const IPv4Param &iIP) throw (Exception);
	/**
	 * Set IP address using a string.
	 * input string must be in standard format, for example :
	 * "2001:0db8:0000:0000:0000:ff00:0042:8329"
	 * "2001:db8::ff00:42:8329"
	 * "::1"
	 * "::/128"
	 * "fe80::a0::502/64"
	 * or a valid IPv4 string
	 */
	void set(const string &iIP) throw (Exception);
	/**
	 * Set IP address of this instance using a XParam object.
	 * name, IP and Netmask will be copied.
	 */
	void set(const XParam &iIP) throw (Exception);
	/**
	 * set IP address and netmask.
	 * set IP useing seprated numeric parts and netmask
	 * \param [in] part1,part2,part3,part4,part5,part6 parts of IP
	 * \param [in] netmask netmask of address
	 */
	void set(int part1, int part2, int part3, int part4, int part5,
		int part6, int part7, int part8, int netmask);
	/**
	 * Set IP part of Address using a string.
	 * input string can be in standard format of IPv6
	 * \param [in] iIP the string that cotains IP address
	 */
	void setAddress(const string &iIP) throw (Exception);
	/**
	 * set only IP Address using its numeric parts.
	 * \param [in] part1,part2,part3,part4,part5,part6 parts of IP
	 */
	void setAddress(int part1, int part2, int part3, int part4, int part5,
		int part6, int part7, int part8) throw (Exception);
	/**
	 * get only IP Address part of this address
	 * \return return IP address in a string
	 */
	string getAddress() const;
	/**
	 * get only IP Address part of this address, in standard 8 part mode
	 * \return return IP address in a string
	 */
	string getAddressComplete() const;
	/**
	 * get parts of IP address as an Array
	 * \param [out] parts A pointer to An array to fill up.
	 */
	void getAddressParts(int *parts);
	/**
	 * set only Netmask.
	 * \param [in] iNetmask Netmask of address. in range of 0 - 128 .
	 */
	void setNetmask(const unsigned int &iNetmask) throw (Exception);
	/**
	 * set Netmask part of Address using a String.
	 * given string must be a number between 0 and 128
	 * \param iIP [in] String that reperesent an Netmask
	 */
	void setNetmask(const string &iNetmask) throw (Exception);
	/**
	 * get Netmask part
	 * \return returns Netmask as a number (0-128), 128 if theres
	 * nothing assigned to Netmask
	 */
	int getNetmask() const;
	/**
	 * get IP address and netmask ( if exist ).
	 * \return returns IP address and Netmask in a string.
	 */
	string value() const;
	/**
	 * check if the given IP is accessible through this IP
	 * \param IPAddress [in] the IP that will check accessibility for.
	 * \return return true if given IP is accessible through this IP
	 */
	bool checkNetworkAvailability(string IPAddress) const;
	/**
	 * check if the given IP is accessible through this IP
	 * \param IPAddress [in] the IP that will check accessibility for.
	 * \return return true if given IP is accessible through this IP
	 */
	bool checkNetworkAvailability(IPv6Param IPAddress) const;
};

/**
 * \class IPxParam
 * \brief One class for both IPv4 and IPv6
 * \author Hamed Haji Hussaini(hajihussaini@pdnsoft.com)
 *
 * This class defines a structure to include both IPv4 and IPv6 classes
 */
class IPxParam : public IPParam
{
public:
	IPxParam(const string &name = "ipx"): IPParam(name)
	{
		version = IPType::MAX;
		ipv4 = NULL;
		ipv6 = NULL;
	}
	/**
	 * Set IP address of this instance using another IPx object
	 */
	virtual IPxParam &operator = (const IPxParam &ip);
	virtual XParam &operator = (const string &ip);
	virtual XParam &operator = (const XParam &parameter);
	virtual string value() const;
	/**
	 * get only IP Address part of this address
	 * \return return IP address in a string
	 */
	virtual string getAddress() const;
	virtual IPType::Version getIPVersion();
	virtual string getBroadcast() const;
	virtual string getUnicast() const;
	virtual void set(const string &iIP);
	virtual void set(const XParam &iIP);
	virtual void setAddress(const string &iIP);
	virtual void setNetmask(const unsigned int &iNetmask);
	virtual void setNetmask(const string &iNetmask);
	virtual int getNetmask() const;
	virtual string getNetmaskString() const;
	virtual bool checkNetworkAvailability(string IPAddress) const;
	~IPxParam()
	{
		if (ipv4)
			delete ipv4;
		if (ipv6)
			delete ipv6;
	}

private:
	IPType::Version	version;
	IPv4Param	*ipv4;
	IPv6Param	*ipv6;
};

/**
 * \class IPxRangeParam
 * Defines range of ip addresses.
 */
class IPxRangeParam : public XMixParam
{
public:
	IPxRangeParam(const string &pname = "ipx_range") : XMixParam(pname),
				from("from"), to("to"), _not("not")
	{
		addParam(&from);
		addParam(&to);
		addParam(&_not);

		_not.no();
	}
	bool is_not()
	{
		return _not.is_enable();
	}
	void set_not()
	{
		_not.yes();
	}
	void unset_not()
	{
		_not.no();
	}
	bool has_from()
	{
		return ! from.value().empty();
	}
	bool has_to()
	{
		return ! to.value().empty();
	}
	string getFrom()
	{
		return from.value();
	}
	string getTo()
	{
		return to.value();
	}
	IPxParam &get_from()
	{
		return from;
	}
	IPxParam &get_to()
	{
		return to;
	}
	bool key(string &_key)
	{
		_key = value();
		return true;
	}
	string get_key()
	{
		return value();
	}
	virtual string value()
	{
		string val = (is_not()) ? "!" : "";
		val += from.value() + ":" + to.value();
		return val;
	}
	virtual IPType::Version getIPVersion()
	{
		return from.getIPVersion();
	}
	virtual string getAddressFrom() const
	{
		return from.getAddress();
	}
	virtual string getAddressTo() const
	{
		return to.getAddress();
	}
	virtual string getBroadcastFrom() const
	{
		return from.getBroadcast();
	}
	virtual string getBroadcastTo() const
	{
		return to.getBroadcast();
	}
	virtual string getUnicastFrom() const
	{
		return from.getUnicast();
	}
	virtual string getUnicastTo() const
	{
		return to.getUnicast();
	}
	virtual void setFrom(const string &iIP)
	{
		from.set(iIP);
	}
	virtual void setTo(const string &iIP)
	{
		to.set(iIP);
	}
	virtual void setFrom(const XParam &iIP)
	{
		from.set(iIP);
	}
	virtual void setTo(const XParam &iIP)
	{
		to.set(iIP);
	}
	virtual void setAddressFrom(const string &iIP)
	{
		from.setAddress(iIP);
	}
	virtual void setAddressTo(const string &iIP)
	{
		to.setAddress(iIP);
	}
	virtual void setNetmaskFrom(const unsigned int &iNetmask)
	{
		from.setNetmask(iNetmask);
	}
	virtual void setNetmaskTo(const unsigned int &iNetmask)
	{
		to.setNetmask(iNetmask);
	}
	virtual void setNetmaskFrom(const string &iNetmask)
	{
		from.setNetmask(iNetmask);
	}
	virtual void setNetmaskTo(const string &iNetmask)
	{
		to.setNetmask(iNetmask);
	}
	virtual int getNetmaskFrom() const
	{
		return from.getNetmask();
	}
	virtual int getNetmaskTo() const
	{
		return to.getNetmask();
	}
	virtual string getNetmaskStringFrom() const
	{
		return from.getNetmaskString();
	}
	virtual string getNetmaskString() const
	{
		return to.getNetmaskString();
	}
protected:
	IPxParam from;
	IPxParam to;
	/**
	 * Means not of this range.
	 */
	BoolParam _not;
};

/**
 * \class IPList
 * \brief Includes list of IPs
 * \author Hamed Haji Hussaini(hajihussaini@pdnsoft.com)
 *
 * Includes list of IPs and gets an IP address to check whether it is available
 * or not
 */
class IPList : public XISetParam<IPParam, string>
{
public:
	IPList(const string &name) : XISetParam<IPParam, string>(name)
	{
		openCharacter = '[';
		closeCharacter = ']';
	}
	void setSurroundedCharacters(const char _open,const char _close)
	{
		openCharacter = _open;
		closeCharacter = _close;
	}
	string value()
	{
		IPParam			*ip;
		string			addresses = "";
		XMixParam::iterator	iterator;

		if (!size())
			return "";
		if (size() == 1) {
			ip = dynamic_cast<IPParam*>(*begin());

			return ip->value();
		}
		addresses += openCharacter;
		for (iterator = begin(); iterator != end(); iterator++) {
			ip = dynamic_cast<IPParam*>(*iterator);
			if (iterator != begin())
				addresses += ',';
			addresses += ip->value();
		}
		addresses += closeCharacter;

		return addresses;
	}
	bool checkNetworkAvailability(const string &ipAddress) const
	{
		const IPParam			*ip;
		XMixParam::const_iterator	iterator;

		for (iterator = const_begin(); iterator != const_end(); 
								iterator++) {
			ip = dynamic_cast<const IPParam*>(*iterator);
			if (ip->checkNetworkAvailability(ipAddress))
				return true;
		}

		return false;
	}

private:
	char	openCharacter;
	char	closeCharacter;
};

/**
 * \class IPxList
 * Defines list of "IPxParam" objects.
 */
class IPxList : public XSetParam<IPxParam, string>
{
public:
	IPxList(const string &name) : XSetParam<IPxParam, string>(name)
	{
		openCharacter = '[';
		closeCharacter = ']';
	}
	void setSurroundedCharacters(const char _open,const char _close)
	{
		openCharacter = _open;
		closeCharacter = _close;
	}
	string value()
	{
		IPxParam		*ipx;
		string			addresses = "";
		XMixParam::iterator	iterator;

		if (!size())
			return "";
		if (size() == 1) {
			ipx = dynamic_cast<IPxParam*>(*begin());

			return ipx->value();
		}
		addresses += openCharacter;
		for (iterator = begin(); iterator != end(); iterator++) {
			ipx = dynamic_cast<IPxParam*>(*iterator);
			if (iterator != begin())
				addresses += ',';
			addresses += ipx->value();
		}
		addresses += closeCharacter;

		return addresses;
	}
	bool checkNetworkAvailability(const string &ipxAddress) const
	{
		const IPxParam			*ipx;
		XMixParam::const_iterator	iterator;

		for (iterator = const_begin(); iterator != const_end(); 
								iterator++) {
			ipx = dynamic_cast<const IPxParam*>(*iterator);
			if (ipx->checkNetworkAvailability(ipxAddress))
				return true;
		}

		return false;
	}

private:
	char	openCharacter;
	char	closeCharacter;
};

/**
 * \class PortParam
 * \breif Defines Port and Port Range
 * \author Hamed Haji Hussaini(hajihussaini@pdnsoft.com)
 *
 * This class includes single port and port range at following formats:
 * [!][[N]:]N OR [!]N[:[N]]
 * Which N is a unsigned interger between 1 to 65536
 */
class PortParam : public XSingleParam
{
public:
	PortParam(const string &name = "port") : XSingleParam(name)
	{
		portRange = false;
		notSign = false;
		from = INVALID_PORT;
		from = to = INVALID_PORT;
		portString = "";
	}
	PortParam(const PortParam &port) : XSingleParam(port.get_pname())
	{
		notSign = port.notSign;
		from = port.from;
		to = port.to;
		portString = port.portString;
	}
	bool key(string &_key)
	{
		_key = portString;

		return true;
	}
	PortParam &operator = (const PortParam &) throw (Exception);
	XParam &operator = (const unsigned int) throw (Exception);
	XParam &operator = (const string &) throw (Exception);
	XParam &operator = (const XParam &) throw (Exception);
	bool isPortRange()
	{
		return portRange;
	}
	bool haveNOTSign()
	{
		return notSign;
	}
	XInt get_from()
	{
		return from;
	}
	XInt get_to()
	{
		return to;
	}
	string value() const
	{
		return portString;
	}

private:
	enum {
		INVALID_PORT =	-1,
		MIN_PORT =	0,
		MAX_PORT =	65535
	};
	bool	notSign;
	bool	portRange;
	string	portString;
	XInt	from;
	XInt	to;
};

/**
 * \class PortList
 * \brief List of Ports
 * \author Hamed Haji Hussaini(hajihussaini@pdnsoft.com)
 *
 * This class includes list of ports
 */
class PortList : public XSetParam<PortParam,string>
{
public:
	PortList(const string &name,const string &portName = "port") :
		XSetParam<PortParam,string>(name)
	{
		openCharacter = '[';
		closeCharacter = ']';

		enable_smap();
	}
	PortParam *addPort(const string &_port)
	{
		PortParam	portParam(port.get_pname());

		portParam = _port;

		return addT(portParam);
	}
	void deletePort(const string &_port)
	{
		del(_port);
	}
	string value()
	{
		PortParam		*_port;
		string			portSet = "";
		XMixParam::iterator	iterator;

		if (!size())
			return "";
		if (size() == 1) {
			_port = dynamic_cast<PortParam*>(*begin());

			return _port->value();
		}
		portSet = openCharacter;
		for (iterator = begin(); iterator != end(); iterator++) {
			_port = dynamic_cast<PortParam*>(*iterator);
			if (iterator != params.begin())
				portSet += ",";
			portSet += _port->value();
		}
		portSet += closeCharacter;

		return portSet;
	}
	void setSurroundedCharacters(const char _openCharacter,
				const char _closeCharacter)
	{
		openCharacter = _openCharacter;
		closeCharacter = _closeCharacter;
	}

private:
	char		closeCharacter;
	char		openCharacter;
	PortParam	port;
};

/**
 * \class MACAddressParam
 * Defines a MAC address parameter.
 */
class MACAddressParam : public XTextParam
{
public:
	MACAddressParam(const string &pname = "mac_address") : 
		XTextParam(pname)
	{
		set_value("00:00:00:00:00:00");
	}
	bool key(string &_key)
	{
		_key = value();
		return true;
	}
	string get_key()
	{
		return value();
	}
	virtual XParam &operator = (const string &mac) throw (Exception);
	virtual XParam &operator = (const char *mac) throw (Exception);
protected:
	bool macIsValid(const string &mac);
};

class DBEngineParam;

class DBEngineTypes
{
public:
	enum DBType {
		SQLite,
		MAX
	};
	static const string typeString[MAX];
};

/**
 * \brief 
 * \author Ali Esmaeilpour(esmaeilpur@pdnsoft.com)
 * \class DBEngineType
 *
 *
 */
class DBEngineType : public XMixParam
{
public:
	DBEngineType() : XMixParam("DBEngineType"),
		type("dbtype", DBEngineTypes::MAX)
	{
		addParam(&type);
	}
	void set_type(int x) throw (Exception)
	{
		type.set_value(x);
	}
	int get_type() const
	{
		return type.get_value();
	}
	DBEngineParam *newT() throw (Exception);
	XParam *getTypeParam() { return &type; }
	virtual XParam &operator = (const XmlNode *node) throw (Exception);
protected:
	XEnumParam<DBEngineTypes> type;
};

/**
 * \brief 
 * \author Ali Esmaeilpour(esmaeilpur@pdnsoft.com)
 * \class DBEngineParam
 *
 *
 */
class DBEngineParam: public XMixParam
{
public:
	typedef DBEngineType Type;
	DBEngineParam(const string &pname) :
		XMixParam(pname), connectionstring("connectionstring"), dbe(0)
	{
		addParam(&connectionstring);
		addParam(dbetype.getTypeParam());
	}
	DBEngineParam &operator =(const DBEngineParam &dbe) throw (Exception)
	{
		this->dbe=dbe.dbe;
		dbetype.set_type(dbe.dbetype.get_type());
		connectionstring=dbe.connectionstring;
		return *this;
	}
	virtual XParam &operator =(const string &str)
	{
		setConnectionString(str);
		return *this;
	}
	virtual XParam &operator =(const XParam &xp) throw (Exception)
	{
		const DBEngineParam* xtp =
			dynamic_cast<const DBEngineParam*>(&xp);
		if (xtp == NULL)
			throw Exception("Bad text XParam in assignment !",
				TracePoint("pparam"));

		// check prameter name.
		if (get_pname() != xtp->get_pname())
			throw Exception("Different xparameter names "
				"in assignment !", TracePoint("pparam"));
		this->dbe=xtp->dbe;
		dbetype.set_type(xtp->dbetype.get_type());
		connectionstring=xtp->connectionstring;
		return *this;
	}
	void setConnectionString(string str)
	{
		if (str != connectionstring.value()) {
			connectionstring = str;
			if (dbe!=NULL && dbe->isConnected()) {
				Disconnect();
				Connect();
			}
		}
	}
	string value() const
	{
		return connectionstring.value();
	}
	void set_value(const string &str)
	{
		(*this) = str;
	}
	string get_value() const
	{
		return connectionstring.value();
	}
	virtual XDBEngine *DBEngine()
	{
		return dbe;
	}
	virtual void DBEngine(XDBEngine *xdb)
	{
		dbe = xdb;
	}
	virtual void Connect() throw (Exception)
	{
		if (connectionstring.empty())
			throw Exception("connectionstring is empty !",
				TracePoint("sparam"));
		if (dbe==NULL)
			throw Exception("no database connection !",
							TracePoint("sparam"));
		dbe->connect(connectionstring.value());
	}
	virtual void Disconnect()
	{
		if(dbe!=NULL)
			dbe->disconnect();
	}
	virtual bool isConnected()
	{
		if(dbe!=NULL)
			return dbe->isConnected();
		return false;
	}
	virtual void type(Type &_type) const
	{
	}
protected:
	XTextParam connectionstring;
	XDBEngine *dbe;
	Type dbetype;
};

/**
 * \brief 
 * \author Ali Esmaeilpour(esmaeilpur@pdnsoft.com)
 * \class SQLiteDBEngineParam
 *
 *
 */
class SQLiteDBEngineParam: public DBEngineParam
{
public:
	SQLiteDBEngineParam(const string &pname) :
		DBEngineParam(pname)
	{
		sqlitedb = new SQLiteDBEngine();
		dbe = (XDBEngine*) sqlitedb;
		dbetype.set_type(DBEngineTypes::SQLite);
	}
	using DBEngineParam::operator =;
	SQLiteDBEngine *SqliteDBEngine()
	{
		return sqlitedb;
	}
	void SqliteDBEngine(SQLiteDBEngine *xdb)
	{
		dbe = (XDBEngine*) xdb;
		sqlitedb = xdb;
	}
	XDBEngine *DBEngine()
	{
		return (XDBEngine*)sqlitedb;
	}
	void DBEngine(XDBEngine *xdb) throw (Exception)
	{
		SQLiteDBEngine *sqlitedb = dynamic_cast<SQLiteDBEngine*>(xdb);
		if (sqlitedb == NULL)
			throw Exception(
				"cannot cast XDBEngine to SQLiteDBEngine",
				TracePoint("sparam"));
		SqliteDBEngine(sqlitedb);
	}
	virtual void type(Type &_type) const
	{
		_type.set_type(DBEngineTypes::SQLite);
	}
protected:
	SQLiteDBEngine *sqlitedb;
};

} // namespace pparam
#endif //_PDN_SPARAM_HPP_
