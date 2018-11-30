#ifndef SGL_STRING_H
#define SGL_STRING_H

#include "containers/array.h"

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
		if (buffer && size) data.pushUnsafe(buffer, size);
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
			data.pushUnsafe(s, length);
		}

		return *this;
	}
	FORCE_INLINE String & append(const ansichar s[]) { return operator+=(s); }
	/// @}

public:
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
};

#endif