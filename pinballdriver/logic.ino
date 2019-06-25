/********************************************************************************/
/*										*/
/*	logic.ino -- game playing logic 			    .		*/
/*										*/
/********************************************************************************/




/********************************************************************************/
/*										*/
/*	Private Storage 					    .		*/
/*										*/
/********************************************************************************/

static GameState	game_state;
static GameData 	game_data;

static unsigned long	next_logic;
static unsigned long	next_highscore;
static unsigned long	next_gameup;
static unsigned long	shoot_again_limit;
static unsigned long	next_laser_reset;
static unsigned long	next_balls_reset;
static unsigned long	next_start_turn;

static int		gameup_state;




/********************************************************************************/
/*										*/
/*	Logic Initialization methods						*/
/*										*/
/********************************************************************************/

void logicSetup()
{
   game_state = GAME_IDLE;

   game_data.game_credits = 0;
   game_data.high_score = 0;
   game_data.saved_high_score = 0;
   game_data.star_flags = 0;
   game_data.num_stars = 0;

   readHighScore();

   gameOver();

   next_logic = 0;
   next_highscore = 0;
   next_gameup = 0;
   gameup_state = 0;
   next_laser_reset = 0;
   next_balls_reset = 0;
   next_start_turn = 0;
}



static void initPlayer(int i)
{
   game_data.player[i].player_balls = 0;
   game_data.player[i].player_score = 0;
}


/********************************************************************************/
/*										*/
/*	Logic Update methods							*/
/*										*/
/********************************************************************************/

void logicWrap()
{
   next_logic = 0;
   next_highscore = 0;
   if (next_gameup != 0) next_gameup = 500000;
}



void logicUpdate(unsigned long now)
{
   if (now >= next_logic) {
      switch (game_state) {
	 case GAME_IDLE:
	    handleIdleLogic(now);
	    break;
	 case GAME_UP :
	    handleGameUpLogic(now);
	    break;
	 case GAME_PLAY :
	    handleGamePlayLogic(now);
	    break;
       }
      next_logic = addTime(now, LOGIC_UPDATE);
    }
}



static void handleIdleLogic(unsigned long now)
{
   if (now > next_highscore) {
      highScoreDisplay();
      next_highscore = addTime(now, LOGIC_HIGH_SCORE_DISPLAY_TIME);
    }

   handleSwitchChanges(4, idleSwitchOff, idleSwitchOn);
   if (game_state == GAME_UP) {
      next_gameup = 0;
      gameup_state = 0;
    }
}


static void handleGameUpLogic(unsigned long now)
{
   if (now > next_gameup) {
      gameUpTimer();
      if (game_state == GAME_UP) {
	 next_gameup = addTime(now, LOGIC_GAME_UP_STEP_TIME);
       }
      else {
	 next_gameup = 0;
	 gameup_state = 0;
       }
    }

   handleSwitchChanges(4, gameupSwitchOff, gameupSwitchOn);
}



static void handleGamePlayLogic(unsigned long now)
{
   if (now > next_laser_reset) next_laser_reset = 0;
   if (now > next_balls_reset) next_balls_reset = 0;
   if (shoot_again_limit > 0 && now > shoot_again_limit) disableShootAgain();
   if (next_start_turn > 0 && now > next_start_turn) startTurn();

   handleSwitchChanges(4, gamePlaySwitchOff, gamePlaySwitchOn);
}


void logicReset()
{ }




/********************************************************************************/
/*										*/
/*	Generic switch methods							*/
/*										*/
/********************************************************************************/

void defaultSwitchOff(int sw)
{
   // nothing to do
}



void defaultSwitchOn(int sw)
{
   switch (sw) {
      case SWITCH_HIGH_SCORE_RESET :
	 resetHighScore();
	 break;
      default :
	 break;
    }
}



/********************************************************************************/
/*										*/
/*	Game Over and Idle Mode 						*/
/*										*/
/********************************************************************************/

