/********************************************************************************/
/*										*/
/*	logic.h -- definitions for game logic					*/
/*										*/
/********************************************************************************/




/********************************************************************************/
/*										*/
/*	EEPROM locations							*/
/*										*/
/********************************************************************************/

const int LOGIC_HIGH_SCORE_ADDRESS = 0;
const int LOGIC_HIGH_SCORE_LENGTH = sizeof(long);


/********************************************************************************/
/*										*/
/*	Constants								*/
/*										*/
/********************************************************************************/

const int NUM_PLAYER = 4;
const long LOGIC_UPDATE = 10000;

const long LOGIC_HIGH_SCORE_DISPLAY_TIME = 1000000;
const long LOGIC_GAME_UP_STEP_TIME = 500000;
const long LOGIC_SHOOT_AGAIN_TIME = 8000000;
const long LOGIC_NEXT_PLAYER_WAIT = 500000;
const long LOGIC_NEXT_SHOOTER_UPDATE = 500000;
const long LOGIC_SHOOTER_DELAY = 500000;
const long LOGIC_NEXT_RELEASE_UPDATE = 2000000;
const long LOGIC_RELEASE_DELAY = 2000000;
const long LOGIC_END_GAME_DELAY = 3000000;

const long LOGIC_RESET_TIME = 150000;




/********************************************************************************/
/*										*/
/*	Enumerations								*/
/*										*/
/********************************************************************************/

enum GameState {
   GAME_IDLE,
   GAME_UP,
   GAME_PLAY,
};




/********************************************************************************/
/*										*/
/*	Data structures 							*/
/*										*/
/********************************************************************************/


typedef struct _Player {
   long player_score;
   int player_balls;
} Player;


typedef struct _GameData {
   Player player[NUM_PLAYER];
   int num_players;
   int game_credits;
   long high_score;
   long saved_high_score;
   int current_player;
   bool in_play;
   bool is_tilt;
   int ae_lamps;
   long bonus_value;
   long bonus_mult;
   long laser_score;
   int star_flags;
   int num_stars;
   int roll_flags;
   int num_rolls;
   int free_balls;
} GameData;







/********************************************************************************/
/*										*/
/*	Light Names								*/
/*										*/
/********************************************************************************/

