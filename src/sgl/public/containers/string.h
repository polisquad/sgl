#ifndef SGL_STRING_H
#define SGL_STRING_H

#include "containers/array.h"
#include "hal/platform_string.h"

/**
 * @class String containers/string.h
 * @brief Base string class
 * 
 * A dynamic size string
 */
class String
{
protected:
	/// @brief Data implemented using an array
	Array<ansichar> data;

public:
	/// @brief Default-constructor
	String() = default;

	/**
	 * @brief C-string-constructor
	 * 
	 * We assume the source string to be
	 * null terminated
	 * 
	 * @param [in] source source C-string
	 */
	FORCE_INLINE String(const ansichar source[])
	{
		if (source)
		{
			// Get string length
			const sizet length = strlen(source);

			// Push characters
			data.pushUnsafe(source, length);
		}
	}

	/**
	 * @brief Buffer-constructor
	 * 
	 * @param [in]	source	buffer of characters
	 * @param [in]	size	size of the buffer (i.e. length)
	 */
	FORCE_INLINE String(const ansichar * source, sizet size)
	{
		if (source && size) data.pushUnsafe(source, size);
	}

	/// @brief Returns allocated size
	FORCE_INLINE sizet getSize() const { return data.getSize(); }

	/// @brief Returns length of the string
	/// @{
	FORCE_INLINE sizet getCount() const { return data.getCount(); }
	FORCE_INLINE sizet length() const { return data.getCount(); }
	/// @}

	/// @brief Returns @c true if string is empty
	FORCE_INLINE bool isEmtpy() const { return data.getCount() == 0; }
	
	/// @brief Access element
	/// @{
	FORCE_INLINE ansichar & operator[](uint64 i) { return data[i]; }
	FORCE_INLINE const ansichar operator[](uint64 i) const { return data[i]; }
	/// @}

	/// @brief Iterators
	/// @{
	typedef Array<ansichar>::Iterator Iterator;
	typedef Array<ansichar>::ConstIterator ConstIterator;

	FORCE_INLINE Iterator begin() { return data.begin(); }
	FORCE_INLINE ConstIterator begin() const { return data.begin(); }

	FORCE_INLINE Iterator end() { return data.end(); }
	FORCE_INLINE ConstIterator end() const { return data.end(); }
	/// @}

	/// @brief Returns pointer to the underlying data
	/// @{
	FORCE_INLINE const ansichar * operator*() const { return data.getCount() ? *data : ""; }
	FORCE_INLINE const ansichar * c_str() const { return operator*(); }
	/// @}

	/// @brief Returns underlying array
	/// @{
	FORCE_INLINE Array<ansichar> & getArray() { return data; }
	FORCE_INLINE const Array<ansichar> & getArray() const { return data; }
	/// @}

	/// @brief Comparison operators
	/// @{

	/**
	 * @return	zero if strings are equal,
	 * 			greater than zero if first greater,
	 * 			less than zero otherwise
	 * @{
	 */
	FORCE_INLINE int32 compare(const ansichar * s, sizet n) const { return PlatformString::strcmp(c_str(), s); }
	FORCE_INLINE int32 compare(const ansichar s[]) const { return PlatformString::strcmp(c_str(), s); }
	FORCE_INLINE int32 compare(const String & s) const { return PlatformString::strcmp(c_str(), s.c_str()); }
	FORCE_INLINE friend int32 compare(const ansichar s1[], const String & s2) { return PlatformString::strcmp(s1, *s2); }

	/// @note Case insensitive
	/// @{
	FORCE_INLINE int32 comparei(const ansichar s[], sizet n) const { return PlatformString::strcmpi(c_str(), s); }
	FORCE_INLINE int32 comparei(const ansichar s[]) const { return PlatformString::strcmpi(c_str(), s); }
	FORCE_INLINE int32 comparei(const String & s) const { return PlatformString::strcmpi(c_str(), s.c_str()); }
	FORCE_INLINE friend int32 comparei(const ansichar s1[], const String & s2) { return PlatformString::strcmpi(s1, *s2); }
	/// @}
	/// @}

	/// String-string comparison
	/// @{
	template<typename StringType> FORCE_INLINE bool operator==(const StringType & s) const { return compare(s) == 0; }
	template<typename StringType> FORCE_INLINE bool operator!=(const StringType & s) const { return compare(s) != 0; }
	template<typename StringType> FORCE_INLINE bool operator<(const StringType & s) const { return compare(s) < 0; }
	template<typename StringType> FORCE_INLINE bool operator>(const StringType & s) const { return compare(s) > 0; }
	template<typename StringType> FORCE_INLINE bool operator<=(const StringType & s) const { return compare(s) <= 0; }
	template<typename StringType> FORCE_INLINE bool operator>=(const StringType & s) const { return compare(s) >= 0; }
	/// @{