void gameOver()
{
   disableAllLights();
   removeAllSolenoids();
   specialDisable();

   for (int i = 0; i < NUM_PLAYER; ++i) initPlayer(i);
   game_data.current_player = 0;

   noteHighScore();

   game_data.in_play = false;
   game_data.is_tilt = false;
   game_data.ae_lamps = 0;
   game_data.bonus_value = 1000;
   game_data.bonus_mult = 1;
   game_data.laser_score = 0;
   game_data.num_players = 0;
   game_data.star_flags = 0;
   game_data.num_stars = 0;
   game_data.roll_flags = 0;
   game_data.num_rolls = 0;

   resetLASERDropTargets();
   resetBALLDropTargets();
   blankScores();
   updateCredits();

   lightOff(LIGHT_PLAYER_1);
   lightOff(LIGHT_PLAYER_2);
   lightOff(LIGHT_PLAYER_3);
   lightOff(LIGHT_PLAYER_4);
   lightOn(LIGHT_GAME_OVER);

   ballOutOfPlay();

   game_state = GAME_IDLE;
   next_highscore = 0;
}


void highScoreDisplay()
{
   static int lastdisplay = 3;
   int which = 0;
   do {
      which = random(NUM_PLAYER);
    }
   while (which == lastdisplay);
   blankScores();
   setDisplay(which, game_data.high_score);
   disableAllLights();
   lightOn(LIGHT_GAME_OVER);
   for (int i = 0; i < NUM_LIGHTS; ++i) {
      if (random(2) == 1) lightOn(i);
    }
}



static void idleSwitchOff(int which)
{
   switch (which) {
      case SWITCH_RIGHT_COIN_CHUTE :
	 if (game_data.num_players == 0) addCredit();
	 break;
      default :
	 defaultSwitchOff(which);
	 break;
    }
}



static void idleSwitchOn(int which)
{
   switch (which) {
      case SWITCH_CREDIT_BUTTON :
	 addPlayer();
	 game_state = GAME_UP;
	 gameup_state = 6;
	 break;
      default :
	 defaultSwitchOn(which);
	 break;
    }
}



/********************************************************************************/
/*										*/
/*	Game-Up methods 							*/
/*										*/
/********************************************************************************/

static void gameUpTimer()
{
   int time = 0;

   switch (gameup_state) {
      case 0 :
      case 2 :
      case 4 :
	 time = (6 - gameup_state) / 2;
	 setDisplayRight(time);
	 break;
      case 1 :
      case 3 :
      case 5 :
	 blankDisplayRight();
	 break;
      case 6 :
	 gameUp();
	 break;
    }
}



static void gameUp()
{
   lightOff(LIGHT_HIGH_SCORE);
   disableAllLights();
   blankScores();
   game_data.current_player = 0;
   checkHighScore();

   lightOn(LIGHT_PLAYER_1_UP);
   lightOn(LIGHT_BONUS_1K);
   lightOff(LIGHT_GAME_OVER);

   startTurn();
}



static void gameupSwitchOn(int which)
{
   switch (which) {
      case SWITCH_CREDIT_BUTTON :
	 addPlayer();
	 game_state = GAME_UP;
	 gameup_state = 0;
	 break;
      default :
	 defaultSwitchOn(which);
	 break;
    }
}



static void gameupSwitchOff(int which)
{
   defaultSwitchOff(which);
}


static void startTurn()
{
   next_start_turn = 0;
   game_state = GAME_PLAY;
   updatePlayerInfo();
   checkHighScore();
   releaseBall();
   specialEnable();
}



/********************************************************************************/
/*										*/
/*	Game Play logic 							*/
/*										*/
/********************************************************************************/