const int	LIGHT_SHOOT_AGAIN		= 0;
const int	LIGHT_BULLS_EYE_EXTRA		= 1;
const int	LIGHT_AE_1			= 2;
const int	LIGHT_AE_2			= 3;
const int	LIGHT_AE_3			= 4;
const int	LIGHT_AE_4			= 5;
const int	LIGHT_AE_5			= 6;
const int	LIGHT_AE_6			= 7;
const int	LIGHT_LR_SPECIALS		= 8;
const int	LIGHT_BONUS_10K 		= 9;
const int	LIGHT_MULT_2X			= 10;
const int	LIGHT_MULT_3X			= 11;
const int	LIGHT_MULT_5X			= 12;
const int	LIGHT_BONUS_1K			= 13;
const int	LIGHT_BONUS_2K			= 14;
const int	LIGHT_BONUS_3K			= 15;
const int	LIGHT_BONUS_4K			= 16;
const int	LIGHT_BONUS_5K			= 17;
const int	LIGHT_BONUS_6K			= 18;
const int	LIGHT_BONUS_7K			= 19;
const int	LIGHT_BONUS_8K			= 20;
const int	LIGHT_BONUS_9K			= 21;
const int	LIGHT_BULLS_EYE_SPECIAL 	= 22;
const int	LIGHT_SPINNER_1K		= 23;
const int	LIGHT_BONUS_20K 		= 24;
const int	LIGHT_LASER_10K 		= 25;
const int	LIGHT_LASER_15K 		= 26;
const int	LIGHT_LASER_20K 		= 27;
const int	LIGHT_TOP_1			= 28;
const int	LIGHT_TOP_2			= 29;
const int	LIGHT_TOP_3			= 30;
const int	LIGHT_TOP_4			= 31;
const int	LIGHT_TOP_5			= 32;
const int	LIGHT_TOP_6			= 33;
const int	LIGHT_TOP_7			= 34;
const int	LIGHT_TOP_8			= 35;
const int	LIGHT_TOP_9			= 36;
const int	LIGHT_TOP_10			= 37;
const int	LIGHT_STAR_1			= 38;
const int	LIGHT_STAR_2			= 39;
const int	LIGHT_STAR_3			= 40;
const int	LIGHT_STAR_4			= 41;
const int	LIGHT_STAR_5			= 42;
const int	LIGHT_STAR_6			= 43;
const int	LIGHT_STAR_7			= 44;
const int	LIGHT_STAR_8			= 45;
const int	LIGHT_STAR_9			= 46;
const int	LIGHT_STAR_10			= 47;
const int	LIGHT_E_HOLE_EXTRA_BALL 	= 48;
const int	LIGHT_PLAYER_1			= 49;
const int	LIGHT_PLAYER_2			= 50;
const int	LIGHT_PLAYER_3			= 51;
const int	LIGHT_PLAYER_4			= 52;
const int	LIGHT_MATCH			= 53;
const int	LIGHT_BALL_IN_PLAY		= 54;
const int	LIGHT_CREDITS			= 55;
const int	LIGHT_PLAYER_1_UP		= 56;
const int	LIGHT_PLAYER_2_UP		= 57;
const int	LIGHT_PLAYER_3_UP		= 58;
const int	LIGHT_PLAYER_4_UP		= 59;
const int	LIGHT_TILT			= 60;
const int	LIGHT_GAME_OVER 		= 61;
const int	LIGHT_SHOOT_AGAIN_BACK		= 62;
const int	LIGHT_HIGH_SCORE		= 63;




/********************************************************************************/
/*										*/
/*	Solenoid Names								*/
/*										*/
/********************************************************************************/

const int	SOLENOID_BALL_RELEASE			= 0;
const int	SOLENOID_LEFT_SHOOTER			= 1;
const int	SOLENOID_LA_DROP_RESET			= 2;
const int	SOLENOID_SER_DROP_RESET 		= 3;
const int	SOLENOID_EJECT_HOLE			= 4;
const int	SOLENOID_BA_DROP_RESET			= 5;
const int	SOLENOID_LL_DROP_RESET			= 6;
const int	SOLENOID_RIGHT_SHOOTER			= 7;
const int	SOLENOID_CREDIT_KNOCKER_BROKEN		= 8;
const int	SOLENOID_LASER_LAMPS			= 9;
const int	SOLENOID_COIN_LOCKOUT			= 10;



/********************************************************************************/
/*										*/
/*	Switch names								*/
/*										*/
/********************************************************************************/

