/*
   Copyright 2021 Daniel S. Buckstein

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	GPRO Net SDK: Networking framework.
	By Daniel S. Buckstein

	gpro-net-gamestate.h
	Header for common networking mini-game states.
*/

#ifndef _GPRO_NET_GAMESTATE_H_
#define _GPRO_NET_GAMESTATE_H_


#define gpro_flag_raise(value, flag)	(value | flag)	// value with flag raised
#define gpro_flag_lower(value, flag)	(value & ~flag)	// value with flag lowered
#define gpro_flag_check(value, flag)	(value & flag)	// value contains part of flag
#define gpro_flag_equal(value, flag)	(gpro_flag_check(value, flag) == flag)	// value contains entire flag


#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus


	struct MancalaBoard
	{
	private:


	public:


	};

typedef unsigned char
	gpro_mancala[2][8];			// mancala board (shared)

/*
	Mancala:
		[2]        ___________________________
				  /				7			  \
			 _______________________________________
		 0	| 0  | 1  | 2  | 3  | 4  | 5  | 6  |    |	[8]
			|    |____|____|____|____|____|____|    |
		 1	|    | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
			|____|____|____|____|____|____|____|____|

				  \___________________________/
								7
*/

typedef enum gpro_mancala_index
{
	gpro_mancala_score,
	gpro_mancala_cup1,
	gpro_mancala_cup2,
	gpro_mancala_cup3,
	gpro_mancala_cup4,
	gpro_mancala_cup5,
	gpro_mancala_cup6,
} gpro_mancala_index;

inline void gpro_mancala_reset(gpro_mancala gs)
{
	long long* itr = (long long*)gs;
	itr[0] = itr[1] = 0x0004040404040400; // 6 cups, 4 stones each
	gs[0][7] = gs[1][7] = 24; // on-side total
}


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !_GPRO_NET_GAMESTATE_H_