static void gamePlaySwitchOn(int which)
{
   switch (which) {
      case SWITCH_OUTHOLE :
	 handleOutHole();
	 break;
      case SWITCH_PLUMB_BOB_TILT :
      case SWITCH_BALL_ROLL_TILT :
	 tilt();
	 break;
      case SWITCH_LEFT_SHOOTER :
      case SWITCH_RIGHT_SHOOTER :
	 handleShooter(which);
	 break;
      case SWITCH_LEFT_SPECIAL :
      case SWITCH_RIGHT_SPECIAL :
	 handleSpecial();
	 break;
      case SWITCH_LEFT_INSIDE_ROLLOVER :
      case SWITCH_RIGHT_INSIDE_ROLLOVER :
	 handleInsideRollover();
	 break;
      case SWITCH_LEFT_KICKER :
      case SWITCH_RIGHT_KICKER :
	 handleKicker();
	 break;
      case SWITCH_MIDDLE_LEFT_SIDE_STANDUP :
	 handleMLSStandup();
	 break;
      case SWITCH_LEFT_SIDE_TARGET :
	 handleLSTarget();
	 break;
      case SWITCH_LEFT_SPINNER :
      case SWITCH_RIGHT_SPINNER :
	 handleSpinner();
	 break;
      case SWITCH_BULLS_EYE_TARGET :
	 handleBETarget();
	 break;
      case SWITCH_LASER_DROP_TARGET_SERIES :
	 handleLDropTargetSeries();
	 break;
      case SWITCH_LOWER_TOP_LEFT_STANDUP :
      case SWITCH_UPPER_TOP_LEFT_STANDUP :
	 handleTopStandup();
	 break;
      case SWITCH_ROLLOVER_1_AND_2 :
      case SWITCH_ROLLOVER_3_AND_4 :
      case SWITCH_ROLLOVER_5_AND_6 :
      case SWITCH_ROLLOVER_7_AND_8 :
      case SWITCH_ROLLOVER_9_AND_10 :
	 handleRollover(which);
	 break;
      case SWITCH_EJECT_HOLE :
	 handleEjectHole();
	 break;
      case SWITCH_UPPER_RIGHT_STANDUP :
	 handleTopStandup();
	 break;
      case SWITCH_BALL_DROP_TARGET_SERIES :
	 handleBDropTargetSeries();
	 break;
      case SWITCH_BALL_DROP_TARGET_STANDUP :
	 handleBDTStandup();
	 break;
      case SWITCH_LEFT_JET_BUMPER :
      case SWITCH_RIGHT_JET_BUMPER :
      case SWITCH_CENTER_JET_BUMPER :
	 handleJetBumper();
	 break;
      case SWITCH_SER_DROP_TARGET_STANDUP :
	 handleSDTStandup();
	 break;
      case SWITCH_STAR_1_ROLLOVER :
      case SWITCH_STAR_2_ROLLOVER :
      case SWITCH_STAR_3_ROLLOVER :
      case SWITCH_STAR_4_ROLLOVER :
      case SWITCH_STAR_5_ROLLOVER :
      case SWITCH_STAR_6_ROLLOVER :
      case SWITCH_STAR_7_ROLLOVER :
      case SWITCH_STAR_8_ROLLOVER :
      case SWITCH_STAR_9_ROLLOVER :
      case SWITCH_STAR_10_ROLLOVER :
	 handleStar(which);
	 break;
      case SWITCH_LASER_L_DROP_TARGET :
      case SWITCH_LASER_A_DROP_TARGET :
      case SWITCH_LASER_S_DROP_TARGET :
      case SWITCH_LASER_E_DROP_TARGET :
      case SWITCH_LASER_R_DROP_TARGET :
	 handleLASERDropTarget();
	 break;
      case SWITCH_BALL_B_DROP_TARGET :
      case SWITCH_BALL_A_DROP_TARGET :
      case SWITCH_BALL_L_DROP_TARGET :
      case SWITCH_BALL_L_DROP_TARGET_BOTTOM :
	 handleBALLDropTarget();
	 break;

      default :
	 defaultSwitchOn(which);
    }
}



static void gamePlaySwitchOff(int which)
{
   defaultSwitchOff(which);
}



/********************************************************************************/
/*										*/
/*	Game Playing methods							*/
/*										*/
/********************************************************************************/

static void incrBonusMult()
{
   switch (game_data.bonus_mult) {
      case 1 :
	 game_data.bonus_mult = 2;
	 lightOn(LIGHT_MULT_2X);
	 break;
      case 2 :
	 game_data.bonus_mult = 3;
	 lightOff(LIGHT_MULT_2X);
	 lightOn(LIGHT_MULT_3X);
	 break;
      case 3 :
	 game_data.bonus_mult = 5;
	 lightOff(LIGHT_MULT_3X);
	 lightOn(LIGHT_MULT_5X);
	 lightOn(LIGHT_SPINNER_1K);
	 break;
      default :
	 break;
    }
}



