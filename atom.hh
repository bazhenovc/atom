
#pragma once

///
/// Atom
///
/// Copyright (C) 2014 Kirill Bazhenov
/// This work is free. You can redistribute it and/or modify it under the
/// terms of the Do What The Fuck You Want To Public License, Version 2,
/// as published by Sam Hocevar. See the COPYING file or http://www.wtfpl.net/ 
/// for more details.
/// 
/// Atom is a library for manipulating human-readable ASCII string literals packed into 64 bit integers.
/// It allows creating and comparing strings that are up to 8 characters long very efficiently from the machine perspective.
/// It is a very small header-only library with NO additional dependencies. It does not #include anything and does not pollute
/// the global namespace with macros or global variables.
///
/// Use this to create and compare small strings. For example:
///
/// \code
/// atom_t atom0 = literal("Atom 0");
/// atom_t atom1 = literal("Atom 1");
/// if (atom0 != atom1) {
///     // do something useful
/// }
/// \endcode
///
/// All compilers will replace literal(...) calls with statically calculated constant, so creating a new atom takes zero time.
/// For instance, Clang 3.4.1 will generate the following assembler code for the example above:
///
/// \code
/// movabsq	$52915833107521, %rax   # Store "Atom 0"
/// movq	%rax, -8(%rsp)
/// movabsq	$54015344735297, %rax   # Store "Atom 1"
/// movq	%rax, -16(%rsp)
/// movq	-8(%rsp), %rax
/// cmpq	-16(%rsp), %rax         # Compare them
/// \endcode
///
/// As you can see, it generates a single MOV instruction to create each atom (store a constant in the register)
/// and a single CMP instruction to compare them.
/// What can be faster?
///
/// This library requires C++11 support in the compiler for static_assert usage, however this is not a strict requirement,
/// because static_assert can be easily implemented without C++11.
///
/// Officially supported compilers are:
///    MSVC  >= 2012
///    GCC   >= 4.4
///    ICC   >= 13.01
///    Clang >= 3.2
///

#define ATOM_INLINE inline

#ifdef _MSC_VER
#undef ATOM_INLINE
#define ATOM_INLINE __forceinline
#endif

#if defined(__GNUG__) || defined(__INTEL_COMPILER) || defined(__ICC)
#undef ATOM_INLINE
#define ATOM_INLINE __inline__  __attribute__((always_inline))
#endif

#ifdef __clang__ 
#undef ATOM_INLINE
#define ATOM_INLINE __inline__  __attribute__((always_inline))
#endif

/// Main library namespace
namespace atom
{
	/// A typedef for 64 bit unsigned integer. Guaranteed to be 64 bit.
	typedef unsigned long long atom_uint64_t;
	static_assert(sizeof(atom_uint64_t) == 8, "atom_uint64_t is required to be exactly 8 bytes");
	
	/// A typedef for 8 character string.
	typedef atom_uint64_t atom_t;

	/// Private namespace that contains implementation details.
	namespace detail
	{
		ATOM_INLINE atom_uint64_t set_byte(atom_uint64_t byte, atom_uint64_t value)
		{
			return value << byte * 8;
		}

		template <atom_uint64_t N, atom_uint64_t C>
		struct atom_helper
		{
			static ATOM_INLINE atom_uint64_t get_value(const char (&str)[N])
			{
				return set_byte(C, str[C]) | atom_helper<N, C - 1>::get_value(str);
			}
		};

		template <atom_uint64_t N>
		struct atom_helper<N, 0>
		{
			static ATOM_INLINE atom_uint64_t get_value(const char (&str)[N])
			{
				return set_byte(0, str[0]);
			}
		};
	}

	///
	/// Create a packed string literal.
	/// String length cannot be > 8
	///
	template <atom_uint64_t N>
	ATOM_INLINE atom_t literal(const char (&str)[N])
	{
		static_assert(N <= sizeof(atom_uint64_t), "string too large");
		return detail::atom_helper<N, N - 1>::get_value(str);
	}
}

#undef ATOM_INLINE
