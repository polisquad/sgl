#ifndef SGL_STRING_H
#define SGL_STRING_H

#include "containers_fwd.h"
#include "array.h"
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
	/// Underlying data
	Array<ansichar> data;

public:
	/// Default constructor
	FORCE_INLINE String() :
		data(16) {}

	/// String constructor
	FORCE_INLINE String(const ansichar * string)
		: data(PlatformString::strlen(string) + 1)
	{
		if (string)
		{
			data.count = data.size - 1;
			moveOrCopy(data.buffer, string, data.size);
		}
	}

	/// Provides access to underying data
	/// @{
	FORCE_INLINE ansichar *			operator*()			{ return data.buffer; }
	FORCE_INLINE const ansichar *	operator*() const	{ return data.buffer; }
	/// @}

	/// Random access operator
	/// @{
	FORCE_INLINE ansichar &			operator[](uint64 i)		{ return data.buffer[i]; }
	FORCE_INLINE const ansichar &	operator[](uint64 i) const	{ return data.buffer[i]; }
	/// @}

	/// Returns string length (without null terminating character)
	FORCE_INLINE uint64 getLength() const { return data.count; }
};

#endif