static void incrBonus()
{
   switch (game_data.bonus_value) {
      case 1000 :
	 game_data.bonus_value = 2000;
	 lightOff(LIGHT_BONUS_1K);
	 lightOn(LIGHT_BONUS_2K);
	 break;
      case 2000 :
	 game_data.bonus_value = 3000;
	 lightOff(LIGHT_BONUS_2K);
	 lightOn(LIGHT_BONUS_3K);
	 break;
      case 3000 :
	 game_data.bonus_value = 4000;
	 lightOff(LIGHT_BONUS_3K);
	 lightOn(LIGHT_BONUS_4K);
	 break;
      case 4000 :
	 game_data.bonus_value = 5000;
	 lightOff(LIGHT_BONUS_4K);
	 lightOn(LIGHT_BONUS_5K);
	 break;
      case 5000 :
	 game_data.bonus_value = 6000;
	 lightOff(LIGHT_BONUS_5K);
	 lightOn(LIGHT_BONUS_6K);
	 break;
      case 6000 :
	 game_data.bonus_value = 7000;
	 lightOff(LIGHT_BONUS_6K);
	 lightOn(LIGHT_BONUS_7K);
	 break;
      case 7000 :
	 game_data.bonus_value = 8000;
	 lightOff(LIGHT_BONUS_7K);
	 lightOn(LIGHT_BONUS_8K);
	 break;
      case 8000 :
	 game_data.bonus_value = 9000;
	 lightOff(LIGHT_BONUS_8K);
	 lightOn(LIGHT_BONUS_9K);
	 break;
      case 9000 :
	 game_data.bonus_value = 10000;
	 lightOff(LIGHT_BONUS_9K);
	 lightOn(LIGHT_BONUS_10K);
	 break;
      case 10000 :
	 game_data.bonus_value = 20000;
	 lightOff(LIGHT_BONUS_10K);
	 lightOn(LIGHT_BONUS_20K);
	 break;
      default :
	 break;
    }
}





static void handleShooter(int sw)
{
   if (game_data.is_tilt) return;

   int score = 500 + 1000 * game_data.num_stars;
   addPoints(score);
   if (sw == SWITCH_LEFT_SHOOTER) queueSolenoid(SOLENOID_LEFT_SHOOTER);
   else queueSolenoid(SOLENOID_RIGHT_SHOOTER);
   queueSound(SOUND_GROAN);
}




static void handleStar(int sw)
{
   if (game_data.is_tilt) return;

   int star = sw - SWITCH_STAR_1_ROLLOVER;
   int light = LIGHT_STAR_1 + star;
   if (insertStar(star)) {
      lightOn(light);
    }
   queueSound(SOUND_DING1);
   queueSound(SOUND_DING2);
   queueSound(SOUND_DING3);
   if (game_data.num_stars == 10) {
      if (game_data.ae_lamps < 6) {
	 lightOn(LIGHT_AE_1 + game_data.ae_lamps);
	 game_data.ae_lamps++;
       }
      if (game_data.ae_lamps >= 5) addPoints(10000);
      queueSound(SOUND_DING2);
      queueSound(SOUND_DING1);
      queueSound(SOUND_DING2);
      queueSound(SOUND_DING3);
      queueSound(SOUND_DING2);
      queueSound(SOUND_DING1);
      lightOff(LIGHT_STAR_1);
      lightOff(LIGHT_STAR_2);
      lightOff(LIGHT_STAR_3);
      lightOff(LIGHT_STAR_4);
      lightOff(LIGHT_STAR_5);
      lightOff(LIGHT_STAR_6);
      lightOff(LIGHT_STAR_7);
      lightOff(LIGHT_STAR_8);
      lightOff(LIGHT_STAR_9);
      lightOff(LIGHT_STAR_10);
      resetStars();
    }
   addPoints(50);
}



static void handleSpecial()
{
   if (game_data.is_tilt) return;

   queueSound(SOUND_THROB);
   addPoints(5000);
}


static void handleInsideRollover()
{
   if (game_data.is_tilt) return;

   queueSound(SOUND_DING1);
   addPoints(3000);
   incrBonus();
}


static void handleKicker()
{
   if (game_data.is_tilt) return;

   queueSound(SOUND_BLAST);
   addPoints(10);
}


static void handleMLSStandup()
{
   if (game_data.is_tilt) return;

   queueSound(SOUND_BLAST);
   addPoints(300);
}


static void handleLSTarget()
{
   if (game_data.is_tilt) return;

   queueSound(SOUND_THROB);
   addPoints(1000);
   incrBonus();
}



static void handleSpinner()
{
   if (game_data.is_tilt) return;

   int score = 100;
   queueSound(SOUND_SPINNER);
   if (game_data.bonus_mult == 5) score = 1000;
   addPoints(score);
}



