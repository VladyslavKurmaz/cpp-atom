/*
/-----------------------------------------------------------------------------\
| Copyright � 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.09.23 - defs extracted from z3d/slot.hpp                  |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:		                                                                    |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_NODE_TLDEFS_HPP
#define ATOM_NODE_TLDEFS_HPP

#define LOKI_TYPELIST_0() \
									::Loki::NullType
#define	DEFLIST_0 \
									::Loki::NullType
#define	DEFLIST_1( I1 ) \
									std::pair< I1, ::Loki::NullType >
#define	DEFLIST_2( I1, I2 ) \
									std::pair< I1, DEFLIST_1( I2 ) >
#define	DEFLIST_3( I1, I2, I3 ) \
									std::pair< I1, DEFLIST_2( I2, I3 ) >
#define	DEFLIST_4( I1, I2, I3, I4 ) \
									std::pair< I1, DEFLIST_3( I2, I3, I4 ) >
#define	DEFLIST_5( I1, I2, I3, I4, I5 ) \
									std::pair< I1, DEFLIST_4( I2, I3, I4, I5 ) >
#define	DEFLIST_6( I1, I2, I3, I4, I5, I6 ) \
									std::pair< I1, DEFLIST_5( I2, I3, I4, I5, I6 ) >
#define	DEFLIST_7( I1, I2, I3, I4, I5, I6, I7 ) \
									std::pair< I1, DEFLIST_6( I2, I3, I4, I5, I6, I7 ) >
#define	DEFLIST_8( I1, I2, I3, I4, I5, I6, I7, I8 ) \
									std::pair< I1, DEFLIST_7( I2, I3, I4, I5, I6, I7, I8 ) >
#define	DEFLIST_9( I1, I2, I3, I4, I5, I6, I7, I8, I9 ) \
									std::pair< I1, DEFLIST_8( I2, I3, I4, I5, I6, I7, I8, I9 ) >
#define	DEFLIST_10( I1, I2, I3, I4, I5, I6, I7, I8, I9, I10 ) \
									std::pair< I1, DEFLIST_9( I2, I3, I4, I5, I6, I7, I8, I9, I10 ) >
#define	DEFLIST_11( I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11 ) \
									std::pair< I1, DEFLIST_10( I2, I3, I4, I5, I6, I7, I8, I9, I10, I11 ) >
#define	DEFLIST_12( I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12 ) \
									std::pair< I1, DEFLIST_11( I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12 ) >
#define	DEFLIST_13( I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13 ) \
									std::pair< I1, DEFLIST_12( I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13 ) >
#define	DEFLIST_14( I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14 ) \
									std::pair< I1, DEFLIST_13( I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14 ) >
#define	DEFLIST_15( I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15 ) \
									std::pair< I1, DEFLIST_14( I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15 ) >

#define	INITLIST_0 \
									::Loki::NullType()
#define	INITLIST_1( I1 )\
									std::make_pair( I1, ::Loki::NullType() )
#define	INITLIST_2( I1, I2 ) \
									std::make_pair( I1, INITLIST_1( I2 ) )
#define	INITLIST_3( I1, I2, I3 ) \
									std::make_pair( I1, INITLIST_2( I2, I3 ) )
#define	INITLIST_4( I1, I2, I3, I4 ) \
									std::make_pair( I1, INITLIST_3( I2, I3, I4 ) )
#define	INITLIST_5( I1, I2, I3, I4, I5 ) \
									std::make_pair( I1, INITLIST_4( I2, I3, I4, I5 ) )
#define	INITLIST_6( I1, I2, I3, I4, I5, I6 ) \
									std::make_pair( I1, INITLIST_5( I2, I3, I4, I5, I6 ) )
#define	INITLIST_7( I1, I2, I3, I4, I5, I6, I7 ) \
									std::make_pair( I1, INITLIST_6( I2, I3, I4, I5, I6, I7 ) )
#define	INITLIST_8( I1, I2, I3, I4, I5, I6, I7, I8 ) \
									std::make_pair( I1, INITLIST_7( I2, I3, I4, I5, I6, I7, I8 ) )
#define	INITLIST_9( I1, I2, I3, I4, I5, I6, I7, I8, I9 ) \
									std::make_pair( I1, INITLIST_8( I2, I3, I4, I5, I6, I7, I8, I9 ) )
#define	INITLIST_10( I1, I2, I3, I4, I5, I6, I7, I8, I9, I10 ) \
									std::make_pair( I1, INITLIST_9( I2, I3, I4, I5, I6, I7, I8, I9, I10 ) )
#define	INITLIST_11( I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11 ) \
									std::make_pair( I1, INITLIST_10( I2, I3, I4, I5, I6, I7, I8, I9, I10, I11 ) )
#define	INITLIST_12( I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12 ) \
									std::make_pair( I1, INITLIST_11( I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12 ) )
#define	INITLIST_13( I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13 ) \
									std::make_pair( I1, INITLIST_12( I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13 ) )
#define	INITLIST_14( I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14 ) \
									std::make_pair( I1, INITLIST_13( I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14 ) )
#define	INITLIST_15( I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15 ) \
									std::make_pair( I1, INITLIST_14( I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15 ) )

#endif //ATOM_NODE_TLDEFS_HPP