	/// C-string-String comparison
	/// @{
	FORCE_INLINE friend bool operator==(const ansichar s1[], const String & s2) { return PlatformString::strcmp(s1, *s2) == 0; }
	FORCE_INLINE friend bool operator!=(const ansichar s1[], const String & s2) { return PlatformString::strcmp(s1, *s2) != 0; }
	FORCE_INLINE friend bool operator<(const ansichar s1[], const String & s2) { return PlatformString::strcmp(s1, *s2) < 0; }
	FORCE_INLINE friend bool operator<=(const ansichar s1[], const String & s2) { return PlatformString::strcmp(s1, *s2) <= 0; }
	FORCE_INLINE friend bool operator>(const ansichar s1[], const String & s2) { return PlatformString::strcmp(s1, *s2) > 0; }
	FORCE_INLINE friend bool operator>=(const ansichar s1[], const String & s2) { return PlatformString::strcmp(s1, *s2) >= 0; }
	/// @}

	/// @}

private:
	/// @brief Null-terminate string buffer
	FORCE_INLINE void teminateBuffer()
	{
		data.appendSpace(1);
		data[data.getCount()] = '\0';
	}
	
public:
	/**
	 * @brief Append a single character at the end
	 * 
	 * @param [in] c character
	 * 
	 * @return self
	 * @{
	 */
	FORCE_INLINE String & operator+=(ansichar c)
	{
		// Make space for null character
		data.appendSpace(2);
		data.push(c);

		// The terminating character is not really necessary
		// but we leverage it in string comparison and in
		// C-string conversion
		data[data.getCount()] = '\0';

		return *this;
	}
	FORCE_INLINE String & append(ansichar c) { return operator+=(c); }
	/// @}

	/**
	 * @brief Add characters at the end of the string
	 * 
	 * @param [in]	buffer	buffer of characters
	 * @param [in]	size	size of buffer (i.e. length)
	 * 
	 * @return self
	 */
	FORCE_INLINE String & append(const ansichar * buffer, sizet size)
	{
		if (buffer && size) 
		{
			data.appendSpace(size + 1);
			data.pushUnsafe(buffer, size);

			/// @see append(ansichar)
			data[data.getCount()] = '\0';
		}

		return *this;
	}

	/**
	 * @brief Append other string
	 * 
	 * @param [in] s other string
	 * 
	 * @return self
	 * @{
	 */
	FORCE_INLINE String & operator+=(const String & s)
	{
		// Append data, should be sufficient
		data.append(s.data);

		// @see append(ansichar)
		data.appendSpace(1);
		data[data.getCount()] = '\0';

		return *this;
	}
	FORCE_INLINE String & append(const String & s) { return operator+=(s); }
	/// @}

	/**
	 * @brief Append C-string
	 * 
	 * @param [in] s C-string
	 * 
	 * @return self
	 * @{
	 */
	FORCE_INLINE String & operator+=(const ansichar s[])
	{
		if (s)
		{
			// Get length and push to array
			const uint64 length = strlen(s);
			data.appendSpace(length + 1);
			data.pushUnsafe(s, length);

			// @see append(ansichar)
			data[data.getCount()] = '\0';
		}

		return *this;
	}
	FORCE_INLINE String & append(const ansichar s[]) { return operator+=(s); }
	/// @}

	/**
	 * @brief Concatenate two strings
	 * @todo Overload for rvalue reference
	 * 
	 * @param [in] s other string
	 * 
	 * @return concateneted string
	 */
	FORCE_INLINE String operator+(const String & s) const
	{
		// Create empty and append
		String out;
		out += *this;
		out += s;

		return out;
	}

	/// @brief Remove leading characters
	FORCE_INLINE String & ltrim(ansichar c = ' ')
	{
		uint64 count = 0;
		while (data[count] == c) ++count;
		data.removeAt(0, count);

		// Null terminate
		data[getCount()] = '\0';

		return *this;
	}

	/// @brief Remove trailing characters
	FORCE_INLINE String & rtrim(ansichar c = ' ')
	{
		uint64 count = data.getCount() - 1;
		while (data[count] == c) --count;
		data.shrinkTo(count);

		// Null termiante
		data[getCount()] = '\0';

		return *this;
	}

	/**
	 * @brief Remove leading and trailing occurences of character
	 * 
	 * @param [in] c character
	 * 
	 * @return self
	 */
	FORCE_INLINE String & trim(ansichar c = ' ')
	{
		// Remove leading
		ltrim(c);
		// Remove trailing
		return rtrim(c);
	}
	
	/**
	 * @brief Append path component
	 * 
	 * @param [in] p path to append
	 */
	FORCE_INLINE String & operator/=(const String & s)
	{
		const ansichar PLATFORM_PATH_SEPARATOR = '/';

		// Remove trailing path separator
		rtrim(PLATFORM_PATH_SEPARATOR);

		// L-trim path
		uint64 count = 0;
		while (s.data[count] == PLATFORM_PATH_SEPARATOR) ++count;

		// Append
		data.push(PLATFORM_PATH_SEPARATOR);
		return append(*s + count, s.getCount() - count);
	}
};

#endif