static void handleBETarget()
{
   if (game_data.is_tilt) return;

   queueSound(SOUND_THROB);
   if (!isLightEnabled(LIGHT_BULLS_EYE_EXTRA)) {
      lightOn(LIGHT_BULLS_EYE_EXTRA);
      ++game_data.player[game_data.current_player].player_balls;
    }
   else if (!isLightEnabled(LIGHT_BULLS_EYE_SPECIAL)) {
      lightOn(LIGHT_BULLS_EYE_SPECIAL);
    }
   addPoints(10000);
   incrBonusMult();
}



static void handleDropTarget()
{
   if (game_data.is_tilt) return;

   queueSound(SOUND_BLAST);
   addPoints(500);
}


static void handleRollover(int sw)
{
   if (game_data.is_tilt) return;

   int seen = 0;
   addPoints(1000);
   queueSound(SOUND_DING3);
   queueSound(SOUND_DING2);
   queueSound(SOUND_DING4);
   switch (sw) {
      case SWITCH_ROLLOVER_1_AND_2 :
	 if (insertRollover(1)) lightOn(LIGHT_TOP_1);
	 else if (insertRollover(2)) lightOn(LIGHT_TOP_2);
	 break;
      case SWITCH_ROLLOVER_3_AND_4 :
	 if (insertRollover(3)) lightOn(LIGHT_TOP_3);
	 else if (insertRollover(4)) lightOn(LIGHT_TOP_4);
	 break;
      case SWITCH_ROLLOVER_5_AND_6 :
	 if (insertRollover(5)) lightOn(LIGHT_TOP_5);
	 else if (insertRollover(6)) lightOn(LIGHT_TOP_6);
	 break;
      case SWITCH_ROLLOVER_7_AND_8 :
	 if (insertRollover(7)) lightOn(LIGHT_TOP_7);
	 else if (insertRollover(8)) lightOn(LIGHT_TOP_8);
	 break;
      case SWITCH_ROLLOVER_9_AND_10 :
	 if (insertRollover(9)) lightOn(LIGHT_TOP_9);
	 else if (insertRollover(10)) lightOn(LIGHT_TOP_10);
	 break;
    }
   if (game_data.num_rolls == 10) {
      queueSound(SOUND_DING1);
      queueSound(SOUND_DING2);
      queueSound(SOUND_DING3);
      queueSound(SOUND_DING1);
      queueSound(SOUND_DING2);
      queueSound(SOUND_DING3);
      queueSound(SOUND_DING1);
      queueSound(SOUND_DING2);
      queueSound(SOUND_DING3);
      addPoints(game_data.bonus_value * game_data.bonus_mult);
      resetRollovers();
      lightOff(LIGHT_TOP_1);
      lightOff(LIGHT_TOP_2);
      lightOff(LIGHT_TOP_3);
      lightOff(LIGHT_TOP_4);
      lightOff(LIGHT_TOP_5);
      lightOff(LIGHT_TOP_6);
      lightOff(LIGHT_TOP_7);
      lightOff(LIGHT_TOP_8);
      lightOff(LIGHT_TOP_9);
      lightOff(LIGHT_TOP_10);
    }
   incrBonus();
}


static void handleTopStandup()
{
   if (game_data.is_tilt) return;

   queueSound(SOUND_BLAST);
   addPoints(10);
}


static void handleEjectHole()
{
   if (game_data.is_tilt) return;

   queueSolenoid(SOLENOID_EJECT_HOLE);
   addPoints(5000);
   queueSound(SOUND_GROAN);
   incrBonusMult();
}




static void handleBDTStandup()
{
   if (game_data.is_tilt) return;

   queueSound(SOUND_BLAST);
   addPoints(50);
}



static void handleJetBumper()
{
   if (game_data.is_tilt) return;

   queueSound(SOUND_BLAST);
   addPoints(100);
}


static void handleSDTStandup()
{
   if (game_data.is_tilt) return;

   queueSound(SOUND_BLAST);
   addPoints(10);
}


static void handleOutHole()
{
   if (shoot_again_limit > 0 && micros() < shoot_again_limit) {
      queueSolenoid(SOLENOID_BALL_RELEASE);
      disableShootAgain();
      addPoints(10);
    }
   else if (game_data.free_balls > 0) {
      --game_data.free_balls;
      queueSolenoid(SOLENOID_BALL_RELEASE);
      addPoints(10);
      if (game_data.free_balls == 0) disableShootAgain();
    }
   else {
      endTurn();
    }
}