const int	SWITCH_PLUMB_BOB_TILT		       = 0;
const int	SWITCH_BALL_ROLL_TILT		       = 1;
const int	SWITCH_CREDIT_BUTTON		       = 2;
const int	SWITCH_RIGHT_COIN_CHUTE 	       = 3;
const int	SWITCH_CENTER_COIN_CHUTE	       = 4;
const int	SWITCH_LEFT_COIN_CHUTE		       = 5;
const int	SWITCH_SLAM_TILT		       = 6;
const int	SWITCH_HIGH_SCORE_RESET 	       = 7;
const int	SWITCH_OUTHOLE			       = 8;
const int	SWITCH_LEFT_SHOOTER		       = 9;
const int	SWITCH_LEFT_SPECIAL		       = 10;
const int	SWITCH_LEFT_INSIDE_ROLLOVER	       = 11;
const int	SWITCH_LEFT_KICKER		       = 12;
const int	SWITCH_MIDDLE_LEFT_SIDE_STANDUP        = 13;
const int	SWITCH_LEFT_SIDE_TARGET 	       = 14;
const int	SWITCH_ADVANCE_BONUS		       = 15;
const int	SWITCH_LEFT_SPINNER		       = 16;
const int	SWITCH_BULLS_EYE_TARGET 	       = 17;
const int	SWITCH_LASER_L_DROP_TARGET	       = 18;
const int	SWITCH_LASER_A_DROP_TARGET	       = 19;
const int	SWITCH_LASER_S_DROP_TARGET	       = 20;
const int	SWITCH_LASER_E_DROP_TARGET	       = 21;
const int	SWITCH_LASER_R_DROP_TARGET	       = 22;
const int	SWITCH_LASER_DROP_TARGET_SERIES        = 23;
const int	SWITCH_LOWER_TOP_LEFT_STANDUP	       = 24;
const int	SWITCH_UPPER_TOP_LEFT_STANDUP	       = 25;
const int	SWITCH_ROLLOVER_1_AND_2 	       = 26;
const int	SWITCH_ROLLOVER_3_AND_4 	       = 27;
const int	SWITCH_ROLLOVER_5_AND_6 	       = 28;
const int	SWITCH_ROLLOVER_7_AND_8 	       = 29;
const int	SWITCH_ROLLOVER_9_AND_10	       = 30;
const int	SWITCH_EJECT_HOLE		       = 31;
const int	SWITCH_UPPER_RIGHT_STANDUP	       = 32;
const int	SWITCH_BALL_B_DROP_TARGET	       = 33;
const int	SWITCH_BALL_A_DROP_TARGET	       = 34;
const int	SWITCH_BALL_L_DROP_TARGET	       = 35;
const int	SWITCH_BALL_L_DROP_TARGET_BOTTOM       = 36;
const int	SWITCH_BALL_DROP_TARGET_SERIES	       = 37;
const int	SWITCH_BALL_DROP_TARGET_STANDUP        = 38;
const int	SWITCH_RIGHT_SPINNER		       = 39;
const int	SWITCH_RIGHT_SHOOTER		       = 40;
const int	SWITCH_RIGHT_SPECIAL		       = 41;
const int	SWITCH_RIGHT_INSIDE_ROLLOVER	       = 42;
const int	SWITCH_RIGHT_KICKER		       = 43;
const int	SWITCH_LEFT_JET_BUMPER		       = 44;
const int	SWITCH_RIGHT_JET_BUMPER 	       = 45;
const int	SWITCH_CENTER_JET_BUMPER	       = 46;
const int	SWITCH_SER_DROP_TARGET_STANDUP	       = 47;
const int	SWITCH_PLAYFIELD_TILT		       = 48;
const int	SWITCH_STAR_1_ROLLOVER		       = 49;
const int	SWITCH_STAR_2_ROLLOVER		       = 50;
const int	SWITCH_STAR_3_ROLLOVER		       = 51;
const int	SWITCH_STAR_4_ROLLOVER		       = 52;
const int	SWITCH_STAR_5_ROLLOVER		       = 53;
const int	SWITCH_STAR_6_ROLLOVER		       = 54;
const int	SWITCH_STAR_7_ROLLOVER		       = 55;
const int	SWITCH_STAR_8_ROLLOVER		       = 56;
const int	SWITCH_STAR_9_ROLLOVER		       = 57;
const int	SWITCH_STAR_10_ROLLOVER 	       = 58;




/********************************************************************************/
/*										*/
/*	Sound names								*/
/*										*/
/********************************************************************************/

const int	SOUND_GROAN			= 0;
const int	SOUND_DING1			= 1;
const int	SOUND_DING2			= 2;
const int	SOUND_DING3			= 3;
const int	SOUND_DING4			= 4;
const int	SOUND_PHEW			= 5;
const int	SOUND_THROB			= 6;
const int	SOUND_SPINNER			= 7;
const int	SOUND_BLAST			= 8;

		


/********************************************************************************/
/*										*/
/*	External definitions							*/
/*										*/
/********************************************************************************/

extern void logicSetup(void);
extern void logicWrap(void);
extern void logicUpdate(unsigned long);
extern void logicReset(void);




/* end of logic.h */