/********************************************************************************/
/*										*/
/*	End turn methods							*/
/*										*/
/********************************************************************************/

static void endTurn()
{
   queueSound(SOUND_DEATH);
   // setGameUp(DISABLED);
   ballOutOfPlay();
   game_data.player[game_data.current_player].player_balls--;
   updatePlayerInfo();

   disableAllLights();
   for (int i = 0; i < game_data.num_players; ++i) {
      if (game_data.player[i].player_balls > 0) lightOn(LIGHT_PLAYER_1 + i);
    }
   int next = -1;
   for (int i = 0; i < game_data.num_players; ++i) {
      int p = game_data.current_player + 1 + i;
      if (p >= game_data.num_players) p -= game_data.num_players;
      if (game_data.player[p].player_balls > 0) {
	 next = p;
	 break;
       }
    }
   if (next < 0) {
      gameOver();
    }
   else {
      game_data.current_player = next;
      if (game_data.is_tilt) {
	 lightOff(LIGHT_TILT);
	 game_data.is_tilt = false;
	 specialEnable();
       }
      lightOn(LIGHT_PLAYER_1_UP + next);
      resetStars();
      resetRollovers();
      game_data.ae_lamps = 0;
      resetLASERDropTargets();
      resetBALLDropTargets();
      game_data.bonus_value = 1000;
      game_data.bonus_mult = 1;
      game_data.laser_score = 0;
      lightOn(LIGHT_BONUS_1K);
      next_start_turn = addTime(micros(),LOGIC_NEXT_PLAYER_WAIT);
    }
}


static void disableShootAgain()
{
   lightOff(LIGHT_SHOOT_AGAIN);
   lightOff(LIGHT_SHOOT_AGAIN_BACK);
   shoot_again_limit = 0;
}



/********************************************************************************/
/*										*/
/*	Scoring methods 							*/
/*										*/
/********************************************************************************/

static void blankScores()
{
   blankDisplay(0);
   blankDisplay(1);
   blankDisplay(2);
   blankDisplay(3);
}



static void addPoints(int num)
{
   if (game_data.is_tilt) return;

   game_data.player[num].player_score += num;
   updatePlayerInfo();
}



/********************************************************************************/
/*										*/
/*	Drop Target Methods							*/
/*										*/
/********************************************************************************/

static void resetLASERDropTargets()
{
   queueSolenoid(SOLENOID_LA_DROP_RESET);
   queueSolenoid(SOLENOID_SER_DROP_RESET);
   next_laser_reset = addTime(micros(),LOGIC_RESET_TIME);
}


static void resetBALLDropTargets()
{
   queueSolenoid(SOLENOID_BA_DROP_RESET);
   queueSolenoid(SOLENOID_LL_DROP_RESET);
   next_balls_reset = addTime(micros(),LOGIC_RESET_TIME);
}


static void handleLASERDropTarget()
{
   if (next_laser_reset > 0) return;
   queueSound(SOUND_BLAST);
   addPoints(500);
}


static void handleBALLDropTarget()
{
   if (next_balls_reset > 0) return;
   queueSound(SOUND_BLAST);
   addPoints(500);
}



static void handleLDropTargetSeries()
{
   queueSound(SOUND_THROB);
   if (game_data.laser_score < 20000) {
      switch(game_data.laser_score) {
	 case 5000 :
	    lightOn(LIGHT_LASER_10K);
	    break;
	 case 10000 :
	    lightOff(LIGHT_LASER_10K);
	    lightOn(LIGHT_LASER_15K);
	    break;
	 case 15000 :
	    lightOff(LIGHT_LASER_15K);
	    lightOn(LIGHT_LASER_20K);
	    lightOn(LIGHT_LR_SPECIALS);
	    lightOn(LIGHT_SHOOT_AGAIN);
	    lightOn(LIGHT_SHOOT_AGAIN_BACK);
	    ++game_data.free_balls;
	    break;
       }
      game_data.laser_score += 5000;
    }
   resetLASERDropTargets();
   addPoints(game_data.laser_score);
}




static void handleBDropTargetSeries()
{
   int ct = game_data.ae_lamps;
   int score = 5000;
   if (ct > 0) score = ct * 10000;

   resetBALLDropTargets();
   addPoints(score);
}




/********************************************************************************/
/*										*/
/*	Credit management							*/
/*										*/
/********************************************************************************/

static void updateCredits()
{
   setDisplayLeft(game_data.game_credits);
}


static void addCredit()
{
   game_data.game_credits++;
   updateCredits();
   queueSolenoid(SOLENOID_CREDIT_KNOCKER);
}



/********************************************************************************/
/*										*/
/*	Player management							*/
/*										*/
/********************************************************************************/

static void addPlayer()
{
   int num = game_data.num_players;
   if (num >= NUM_PLAYER) return;
   if (game_data.game_credits > 0) {
      game_data.player[num].player_score = 0;
      game_data.player[num].player_balls = 3;
      game_data.num_players++;
      lightOn(LIGHT_PLAYER_1 + num);
      --game_data.game_credits;
      updateCredits();
      queueSound(SOUND_BLAST);
    }
}



static void updatePlayerInfo()
{
   int playerid = game_data.current_player;
   setDisplay(playerid, game_data.player[playerid].player_score);
   setDisplayRight(game_data.player[playerid].player_balls);
}



/********************************************************************************/
/*										*/
/*	Ball management 							*/
/*										*/
/********************************************************************************/

static void ballInPlay()
{
   game_data.in_play = true;
   lightOn(LIGHT_BALL_IN_PLAY);
}



static void ballOutOfPlay()
{
   game_data.in_play = false;
   lightOff(LIGHT_BALL_IN_PLAY);
}



static void releaseBall()
{
   queueSolenoid(SOLENOID_BALL_RELEASE);
   lightOn(LIGHT_SHOOT_AGAIN);
   lightOn(LIGHT_SHOOT_AGAIN_BACK);
   shoot_again_limit = addTime(micros(), LOGIC_SHOOT_AGAIN_TIME);
   ballInPlay();
}


static void tilt()
{
   game_data.player[game_data.current_player].player_score = 0;
   updatePlayerInfo();
   checkHighScore();
   ballOutOfPlay();
   lightOn(LIGHT_TILT);
   lightOff(LIGHT_BALL_IN_PLAY);
   game_data.is_tilt = true;
   game_data.free_balls = 0;
   disableShootAgain();
   specialDisable();
}



/********************************************************************************/
/*										*/
/*	High score methods							*/
/*										*/
/********************************************************************************/

static void readHighScore()
{
   long score = 0;
   EEPROM.get(LOGIC_HIGH_SCORE_ADDRESS, score);
   if (score <= 0) {
      score = 0;
      localWriteHighScore(score);
    }
   game_data.high_score = score;
   game_data.saved_high_score = score;
}


static void noteHighScore()
{
   if (game_data.high_score > game_data.saved_high_score) {
      localWriteHighScore(game_data.high_score);
      game_data.saved_high_score = game_data.high_score;
    }
}


static void resetHighScore()
{
   localWriteHighScore(0);
   game_data.high_score = 0;
   game_data.saved_high_score = 0;
}


static void localWriteHighScore(long score)
{
   EEPROM.put(LOGIC_HIGH_SCORE_ADDRESS, score);
}



static void checkHighScore()
{

   long score = game_data.player[game_data.current_player].player_score;
   if (score >= game_data.high_score) {
      game_data.high_score = score;
      lightOn(LIGHT_HIGH_SCORE);
    }
   else {
      lightOff(LIGHT_HIGH_SCORE);
    }
}




/********************************************************************************/
/*										*/
/*	Bit set methods 							*/
/*										*/
/********************************************************************************/

static bool insertStar(int s)
{
   int bit = (1 << s);
   if ((game_data.star_flags & bit) != 0) return false;
   game_data.star_flags |= bit;
   ++game_data.num_stars;
   return true;
}


static void resetStars()
{
   game_data.num_stars = 0;
   game_data.star_flags = 0;
}




static bool insertRollover(int r)
{
   int bit = (1 << r);
   if ((game_data.roll_flags & bit) != 0) return false;
   game_data.roll_flags |= bit;
   ++game_data.num_rolls;
   return true;
}



static void resetRollovers()
{
   game_data.num_rolls = 0;
   game_data.roll_flags = 0;
}



/* end of logic.ino */


