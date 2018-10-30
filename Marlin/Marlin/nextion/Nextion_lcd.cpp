/**
 * Nextion_lcd.cpp
 *
 * Copyright (c) 2014-2016 Alberto Cotronei @MagoKimbra
 *
 * Grbl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Grbl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Grbl. If not, see <http://www.gnu.org/licenses/>.
 */
#include "../Marlin.h"
#include "../temperature.h"
#include "../cardreader.h"
//#include "Arduino.h"
//#include "../MarlinConfig.h"

#if ENABLED(NEXTION_DISPLAY)
	#include "../stepper.h"
	#include "../mesh_bed_leveling.h"
	#include "../configuration_store.h"
#endif

#if ENABLED(NEXTION)

  #include "Nextion_lcd.h"
  #include "Nextion_gfx.h"
  #include "library/Nextion.h"

  bool        NextionON                 = false,
              show_Wave                 = true,
              lcdDrawUpdate             = false,
              lcd_clicked               = false;
  uint8_t     PageID                    = 0,
              lcd_status_message_level  = 0;
  uint16_t    slidermaxval              = 20;
  char        bufferson[70]             = { 0 };
  char        lcd_status_message[18]    = WELCOME_MSG;
  const float manual_feedrate_mm_m[]    = MANUAL_FEEDRATE;

  extern uint8_t progress_printing; // dodane nex

	extern float destination[XYZE];// = { 0.0 };
	extern bool g29_in_progress;// = false;
	extern inline void set_current_to_destination() { COPY(current_position, destination); }
	extern inline void set_destination_to_current() { COPY(destination, current_position); }

  #if ENABLED(SDSUPPORT)
    // 0 card not present, 1 SD not insert, 2 SD insert, 3 SD printing
    enum SDstatus_enum {NO_SD = 0, SD_NO_INSERT = 1, SD_INSERT = 2, SD_PRINTING = 3, SD_PAUSE = 4 };
    SDstatus_enum SDstatus    = NO_SD;
    //NexUpload Firmware(NEXTION_FIRMWARE_FILE, 57600);
  #endif
	#if ENABLED(BABYSTEPPING)
			int _babystep_z_shift = 0;
			long babysteps_done = 0;
			//void nextion_babystep_z(bool dir);
	#endif


  #if ENABLED(NEXTION_GFX)
    GFX gfx = GFX(1, 1, 1, 1);
  #endif
	
  /**
   *******************************************************************
   * Nextion component all page
   *******************************************************************
   */
  //NexObject Pstart        = NexObject(0,  0,  "start");
  //NexObject Pmenu         = NexObject(1,  0,  "menu");
  NexObject Pprinter      = NexObject(2,  0,  "printer");
  //NexObject Psdcard       = NexObject(3,  0,  "sdcard");
  NexObject Psetup        = NexObject(4,  0,  "setup");
  //NexObject Pmove         = NexObject(5,  0,  "move");
  //NexObject Pspeed        = NexObject(6,  0,  "speed");
  //NexObject Pgcode        = NexObject(7,  0,  "gcode");
  //NexObject Prfid         = NexObject(8,  0,  "rfid");
  //NexObject Pbrightness   = NexObject(9,  0,  "brightness");
  NexObject Pinfo         = NexObject(10, 0,  "info");
  NexObject Pyesno        = NexObject(11, 0,  "yesno");
  //NexObject Pfilament     = NexObject(12, 0,  "filament");
  NexObject Pselect       = NexObject(13, 0,  "select");
  NexObject Pprobe        = NexObject(14, 0,  "bedlevel");
	NexObject Pheatup				= NexObject(15, 0,	"heatup");
	//NexObject Poptions			= NexObject(16, 0,	"maintain");
  //NexObject Ptime         = NexObject(17, 0,  "infomove");
  NexObject Pfanspeedpage = NexObject(18, 0,  "fanspeedpage");
	//NexObject Pstats				= NexObject(19, 0,	"statscreen");
	//NexObject Ptsettings		= NexObject(20, 0,  "tempsettings");
	//NexObject Pinfobedlevel = NexObject(21, 0, "infobedlevel");
	//NexObject Pservice			= NexObject(22, 0, "servicepage");
	//NexObject Paccel				= NexObject(23, 0, "accelpage");
	//NexObject Pjerk					= NexObject(25, 0, "jerkpage");
	NexObject Pkill					= NexObject(30, 0, "killpage");
	
  /**
   *******************************************************************
   * Nextion component for page:start
   *******************************************************************
   */
  NexObject startimer     = NexObject(0,  1,  "tm0");

  /**
   *******************************************************************
   * Nextion component for page:menu
   *******************************************************************
   */
  NexObject Version     = NexObject(1, 6,  "t0");

  /**
   *******************************************************************
   * Nextion component for page:printer
   *******************************************************************
   */
  
  NexObject LcdX        = NexObject(2,  4,  "vx");
  NexObject LcdY        = NexObject(2,  5,  "vy");
  NexObject LcdZ        = NexObject(2,  6,  "vz");
  NexObject Extruders   = NexObject(2,  7,  "extruder");
  NexObject Hotend00    = NexObject(2,  8,  "he00");
  NexObject Hotend01    = NexObject(2,  9,  "he01");
  NexObject Hotend10    = NexObject(2, 10,  "he10");
  NexObject Hotend11    = NexObject(2, 11,  "he11");
  NexObject Bed0        = NexObject(2, 12,  "bed0");
  NexObject Bed1        = NexObject(2, 13,  "bed1");
  NexObject Chamber0    = NexObject(2, 14,  "cha0");
  NexObject Chamber1    = NexObject(2, 15,  "cha1");
  NexObject DHT0        = NexObject(2, 16,  "dht0");
  NexObject SD          = NexObject(2, 17,  "sd");
  //NexObject RFID        = NexObject(2, 18,  "rfid");
  //NexObject Fan         = NexObject(2, 19,  "fan");
  NexObject Fanspeed    = NexObject(2, 20,  "fs");
  NexObject VSpeed      = NexObject(2, 21,  "vs");
  NexObject Language    = NexObject(2, 22,  "lang");
  //NexObject LightStatus = NexObject(2, 23,  "light");
  NexObject NStop       = NexObject(2, 33,  "p1");
  NexObject NPlay       = NexObject(2, 34,  "p2");
  NexObject LcdStatus   = NexObject(2, 35,  "t0");
  //NexObject LcdCommand  = NexObject(2, 92,  "t1");
  NexObject LcdTime     = NexObject(2, 36,  "t2");
  NexObject progressbar = NexObject(2, 37,  "j0");
  NexObject Wavetemp    = NexObject(2, 38,  "s0");
	NexObject FanSpeedTouch = NexObject(2, 56, "m1");
  
	/**
	*******************************************************************
	* Nextion component for page:FanSPEEEED
	*******************************************************************
	*/

  /**
   *******************************************************************
   * Nextion component for page:SDCard
   *******************************************************************
   */
  NexObject sdlist      = NexObject(3,   1, "h0");
  NexObject sdrow0      = NexObject(3,   2, "t0");
  NexObject sdrow1      = NexObject(3,   3, "t1");
  NexObject sdrow2      = NexObject(3,   4, "t2");
  NexObject sdrow3      = NexObject(3,   5, "t3");
  NexObject sdrow4      = NexObject(3,   6, "t4");
  NexObject sdrow5      = NexObject(3,   7, "t5");
  NexObject Folder0     = NexObject(3,   8, "p0");
  NexObject Folder1     = NexObject(3,   9, "p1");
  NexObject Folder2     = NexObject(3,  10, "p2");
  NexObject Folder3     = NexObject(3,  11, "p3");
  NexObject Folder4     = NexObject(3,  12, "p4");
  NexObject Folder5     = NexObject(3,  13, "p5");
  NexObject Folderup    = NexObject(3,  14, "p6");
  NexObject sdfolder    = NexObject(3,  16, "t6");
  NexObject ScrollUp    = NexObject(3,  18, "p7");
  NexObject ScrollDown  = NexObject(3,  19, "p8");
  NexObject sd_mount    = NexObject(3,  21, "p12");
  NexObject sd_dismount = NexObject(3,  22, "p13");
  
  /**
   *******************************************************************
   * Nextion component for page:Setup
   *******************************************************************
   */

  /**
   *******************************************************************
   * Nextion component for page:Move
   *******************************************************************
   */
  NexObject XYHome      = NexObject(5,   2, "p4");
  NexObject XYUp        = NexObject(5,   3, "p5");
  NexObject XYRight     = NexObject(5,   4, "p6");
  NexObject XYDown      = NexObject(5,   5, "p7");
  NexObject XYLeft      = NexObject(5,   6, "p8");
  NexObject ZHome       = NexObject(5,   7, "p9");
  NexObject ZUp         = NexObject(5,   8, "p10");
  NexObject ZDown       = NexObject(5,   9, "p11");
  NexObject movecmd     = NexObject(5,  11, "vacmd");
  NexObject LedCoord5   = NexObject(5,  12, "t0");
  NexObject MotorOff    = NexObject(5,  17, "p0");
  NexObject ext         = NexObject(5,  18, "va0");
  NexObject Extrude     = NexObject(5,  19, "p12");
  NexObject Retract     = NexObject(5,  21, "p14");
  NexObject SpeedX      = NexObject(5,  22, "vafrx");
  NexObject SpeedY      = NexObject(5,  23, "vafry");
  NexObject SpeedZ      = NexObject(5,  24, "vafrz");
  NexObject SpeedE      = NexObject(5,  25, "vafre");
  
  /**
   *******************************************************************
   * Nextion component for page:Speed
   *******************************************************************
   */
  NexObject Speed       = NexObject(6,  1,  "h0");

  /**
   *******************************************************************
   * Nextion component for page:GCode
   *******************************************************************
   */
  NexObject Tgcode      = NexObject(7,   1, "tgcode");
  NexObject Send        = NexObject(7,  25, "bsend");

  /**
   *******************************************************************
   * Nextion component for page:Rfid
   *******************************************************************
   */
  //NexObject Rfid0       = NexObject(8,  2,  "b0");
  //NexObject Rfid1       = NexObject(8,  3,  "b1");
  //NexObject Rfid2       = NexObject(8,  4,  "b2");
  //NexObject Rfid3       = NexObject(8,  5,  "b3");
  //NexObject Rfid4       = NexObject(8,  6,  "b4");
  //NexObject Rfid5       = NexObject(8,  7,  "b5");
  //NexObject RfidText    = NexObject(8,  8,  "t0");
  //NexObject RfidR       = NexObject(8,  9,  "bt0");

  /**
   *******************************************************************
   * Nextion component for page:Brightness
   *******************************************************************
   */

  /**
   *******************************************************************
   * Nextion component for page:Temp
   *******************************************************************
   */

  /**
   *******************************************************************
   * Nextion component for page:Info
   *******************************************************************
   */
  NexObject InfoText    = NexObject(10, 2,  "t0");
  NexObject ScrollText  = NexObject(10, 3,  "g0");

  /**
   *******************************************************************
   * Nextion component for page:Yesno
   *******************************************************************
   */
  NexObject Vyes        = NexObject(11, 2,  "va0");
  NexObject Riga0       = NexObject(11, 4,  "t0");
  NexObject Riga1       = NexObject(11, 5,  "t1");
  NexObject Riga2       = NexObject(11, 6,  "t2");
  NexObject Riga3       = NexObject(11, 7,  "t3");
  NexObject Yes         = NexObject(11, 8,  "p1");
  NexObject No          = NexObject(11, 9,  "p2");

  /**
   *******************************************************************
   * Nextion component for page:Filament
   *******************************************************************
   */
  //NexObject FilLoad     = NexObject(12,  3, "p2");
  //NexObject FilUnload   = NexObject(12,  4, "p3");
  //NexObject FilExtr     = NexObject(12,  5, "p4");
  //NexObject Filgcode    = NexObject(12, 10, "vacmd");

  /**
   *******************************************************************
   * Nextion component for page:Select
   *******************************************************************
   */
  NexObject LcdRiga1    = NexObject(13,  1, "t0");
  NexObject LcdRiga2    = NexObject(13,  2, "t1");
  NexObject LcdRiga3    = NexObject(13,  3, "t2");
  NexObject LcdRiga4    = NexObject(13,  4, "t3");
  NexObject LcdValor    = NexObject(13,  5, "t4");
  NexObject LcdUp       = NexObject(13,  6, "p0");
  NexObject LcdSend     = NexObject(13,  7, "p1");
  NexObject LcdDown     = NexObject(13,  8, "p2");
  NexObject LcdMin      = NexObject(13,  9, "min");
  NexObject LcdMax      = NexObject(13, 10, "max");
  NexObject LcdPos      = NexObject(13, 11, "pos");

  /**
   *******************************************************************
   * Nextion component for page:Probe
   *******************************************************************
   */
  NexObject ProbeUp     = NexObject(14, 1,  "p0");
  NexObject ProbeSend   = NexObject(14, 2,  "p1");
  NexObject ProbeDown   = NexObject(14, 3,  "p2");
  //NexObject ProbeMsg    = NexObject(14, 4,  "t0");
  NexObject ProbeZ      = NexObject(14, 5,  "t1");

	/**
	*******************************************************************
	* Nextion component for page:heatup // dodane
	*******************************************************************
	*/

	NexObject heatupenter		= NexObject(15, 7, "m3");
	NexObject temphe				= NexObject(15, 8, "temphe");
	NexObject tempbe				= NexObject(15, 9, "tempbe");
	NexObject heatbedenter	= NexObject(15, 12, "m4");
	NexObject hotendenter		= NexObject(15, 13, "m5");
	NexObject chillenter		= NexObject(15, 14, "m6");


	/**
	*******************************************************************
	* Nextion component for page:maintain/osbluga
	*******************************************************************
	*/
	NexObject vpageid				= NexObject(16, 8, "vpageid");
	NexObject homeaxisbtn		= NexObject(16, 3, "m0");
	NexObject bedlevelbtn		= NexObject(16, 4, "m1");
	NexObject filchangebtn	= NexObject(16, 7, "m4");

	/**
	*******************************************************************
	* Nextion component for page:FAN SCREEN !
	*******************************************************************
	*/
	NexObject fansetbtn			= NexObject(18, 9, "m1");
	NexObject vfan					= NexObject(18, 7, "vfan");
	/**
	*******************************************************************
	* Nextion component for page:STAT SCREEN 19!
	*******************************************************************
	*/
	
	NexObject statin			= NexObject(4, 2, "m2"); //przycisk z innej strony -> setup
	NexObject Sprints			= NexObject(19, 3, "t0");
	NexObject Scompl			= NexObject(19, 4, "t1");
	NexObject Spanic			= NexObject(19, 5, "t2");
	NexObject Stimetotal	= NexObject(19, 6, "t3");
	NexObject Stimelong		= NexObject(19, 7, "t4");
	NexObject Sfilament		= NexObject(19, 8, "t5");

	NexObject Sfirmware		= NexObject(19, 10, "t6");
	NexObject Skompil			= NexObject(19, 11, "t7");
	NexObject Sleveling		= NexObject(19, 12, "t8");
	NexObject Svlcs				= NexObject(19, 13, "t9");
	NexObject Sfilsensor	= NexObject(19, 14, "t10");

	/**
	*******************************************************************
	* Nextion component for page:SERVICE PAGE 22 !
	*******************************************************************
	*/
	NexObject SvJerk				= NexObject(22, 4, "m2"); //wejscie w jerk -> przekazuje zmienne float na nuber nextion (brak dziesietnych)
	NexObject SvSteps				= NexObject(22, 5, "m3");	//wejscie w steps -> przekazuje zmienne float na nuber nextion (brak dziesietnych)

	/**
	*******************************************************************
	* Nextion component for page:ACCEL SCREEN 23!
	*******************************************************************
	*/
	NexObject accelin		= NexObject(4, 5, "m5");	// setaccelpagePopCallback -> przekazuje zmienne float do strony z akceleracja
	NexObject Awork			= NexObject(23, 22, "va0");
	NexObject Aretr			= NexObject(23, 23, "va1");
	NexObject Atravel		= NexObject(23, 24, "va2");
	NexObject Amaxx			= NexObject(23, 25, "va3");
	NexObject Amaxy			= NexObject(23, 26, "va4");
	NexObject Amaxz			= NexObject(23, 27, "va5");
	NexObject Amaxe			= NexObject(23, 28, "va6");
	NexObject Asend			= NexObject(23, 33, "p12"); 
	NexObject Asave			= NexObject(23, 30, "p10");	// setaccelsavebtnPopCallback -> wywo³uje settings.save();
	NexObject Aload			= NexObject(23, 31, "p11"); // setaccelloadbtnPopCallback	-> wywo³uje settings.load();

	/**
	*******************************************************************
	* Nextion component for page:JERK SCREEN 25!
	*******************************************************************
	*/
	//NexObject Jsend			= NexObject(23, 33, "p12");

	/**
	*******************************************************************
	* Nextion component for page:JERK SCREEN 28!
	*******************************************************************
	*/
	NexObject ZbabyUp			= NexObject(28, 1, "m0");
	NexObject ZbabyDown		= NexObject(28, 2, "m1");

	/**
	*******************************************************************
	* Nextion component for page:KILL SCREEN 29!
	*******************************************************************
	*/
	NexObject Kmsg				= NexObject(30, 7, "t0");


  NexObject *nex_listen_list[] =
  {
    // Page 2 touch listen
    &NPlay,

    // Page 3 touch listen
    &sdlist, &ScrollUp, &ScrollDown, &sdrow0, &sdrow1, &sdrow2,
    &sdrow3, &sdrow4, &sdrow5, &Folderup, &sd_mount, &sd_dismount,

    // Page 4 touch listen setup
		&statin, &accelin,

    // Page 5 touch listen
    &MotorOff, &XYHome, &XYUp, &XYRight, &XYDown, &XYLeft,
    &ZHome, &ZUp, &ZDown,
    &Extrude, &Retract,

    // Page 6 touch listen
    &Speed,

    // Page 7 touch listen
    &Send,

    // Page 8 touch listen
    //&Rfid0, &Rfid1, &Rfid2, &Rfid3, &Rfid4, &Rfid5,

    // Page 11 touch listen
    &Yes, &No,

    // Page 12 touch listen
    //&FilLoad, &FilUnload, &FilExtr,

    // Page 13 touch listen
    &LcdSend,

    // Page 14 touch listen
    &ProbeUp, &ProbeDown, &ProbeSend,

		// Page 15 tacz listen
		&heatupenter, &heatbedenter, &hotendenter, &chillenter,

		// Page 16 tacz listen
		&homeaxisbtn, &bedlevelbtn, &filchangebtn,

		// Page 18 tacz listen
		&fansetbtn,

		// Page 19 tacz listen

		// Page 23 tacz listen
		&Asend, &Asave, &Aload,

		// Page 24
		&SvSteps,

		// Page 25
		&SvJerk,

		// Page 28 babystep
		&ZbabyUp, &ZbabyDown,

    NULL
  };

  NexObject *lcd_row_list[] =
  {
    &LcdRiga1,
    &LcdRiga2,
    &LcdRiga3,
    &LcdRiga4,
    NULL
  };

  NexObject *heater_list0[] =
  {
    &Hotend00,
    &Hotend10,
    &Bed0,
    &Chamber0,
    &DHT0,
    NULL
  };

  NexObject *heater_list1[] =
  {
    &Hotend01,
    &Hotend11,
    &Bed1,
    &Chamber1,
    NULL
  };

  NexObject *row_list[] =
  {
    &sdrow0,
    &sdrow1,
    &sdrow2,
    &sdrow3,
    &sdrow4,
    &sdrow5,
    NULL
  };

  NexObject *folder_list[] =
  {
    &Folder0,
    &Folder1,
    &Folder2,
    &Folder3,
    &Folder4,
    &Folder5,
    NULL
  };

  NexObject *speed_list[] =
  {
    &SpeedX,
    &SpeedY,
    &SpeedZ,
    &SpeedE,
    NULL
  };

  // Function pointer to menu functions.
  typedef void (*screenFunc_t)();

  /**
   *
   * Menu actions
   *
   */

  void lcd_sdcard_stop() {
	  card.stopSDPrint();
	  clear_command_queue();
	  quickstop_stepper();
	  print_job_timer.stop();
	  thermalManager.disable_all_heaters();
		#if ENABLED(PLOSS_SUPPORT)
	  _babystep_z_shift = 0; // dodane - zeruje babystep po zatrzymaniu wydruku
	  eeprom_update_dword((uint32_t*)(EEPROM_PANIC_BABYSTEP_Z), _babystep_z_shift);	// zeruj babystepping w eeprom
		#endif
		#if FAN_COUNT > 0
	  for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = 0;
		#endif
	  wait_for_heatup = false;
	  lcd_setstatusPGM(PSTR(MSG_PRINT_ABORTED), -1);
}

  void menu_action_back() { Pprinter.show(); }
  void menu_action_function(screenFunc_t func) { (*func)(); }

  void setpagePrinter() {
    char temp[10] = { 0 };

    Version.setText(SHORT_BUILD_VERSION, "menu");

    #if HOTENDS > 0
      Hotend00.setValue(1, "printer");
      #if HOTENDS > 1
        Hotend10.setValue(1, "printer");
      #elif HAS_TEMP_CHAMBER
        Chamber0.setValue(1, "printer");
      #elif ENABLED(DHT_SENSOR)
        DHT0.setValue(1, "printer");
      #endif
    #endif

    #if HAS_TEMP_BED
      Bed0.setValue(1, "printer");
    #endif

    Extruders.setValue(EXTRUDERS, "printer");

    LOOP_XYZE(i) {
      ZERO(temp);
      itoa(manual_feedrate_mm_m[i], temp, 10);
      speed_list[i]->setText(temp, "move");
    }

    #if ENABLED(SDSUPPORT)
      if (!card.cardOK) card.initsd();
      delay(50);
      if (card.cardOK) {
        SDstatus = SD_INSERT;
        card.setroot();  // Initial boot
      }
      else
        SDstatus = SD_NO_INSERT;
      SD.setValue(SDstatus, "printer");
    #endif

    VSpeed.setValue(100, "printer");

    #if FAN_COUNT > 0
      //Fan.setValue(1, "printer");
    #endif

    #if HAS_CASE_LIGHT
      LightStatus.setValue(caselight.status ? 2 : 1, "printer");
    #endif

    #if ENABLED(RFID_MODULE)
      RFID.setValue(1, "printer");
    #endif

    #define LANGUAGE_STRING(M) STRINGIFY(M)
    #define NEXTION_LANGUAGE LANGUAGE_STRING(LCD_LANGUAGE)
    Language.setText(NEXTION_LANGUAGE, "printer");
  }

  void start_menu(const bool encoder=false, const bool push=false) {
    Pselect.show();
    LcdUp.SetVisibility(encoder);
    LcdDown.SetVisibility(encoder);
    LcdSend.SetVisibility(push);
    lcdDrawUpdate = true;
    lcd_clicked = !push;
  }

  /**
   * START_SCREEN  Opening code for a screen having only static items.s
   *               Do simplified scrolling of the entire screen.
   *
   * START_MENU    Opening code for a screen with menu items.
   *               Scroll as-needed to keep the selected line in view.
   */
  #define START_SCREEN() \
    start_menu(false, true); \
    do { \
      uint8_t _lcdLineNr = 0; \

  #define START_MENU() \
    start_menu(true, true); \
    uint16_t encoderLine; \
    uint8_t _lcdLineNr = 0; \
    do { \
      _lcdLineNr = 0; \
      encoderLine = LcdPos.getValue(); \
      delayMicroseconds(200)

  #define MENU_ITEM(TYPE, LABEL, ...) \
      if (lcdDrawUpdate) { \
        lcd_row_list[_lcdLineNr]->setText(LABEL); \
        LcdMax.setValue(_lcdLineNr); \
      } \
      if (lcd_clicked && encoderLine == _lcdLineNr) { \
        menu_action_ ## TYPE(__VA_ARGS__); \
        return; \
      } \
      ++_lcdLineNr

  #define MENU_BACK(LABEL) MENU_ITEM(back, LABEL)

  #define STATIC_ITEM_P(LABEL) \
      if (lcdDrawUpdate) { \
        lcd_row_list[_lcdLineNr]->setText(LABEL); \
        LcdMin.setValue(_lcdLineNr + 1); \
      } \
      ++_lcdLineNr \

  #define STATIC_ITEM(LABEL) STATIC_ITEM_P(LABEL)

  #define END_MENU() \
      idle(); \
      lcdDrawUpdate = false; \
    } while(1)

  #define END_SCREEN() \
      lcdDrawUpdate = false; \
    } while(0)

  #if ENABLED(SDSUPPORT)

	#if ENABLED(NEX_UPLOAD)
    void UploadNewFirmware() {
      if (IS_SD_INSERTED || card.cardOK()) {
        Firmware.startUpload();
        nexSerial.end();
        lcd_init();
      }
    }

	#endif
    void printrowsd(uint8_t row, const bool folder, const char* filename) {
      if (folder) {
        folder_list[row]->SetVisibility(true);
        row_list[row]->attachPop(sdfolderPopCallback, row_list[row]);
      } else if (filename == "") {
        folder_list[row]->SetVisibility(false);
        row_list[row]->detachPop();
      } else {
        folder_list[row]->SetVisibility(false);
        row_list[row]->attachPop(sdfilePopCallback, row_list[row]);
      }
      row_list[row]->setText(filename);
    }

    static void setrowsdcard(uint32_t number = 0) {
      uint16_t fileCnt = card.getnrfilenames(); // nalezaloby przeniesc funkcje get_num_files z mk4duo / jakis tweak
      uint32_t i = 0;
      card.getWorkDirName();

      if (card.filename[0] != '/') {
        Folderup.SetVisibility(true);
        Folderup.attachPop(sdfolderUpPopCallback);
        sdfolder.setText(card.filename);
      } else {
        Folderup.detachPop();
        Folderup.SetVisibility(false);
        sdfolder.setText("");
      }

      if (fileCnt) {
        for (uint8_t row = 0; row < 6; row++) {
          i = row + number;
          if (i < fileCnt) {
            #if ENABLED(SDCARD_SORT_ALPHA)
              card.getfilename_sorted(i);
            #else
              card.getfilename(i);
            #endif
            printrowsd(row, card.isFilenameIsDir(), card.filename); //card.isFilenameIsDir()
          } else {
            printrowsd(row, false, "");
          }
        }
      }
      sendCommand("ref 0");
    }

    static void menu_action_sdfile(const char* filename) {
      card.openAndPrintFile(filename);
      Pprinter.show();
    }

    static void menu_action_sddirectory(const char* filename) {
      card.chdir(filename);
      setpageSD();
    }

    void setpageSD() {
      uint16_t fileCnt = card.getnrfilenames();

      if (fileCnt <= 6)
        slidermaxval = 0;
      else
        slidermaxval  = fileCnt - 6;

      uint16_t hig = 210 - slidermaxval * 10;
      if (hig < 10) hig = 10;

      sdlist.Set_cursor_height_hig(hig);
      sdlist.setMaxval(slidermaxval);
      sdlist.setValue(slidermaxval);
      sendCommand("ref 0");

      setrowsdcard();
    }

    void sdmountdismountPopCallback(void *ptr) {
      if (ptr == &sd_mount) {
        card.initsd();
        if (card.cardOK)
          SDstatus = SD_INSERT;
        else
          SDstatus = SD_NO_INSERT;
        SD.setValue(SDstatus, "printer");
      }
      else {
        card.release();
        SDstatus = SD_NO_INSERT;
        SD.setValue(SDstatus, "printer");
      }
      setpageSD();
    }

    void sdlistPopCallback(void *ptr) {
      UNUSED(ptr);
      uint16_t number = slidermaxval - sdlist.getValue();
      setrowsdcard(number);
    }

    void sdfilePopCallback(void *ptr) {
      ZERO(bufferson);

      if (ptr == &sdrow0)
        sdrow0.getText(bufferson, sizeof(bufferson));
      else if (ptr == &sdrow1)
        sdrow1.getText(bufferson, sizeof(bufferson));
      else if (ptr == &sdrow2)
        sdrow2.getText(bufferson, sizeof(bufferson));
      else if (ptr == &sdrow3)
        sdrow3.getText(bufferson, sizeof(bufferson));
      else if (ptr == &sdrow4)
        sdrow4.getText(bufferson, sizeof(bufferson));
      else if (ptr == &sdrow5)
        sdrow5.getText(bufferson, sizeof(bufferson));

      menu_action_sdfile(bufferson);
    }

    void sdfolderPopCallback(void *ptr) {
      ZERO(bufferson);

      if (ptr == &sdrow0)
        sdrow0.getText(bufferson, sizeof(bufferson));
      else if (ptr == &sdrow1)
        sdrow1.getText(bufferson, sizeof(bufferson));
      else if (ptr == &sdrow2)
        sdrow2.getText(bufferson, sizeof(bufferson));
      else if (ptr == &sdrow3)
        sdrow3.getText(bufferson, sizeof(bufferson));
      else if (ptr == &sdrow4)
        sdrow4.getText(bufferson, sizeof(bufferson));
      else if (ptr == &sdrow5)
        sdrow5.getText(bufferson, sizeof(bufferson));

      menu_action_sddirectory(bufferson);
    }

    void sdfolderUpPopCallback(void *ptr) {
      UNUSED(ptr);
      card.updir();
      setpageSD();
    }

    void PlayPausePopCallback(void *ptr) {
      UNUSED(ptr);

      if (card.cardOK && card.isFileOpen()) {
        if (IS_SD_PRINTING) {
          card.pauseSDPrint();
          print_job_timer.pause();
          #if ENABLED(PARK_HEAD_ON_PAUSE)
            commands.enqueue_and_echo_P(PSTR("M125"));
          #endif
        }
        else {
          card.startFileprint();
          print_job_timer.start();
        }
      }
    }

  #endif

  #if ENABLED(ADVANCED_PAUSE_FEATURE)

    static AdvancedPauseMenuResponse advanced_pause_mode = ADVANCED_PAUSE_RESPONSE_WAIT_FOR;

    static const char* advanced_pause_header() {
      switch (advanced_pause_mode) {
        case ADVANCED_PAUSE_RESPONSE_EXTRUDE_MORE:
          return PSTR(MSG_FILAMENT_CHANGE_LOAD_1);
        case ADVANCED_PAUSE_RESPONSE_WAIT_FOR:
          return PSTR(MSG_FILAMENT_CHANGE_UNLOAD_1);
        default: break;
      }
      return PSTR(MSG_FILAMENT_CHANGE_HEADER); //MSG_FILAMENT_CHANGE_HEADER_PAUSE
    }

    static void lcd_advanced_pause_resume_print() {
      advanced_pause_menu_response = ADVANCED_PAUSE_RESPONSE_RESUME_PRINT;
      Pprinter.show();
    }

    static void lcd_advanced_pause_extrude_more() {
      advanced_pause_menu_response = ADVANCED_PAUSE_RESPONSE_EXTRUDE_MORE;
    }

    static void lcd_advanced_pause_option_menu() {
      START_MENU();
      STATIC_ITEM(MSG_NEX_FILAMENT_CHANGE_OPTION_HEADER);
      MENU_ITEM(function, MSG_FILAMENT_CHANGE_OPTION_RESUME, lcd_advanced_pause_resume_print);
      MENU_ITEM(function, MSG_FILAMENT_CHANGE_OPTION_EXTRUDE, lcd_advanced_pause_extrude_more);
      END_MENU();
    }

    static void lcd_advanced_pause_init_message() {
      START_SCREEN();
      STATIC_ITEM_P(advanced_pause_header());
      STATIC_ITEM(MSG_FILAMENT_CHANGE_INIT_1);
      STATIC_ITEM(MSG_FILAMENT_CHANGE_INIT_2);
      //STATIC_ITEM(MSG_FILAMENT_CHANGE_INIT_3);
      END_SCREEN();
    }

    static void lcd_advanced_pause_unload_message() {
      START_SCREEN();
      STATIC_ITEM_P(advanced_pause_header());
      STATIC_ITEM(MSG_FILAMENT_CHANGE_UNLOAD_1);
			STATIC_ITEM(MSG_FILAMENT_CHANGE_UNLOAD_2);
      //STATIC_ITEM(MSG_FILAMENT_CHANGE_UNLOAD_3);
      END_SCREEN();
    }

    static void lcd_advanced_pause_wait_for_nozzles_to_heat() {
      START_SCREEN();
      STATIC_ITEM_P(advanced_pause_header());
      STATIC_ITEM(MSG_FILAMENT_CHANGE_HEATING_1);
      STATIC_ITEM(MSG_FILAMENT_CHANGE_HEATING_2);
      END_SCREEN();
    }

    static void lcd_advanced_pause_heat_nozzle() {
      START_SCREEN();
      STATIC_ITEM_P(advanced_pause_header());
      STATIC_ITEM(MSG_FILAMENT_CHANGE_HEAT_1);
      STATIC_ITEM(MSG_FILAMENT_CHANGE_HEAT_2);
      END_SCREEN();
    }

    static void lcd_advanced_pause_printer_off() {
      START_SCREEN();
      STATIC_ITEM_P(advanced_pause_header());
      //STATIC_ITEM(MSG_FILAMENT_CHANGE_ZZZ_1);
      //STATIC_ITEM(MSG_FILAMENT_CHANGE_ZZZ_2);
      END_SCREEN();
    }

    static void lcd_advanced_pause_insert_message() {
      START_SCREEN();
      STATIC_ITEM_P(advanced_pause_header());
      STATIC_ITEM(MSG_FILAMENT_CHANGE_INSERT_1);
      STATIC_ITEM(MSG_FILAMENT_CHANGE_INSERT_2);
      STATIC_ITEM(MSG_FILAMENT_CHANGE_INSERT_3);
      END_SCREEN();
    }

    static void lcd_advanced_pause_load_message() {
      START_SCREEN();
      STATIC_ITEM_P(advanced_pause_header());
      STATIC_ITEM(MSG_FILAMENT_CHANGE_LOAD_1);
      STATIC_ITEM(MSG_FILAMENT_CHANGE_LOAD_2);
      //STATIC_ITEM(MSG_FILAMENT_CHANGE_LOAD_3);
      END_SCREEN();
    }

    static void lcd_advanced_pause_purge_message() {
      START_SCREEN();
      STATIC_ITEM_P(advanced_pause_header());
      STATIC_ITEM(MSG_FILAMENT_CHANGE_EXTRUDE_1);
      STATIC_ITEM(MSG_FILAMENT_CHANGE_EXTRUDE_2);
      //STATIC_ITEM(MSG_FILAMENT_CHANGE_PURGE_3);
      END_SCREEN();
    }

    static void lcd_advanced_pause_resume_message() {
      START_SCREEN();
      STATIC_ITEM_P(advanced_pause_header());
      STATIC_ITEM(MSG_FILAMENT_CHANGE_RESUME_1);
      STATIC_ITEM(MSG_FILAMENT_CHANGE_RESUME_2);
      //STATIC_ITEM(MSG_FILAMENT_CHANGE_RESUME_3);
      END_SCREEN();
    }

    void lcd_advanced_pause_show_message(
      const AdvancedPauseMessage message,
      const AdvancedPauseMenuResponse mode/*=ADVANCED_PAUSE_MODE_PAUSE_PRINT*/
    ) {

      //UNUSED(extruder);
      static AdvancedPauseMessage old_message;
      advanced_pause_mode = mode;

      if (old_message != message) {
        switch (message) {
          case ADVANCED_PAUSE_MESSAGE_INIT:
            lcd_advanced_pause_init_message();
            break;
          case ADVANCED_PAUSE_MESSAGE_UNLOAD:
            lcd_advanced_pause_unload_message();
            break;
          case ADVANCED_PAUSE_MESSAGE_INSERT:
            lcd_advanced_pause_insert_message();
            break;
          case ADVANCED_PAUSE_MESSAGE_LOAD:
            lcd_advanced_pause_load_message();
            break;
          case ADVANCED_PAUSE_MESSAGE_EXTRUDE:
            lcd_advanced_pause_purge_message();
            break;
          case ADVANCED_PAUSE_MESSAGE_RESUME:
            lcd_advanced_pause_resume_message();
            break;
          case ADVANCED_PAUSE_MESSAGE_CLICK_TO_HEAT_NOZZLE:
            lcd_advanced_pause_heat_nozzle();
            break;
          //case ADVANCED_PAUSE_MESSAGE_PRINTER_OFF:
            //lcd_advanced_pause_printer_off();
            //break;
          case ADVANCED_PAUSE_MESSAGE_WAIT_FOR_NOZZLES_TO_HEAT:
            lcd_advanced_pause_wait_for_nozzles_to_heat();
            break;
          case ADVANCED_PAUSE_MESSAGE_OPTION:
            advanced_pause_menu_response = ADVANCED_PAUSE_RESPONSE_WAIT_FOR;
            lcd_advanced_pause_option_menu();
            break;
          case ADVANCED_PAUSE_MESSAGE_STATUS:
          default:
            Pprinter.show();
            break;
        }

        old_message = message;
      }
    }

  #endif // ADVANCED_PAUSE_FEATURE

  #if ENABLED(RFID_MODULE)
    void rfidPopCallback(void *ptr) {
      ZERO(bufferson);

      String temp = "M522 ";
      uint16_t Rfid_read = RfidR.getValue();

      if (ptr == &Rfid0)
        temp += "T0 ";
      else if (ptr == &Rfid1)
        temp += "T1 ";
      else if (ptr == &Rfid2)
        temp += "T2 ";
      else if (ptr == &Rfid3)
        temp += "T3 ";
      else if (ptr == &Rfid4)
        temp += "T4 ";
      else if (ptr == &Rfid5)
        temp += "T5 ";

      if(Rfid_read)
        temp += "R";
      else
        temp += "W";

      temp.toCharArray(bufferson, sizeof(bufferson));
      commands.enqueue_and_echo(bufferson);
    }

    void rfid_setText(const char* message, uint32_t color /* = 65535 */) {
      char Rfid_status_message[25];
      strncpy(Rfid_status_message, message, 30);
      RfidText.Set_font_color_pco(color);
      RfidText.setText(Rfid_status_message);
    }
  #endif

#if ENABLED(NEXTION_BED_LEVEL)
    void ProbelPopCallBack(void *ptr) {

      if (ptr == &ProbeUp || ptr == &ProbeDown) {

				set_destination_to_current();

        if (ptr == &ProbeUp)
          destination[Z_AXIS] += (LCD_Z_STEP);
        else
          destination[Z_AXIS] -= (LCD_Z_STEP);

        NOLESS(destination[Z_AXIS], -(LCD_PROBE_Z_RANGE) * 0.5);
        NOMORE(destination[Z_AXIS], (LCD_PROBE_Z_RANGE) * 0.5);

        const float old_feedrate = feedrate_mm_s;
        feedrate_mm_s = MMM_TO_MMS(manual_feedrate_mm_m[Z_AXIS]);
        prepare_move_to_destination(); // will call set_current_from_destination()
        feedrate_mm_s = old_feedrate;

        stepper.synchronize();
      }
      else if (ptr == &ProbeSend) {
				SERIAL_ECHOLNPGM("probesend:");
        #if HAS_LEVELING && ENABLED(NEXTION_BED_LEVEL)
				if (g29_in_progress == true) {
					enqueue_and_echo_commands_P(PSTR("G29 S2")); 
				}
        #endif
					wait_for_user = false;
      }
    }

    float lcd_probe_pt(const float &lx, const float &ly) {

      #if HAS_LEVELING
				mbl.reset();// After calibration bed-level data is no longer valid
      #endif

      mbl.manual_goto_xy(lx, ly);

      Pprobe.show();

			host_keepalive();

			wait_for_user = true;

      while (wait_for_user == true) idle();
			host_keepalive();

      Pprinter.show();
      return current_position[Z_AXIS];
    }

    #if HAS_LEVELING
      void Nextion_ProbeOn()  { Pprobe.show(); }
      void Nextion_ProbeOff() { Pprinter.show(); }
    #endif

#endif


  void sethotPopCallback(void *ptr) {
    UNUSED(ptr);

		uint16_t	temp_hotend = temphe.getValue(), //dodane
							temp_bed = tempbe.getValue();    //dodane

		thermalManager.setTargetHotend(temp_hotend, 0);
		thermalManager.setTargetBed(temp_bed);

    Pprinter.show();
  }

	void sethotendPopCallback(void *ptr) {
		UNUSED(ptr);
		uint16_t	temp_hotend = temphe.getValue();
		thermalManager.setTargetHotend(temp_hotend, 0);
		Pprinter.show();
	}

	void setheatbedPopCallback(void *ptr) {
		UNUSED(ptr);
		uint16_t temp_bed = tempbe.getValue();    //dodane
		thermalManager.setTargetBed(temp_bed);
		Pprinter.show();
	}

	void setfanandgoPopCallback(void *ptr) {
		uint8_t pageidbuff, vfanbuff;
		UNUSED(ptr);
		ZERO(bufferson);
		vfanbuff = vfan.getValue("fanspeedpage");
		pageidbuff = vpageid.getValue("maintain");
		fanSpeeds[0] = vfanbuff;
		if (pageidbuff == 1) // wejscie z status
		{
			Pprinter.show();
		}
		else if (pageidbuff == 0) // wejscie z heatup
		{
			Pheatup.show();
		}
	}
	void setsetupstatPopCallback(void *ptr)
	{
		UNUSED(ptr);
			// PRINTSTATS START
			char buffer[21];
			printStatistics stats = print_job_timer.getStats();

			Sprints.setText(itostr3left(stats.totalPrints));        // Print Count: 999
			Scompl.setText(itostr3left(stats.finishedPrints));			// Completed  : 666

			#if ENABLED(PLOSS_SUPPORT)
				Spanic.setText(itostr3left(eeprom_read_byte((uint8_t*)EEPROM_PANIC_POWER_FAIL_COUNT))); // dodane power fail count
			#endif

			duration_t elapsed = stats.printTime;
			elapsed.toString(buffer);
			Stimetotal.setText(buffer);               // Total print Time: 99y 364d 23h 59m 59s

			elapsed = stats.longestPrint;
			elapsed.toString(buffer);
			Stimelong.setText(buffer);								// Longest job time: 99y 364d 23h 59m 59s

			sprintf_P(buffer, PSTR("%ld.%im"), long(stats.filamentUsed / 1000), int16_t(stats.filamentUsed / 100) % 10);
			Sfilament.setText(buffer);								// Extruded total: 125m
			// END OF PRINTSTATS

			// PRINTER INFO START
			Sfirmware.setText(SHORT_BUILD_VERSION);
			Skompil.setText(STRING_DISTRIBUTION_DATE);
			Sleveling.setText(MSG_MESH_LEVELING);

			#if ENABLED(PLOSS_SUPPORT)
						Svlcs.setText(MSG_INFO_YES);
			#else
						Svlcs.setText(MSG_INFO_NO);
			#endif

			#if ENABLED(FILAMENT_RUNOUT_SENSOR)
						Sfilsensor.setText(MSG_INFO_YES);
			#else
						Sfilsensor.setText(MSG_INFO_NO);
			#endif			
			// END OF PRINTER INFO
	}

	void setaccelpagePopCallback(void *ptr)
	{
			UNUSED(ptr);
			Awork.setValue(planner.acceleration); //va0
			Aretr.setValue(planner.retract_acceleration);	//va1
			Atravel.setValue(planner.travel_acceleration);
			Amaxx.setValue(planner.max_acceleration_mm_per_s2[X_AXIS]);
			Amaxy.setValue(planner.max_acceleration_mm_per_s2[Y_AXIS]);
			Amaxz.setValue(planner.max_acceleration_mm_per_s2[Z_AXIS]);
			Amaxe.setValue(planner.max_acceleration_mm_per_s2[E_AXIS+active_extruder]);
	}
	void setjerkpagePopCallback(void *ptr)
	{
			UNUSED(ptr);
			Awork.setValue(planner.max_jerk[X_AXIS], "accelpage"); //va0
			Aretr.setValue(planner.max_jerk[Y_AXIS], "accelpage");	//va1
			Atravel.setValue(planner.max_jerk[Z_AXIS],"accelpage"); //va2
			Amaxx.setValue(planner.max_jerk[E_AXIS],"accelpage");	//va3
	}

	void setstepspagePopCallback(void *ptr)
	{
			UNUSED(ptr);
			Awork.setValue(planner.axis_steps_per_mm[X_AXIS], "accelpage"); //va0
			Aretr.setValue(planner.axis_steps_per_mm[Y_AXIS], "accelpage");	//va1
			Atravel.setValue(planner.axis_steps_per_mm[Z_AXIS], "accelpage");	//va2
			Amaxx.setValue(planner.axis_steps_per_mm[E_AXIS+active_extruder], "accelpage");	//va3
	}
	void setaccelsavebtnPopCallback(void *ptr)
	{
		settings.save();
		SERIAL_ECHOPGM("zapisane");
	}
	void setaccelloadbtnPopCallback(void *ptr)
	{
		settings.load();
		SERIAL_ECHOPGM("zaladowane");
	}
	void setBabystepUpPopCallback(void *ptr)
	{
		nextion_babystep_z(true);
	}
	void setBabystepDownPopCallback(void *ptr)
	{
		nextion_babystep_z(false);
	}

  void setgcodePopCallback(void *ptr) {
    UNUSED(ptr);
    ZERO(bufferson);
    Tgcode.getText(bufferson, sizeof(bufferson), "gcode");
    Tgcode.setText("", "gcode");
		enqueue_and_echo_command(bufferson);
  }

  #if FAN_PIN
    void setfanPopCallback(void *ptr){
      UNUSED(ptr);
      fanSpeeds[0] = (fanSpeeds[0] ? 0 : 255);
			Fanspeed.setValue(((float)(fanSpeeds[0]) / 255) * 100);
    }
  #endif

  #if HAS_CASE_LIGHT
    void setlightPopCallback(void *ptr) {
      UNUSED(ptr);
      caselight.status = !caselight.status;
      caselight.update();
    }
  #endif

  void setmovePopCallback(void *ptr) {
    UNUSED(ptr);
    #if EXTRUDERS > 1
      const uint8_t temp_extruder = tools.active_extruder;
      char temp[5] = { 0 };

      ZERO(bufferson);
      itoa(ext.getValue(), temp, 2);
      strcat(bufferson, "T");
      strcat(bufferson, temp);
      commands.enqueue_and_echo(bufferson);
    #endif

    ZERO(bufferson);
		SERIAL_ECHO(bufferson);
    movecmd.getText(bufferson, sizeof(bufferson));
		SERIAL_ECHOLN(bufferson);
	enqueue_and_echo_commands_P(PSTR("G91"));
	enqueue_and_echo_command(bufferson);
	enqueue_and_echo_commands_P(PSTR("G90"));

    #if EXTRUDERS > 1
      ZERO(bufferson);
      itoa(temp_extruder, temp, 2);
      strcat(bufferson, "T");
      strcat(bufferson, temp);
      commands.enqueue_and_echo(bufferson);
    #endif
  }

  void motoroffPopCallback(void *ptr) {
    UNUSED(ptr);
		enqueue_and_echo_commands_P(PSTR("M84"));
  }

  void sendPopCallback(void *ptr) {
    UNUSED(ptr);
    lcd_clicked = true;
		wait_for_user = false; // dodane, cos pasuje z tym zrobic..
  }
	/*
  void filamentPopCallback(void *ptr) {
    ZERO(bufferson);
    Filgcode.getText(bufferson, sizeof(bufferson));
    if (ptr == &FilExtr)
	  enqueue_and_echo_command(bufferson);
    else {
	  enqueue_and_echo_commands_P(PSTR("G91"));
	  enqueue_and_echo_command(bufferson);
	  enqueue_and_echo_commands_P(PSTR("G90"));
    }
  }
	*/
  void YesNoPopCallback(void *ptr) {

    if (ptr == &Yes) {
      switch(Vyes.getValue()) {
        #if ENABLED(SDSUPPORT)
          case 1: // Stop Print
						lcd_sdcard_stop();
            Pprinter.show();
            break;
          case 2: // Upload Firmware
			#if ENABLED(NEX_UPLOAD)
            UploadNewFirmware(); 
			#endif
			break;
        #endif
        #if HAS_SD_RESTART
          case 3: // Restart file
            Pprinter.show();
            restart.start_job();
            break;
        #endif
        case 4: // Unconditional stop
          //printer.setWaitForUser(false);
          Pprinter.show();
          break;
        default: break;
      }
    }
    else {
      switch(Vyes.getValue()) {
        #if ENABLED(SDSUPPORT)
          case 2:
            Psetup.show(); break;
        #endif
        #if HAS_SD_RESTART
          case 3:
            card.printingHasFinished();
            Pprinter.show();
            break;
        #endif
        default:
          Pprinter.show(); break;
      }
    }
  }

  void lcd_init() {
    for (uint8_t i = 0; i < 10; i++) {
      ZERO(bufferson);
      NextionON = nexInit(bufferson);
      if (NextionON) break;
      delay(20);
    }
	//SERIAL_ECHOPGM("lcd_init za petla for");

    if (!NextionON) {
	  SERIAL_ECHOPGM("Nextion not connected!");
      return;
    }
    else {
		SERIAL_ECHO_START();
	  SERIAL_ECHOPGM("Nextion");
      // Get Model

      if (strstr(bufferson, "3224")) {       // Model 2.4" or 2.8" Normal or Enhanced
		SERIAL_ECHOPGM(" 2.4");
        #if ENABLED(NEXTION_GFX)
          gfx.set_position(1, 24, 250, 155);
        #endif
      }
      else if (strstr(bufferson, "4024")) {  // Model 3.2" Normal or Enhanced
		SERIAL_ECHOPGM(" 3.2");
        #if ENABLED(NEXTION_GFX)
          gfx.set_position(1, 24, 250, 155);
        #endif
      }
      else if (strstr(bufferson, "4827")) {  // Model 4.3" Normal or Enhanced
		SERIAL_ECHOPGM(" 4.3");
        #if ENABLED(NEXTION_GFX)
          gfx.set_position(1, 24, 250, 155);
        #endif
      }
      else if (strstr(bufferson, "8048")) {  // Model 7" Normal or Enhanced
		SERIAL_ECHOPGM(" 7");
        #if ENABLED(NEXTION_GFX)
          gfx.set_position(274, 213, 250, 155);
        #endif
      }
	  SERIAL_CHAR('"'); SERIAL_ECHOLNPGM(" connected!");

      #if ENABLED(NEXTION_GFX)
        gfx.color_set(NX_AXIS + X_AXIS, 63488);
        gfx.color_set(NX_AXIS + Y_AXIS, 2016);
        gfx.color_set(NX_AXIS + Z_AXIS, 31);
        gfx.color_set(NX_MOVE, 2047);
        gfx.color_set(NX_TOOL, 65535);
        gfx.color_set(NX_LOW, 2047);
        gfx.color_set(NX_HIGH, 63488);
      #endif

      #if ENABLED(SDSUPPORT)
        sd_mount.attachPop(sdmountdismountPopCallback, &sd_mount);
        sd_dismount.attachPop(sdmountdismountPopCallback, &sd_dismount);
        sdlist.attachPop(sdlistPopCallback);
        ScrollUp.attachPop(sdlistPopCallback);
        ScrollDown.attachPop(sdlistPopCallback);
        NPlay.attachPop(PlayPausePopCallback);
      #endif

      #if ENABLED(RFID_MODULE)
        Rfid0.attachPop(rfidPopCallback,  &Rfid0);
        Rfid1.attachPop(rfidPopCallback,  &Rfid1);
        Rfid2.attachPop(rfidPopCallback,  &Rfid2);
        Rfid3.attachPop(rfidPopCallback,  &Rfid3);
        Rfid4.attachPop(rfidPopCallback,  &Rfid4);
        Rfid5.attachPop(rfidPopCallback,  &Rfid5);
      #endif

      #if FAN_COUNT > 0
				FanSpeedTouch.attachPop(setfanPopCallback, &FanSpeedTouch);
      #endif

      #if HAS_CASE_LIGHT
        Light.attachPop(setlightPopCallback, &Light);
      #endif

      #if ENABLED(NEXTION_BED_LEVEL)
        ProbeUp.attachPush(ProbelPopCallBack, &ProbeUp);
        ProbeSend.attachPop(ProbelPopCallBack, &ProbeSend);
        ProbeDown.attachPush(ProbelPopCallBack, &ProbeDown);
      #endif

			heatupenter.attachPop(sethotPopCallback, &heatupenter); // obs³uga przycisku rozgrzej oba
			hotendenter.attachPop(sethotendPopCallback, &hotendenter); //obs³uga przycisku rozgrzej hotend
			heatbedenter.attachPop(setheatbedPopCallback, &heatbedenter); //obs³uga przycisku rozgrzej bed
			chillenter.attachPop(sethotPopCallback, &chillenter); //obs³uga przycisku chlodzenie
			homeaxisbtn.attachPop(setgcodePopCallback); //obs³uga przycisku home
			bedlevelbtn.attachPop(setgcodePopCallback); //obs³uga przycisku level
			filchangebtn.attachPop(setgcodePopCallback); //obs³uga przycisku m600
			statin.attachPop(setsetupstatPopCallback); //dodane info o wejsciu w statystyki
			accelin.attachPop(setaccelpagePopCallback); //setaccelpagePopCallback
			Asend.attachPop(setgcodePopCallback);
			Asave.attachPop(setaccelsavebtnPopCallback);
			Aload.attachPop(setaccelloadbtnPopCallback);
			SvJerk.attachPop(setjerkpagePopCallback);
			SvSteps.attachPop(setstepspagePopCallback);
			ZbabyUp.attachPush(setBabystepUpPopCallback);	// obsluga przycisku babystep up
			ZbabyDown.attachPush(setBabystepDownPopCallback); // obsluga przycisku babystep down
			fansetbtn.attachPop(setfanandgoPopCallback); //obs³uga przycisku fan set
      XYHome.attachPop(setmovePopCallback);
			XYUp.attachPush(setmovePopCallback); // dodane
      XYRight.attachPush(setmovePopCallback);
      XYDown.attachPush(setmovePopCallback);
      XYLeft.attachPush(setmovePopCallback);
      ZHome.attachPop(setmovePopCallback);
      ZUp.attachPush(setmovePopCallback);
      ZDown.attachPush(setmovePopCallback);
      Extrude.attachPush(setmovePopCallback);
      Retract.attachPush(setmovePopCallback);
      MotorOff.attachPop(motoroffPopCallback);
      Send.attachPop(setgcodePopCallback);
      Yes.attachPop(YesNoPopCallback, &Yes);
      No.attachPop(YesNoPopCallback, &No);
      LcdSend.attachPop(sendPopCallback);

      setpagePrinter();
      startimer.enable();
    }
  }

  bool lcd_detected() { return NextionON; }

  static void degtoLCD(const uint8_t h, float temp) {

    NOMORE(temp, 300);//999

    heater_list0[h]->setValue(temp);

    #if ENABLED(NEXTION_GFX)
      if (!(print_job_counter.isRunning() || IS_SD_PRINTING) && !Wavetemp.getObjVis() && show_Wave) {
        Wavetemp.SetVisibility(true);
      }
    #endif
  }

  static void targetdegtoLCD(const uint8_t h, const float temp) {
    heater_list1[h]->setValue(temp);
  }

  static void coordtoLCD() {
    char* valuetemp;
    ZERO(bufferson);

    if (PageID == 2) {
      LcdX.setText(ftostr41sign(LOGICAL_X_POSITION(current_position[X_AXIS])));
      LcdY.setText(ftostr41sign(LOGICAL_Y_POSITION(current_position[Y_AXIS])));
      LcdZ.setText(ftostr41sign(FIXFLOAT(LOGICAL_Z_POSITION(current_position[Z_AXIS]))));
    }

    else if (PageID == 5) {
      if (axis_homed[X_AXIS]) {
        valuetemp = ftostr4sign(LOGICAL_X_POSITION(current_position[X_AXIS]));
        strcat(bufferson, "X");
        strcat(bufferson, valuetemp);
      }
      else
        strcat(bufferson, "?");

      if (axis_homed[Y_AXIS]) {
        valuetemp = ftostr4sign(LOGICAL_Y_POSITION(current_position[Y_AXIS]));
        strcat(bufferson, " Y");
        strcat(bufferson, valuetemp);
      }
      else
        strcat(bufferson, " ?");

      if (axis_homed[Z_AXIS]) {
        valuetemp = ftostr52sp(FIXFLOAT(LOGICAL_Z_POSITION(current_position[Z_AXIS])));
        strcat(bufferson, " Z");
        strcat(bufferson, valuetemp);
      }
      else
        strcat(bufferson, " ?");

      LedCoord5.setText(bufferson);
    }
    else if (PageID == 14) {
      ProbeZ.setText(ftostr43sign(FIXFLOAT(LOGICAL_Z_POSITION(current_position[Z_AXIS]))));
    }
  }

  void lcd_update() {
    if (!NextionON) return;
    nexLoop(nex_listen_list);
  }

  void nextion_draw_update() {

    static uint8_t  PreviousPage = 0,
                    Previousfeedrate = 0,
                    PreviousfanSpeed = 0,
                    PreviouspercentDone = 0;
    static float    PreviousdegHeater[3] = { 0.0 },
                    PrevioustargetdegHeater[3] = { 0.0 };

    if (!NextionON) return;
	
    PageID = Nextion_PageID();

    switch(PageID)
	{
      case 2:
        if (PreviousPage != 2) {
          lcd_setstatus(lcd_status_message);
          #if ENABLED(NEXTION_GFX)
            #if MECH(DELTA)
              gfx_clear(mechanics.delta_print_radius * 2, mechanics.delta_print_radius * 2, mechanics.delta_height);
            #else
              gfx_clear(X_MAX_POS, Y_MAX_POS, Z_MAX_POS);
            #endif
          #endif
		}
        #if FAN_COUNT > 0
          if (PreviousfanSpeed != fanSpeeds[0]) {
          Fanspeed.setValue(((float)(fanSpeeds[0]) / 255) * 100);
          PreviousfanSpeed = fanSpeeds[0];
          }
        #endif
        #if HAS_CASE_LIGHT
          LightStatus.setValue(caselight.status ? 2 : 1);
        #endif
        if (Previousfeedrate != feedrate_percentage) {
          VSpeed.setValue(feedrate_percentage);
          Previousfeedrate = feedrate_percentage;
        }
        #if HAS_TEMP_0
          if (PreviousdegHeater[0] != thermalManager.current_temperature[0]) 
		  {
			  PreviousdegHeater[0] = thermalManager.current_temperature[0];
            degtoLCD(0, PreviousdegHeater[0]);
          }
          if (PrevioustargetdegHeater[0] != thermalManager.target_temperature[0]) 
		  {
			  PrevioustargetdegHeater[0] = thermalManager.target_temperature[0];
            targetdegtoLCD(0, PrevioustargetdegHeater[0]);
          }
        #endif
        #if HAS_TEMP_1
          if (PreviousdegHeater[1] != thermalManager.current_temperature[1]) {
            PreviousdegHeater[1] = thermalManager.current_temperature[1];
            degtoLCD(1, PreviousdegHeater[1]);
          }
          if (PrevioustargetdegHeater[1] != thermalManager.current_temperature[1]) {
            PrevioustargetdegHeater[1] = thermalManager.current_temperature[1];
            targetdegtoLCD(1, PrevioustargetdegHeater[1]);
          }
		  
        #elif HAS_TEMP_CHAMBER
          if (PreviousdegHeater[1] != heaters[CHAMBER_INDEX].current_temperature) {
            PreviousdegHeater[1] = heaters[CHAMBER_INDEX].current_temperature;
            degtoLCD(3, PreviousdegHeater[1]);
          }
          if (PrevioustargetdegHeater[1] != heaters[CHAMBER_INDEX].target_temperature) {
            PrevioustargetdegHeater[1] = heaters[CHAMBER_INDEX].target_temperature;
            targetdegtoLCD(3, PrevioustargetdegHeater[1]);
          }
        #elif ENABLED(DHT_SENSOR)
          if (PreviousdegHeater[1] != dhtsensor.Humidity) {
            PreviousdegHeater[1] = dhtsensor.Humidity;
            degtoLCD(4, PreviousdegHeater[1]);
          }
        #endif
        #if HAS_TEMP_BED
          if (PreviousdegHeater[2] != thermalManager.current_temperature_bed) {
            PreviousdegHeater[2] = thermalManager.current_temperature_bed;
            degtoLCD(2, PreviousdegHeater[2]);
          }
          if (PrevioustargetdegHeater[2] != thermalManager.target_temperature_bed) {
            PrevioustargetdegHeater[2] = thermalManager.target_temperature_bed;
            targetdegtoLCD(2, PrevioustargetdegHeater[2]);
          }
        #endif
        coordtoLCD();

		if (PreviouspercentDone != progress_printing) {
			// Progress bar solid part
			progressbar.setValue(progress_printing);
			// Estimate End Time
			ZERO(bufferson);
			char buffer1[10];
			uint8_t digit;
			duration_t Time = print_job_timer.duration();
			digit = Time.toDigital(buffer1, true);
			strcat(bufferson, "S");
			strcat(bufferson, buffer1);
			Time = (print_job_timer.duration() * (100 - progress_printing)) / (progress_printing + 0.1);
			digit += Time.toDigital(buffer1, true);
			if (digit > 14)
				strcat(bufferson, "E");
			else
				strcat(bufferson, " E");
			strcat(bufferson, buffer1);
			LcdTime.setText(bufferson);
			PreviouspercentDone = progress_printing;
	}

		#if ENABLED(SDSUPPORT)
		if (card.isFileOpen()) {
			if (IS_SD_PRINTING && SDstatus != SD_PRINTING) {
				SDstatus = SD_PRINTING;
				SD.setValue(SDstatus);
			}
			else if (!IS_SD_PRINTING && SDstatus != SD_PAUSE) {
				SDstatus = SD_PAUSE;
				SD.setValue(SDstatus);
			}
		}
		else if (card.cardOK && SDstatus != SD_INSERT) {
			SDstatus = SD_INSERT;
			SD.setValue(SDstatus);
		}
		else if (card.cardOK && SDstatus != SD_NO_INSERT) {
			SDstatus = SD_NO_INSERT;
			SD.setValue(SDstatus);
		}
		#endif // HAS_SD_SUPPORT

		#if HAS_SD_RESTART
          if (restart.count && restart.job_phase == RESTART_IDLE) {
            restart.job_phase = RESTART_MAYBE; // Waiting for a response
            lcd_yesno(3, MSG_RESTART_PRINT, "", MSG_USERWAIT);
          }
        #endif 
        break;
	#if ENABLED(SDSUPPORT)
      case 3:
          if (PreviousPage != 3) setpageSD();
          break;
	#endif
      case 5:
        coordtoLCD();
        break;
      case 6:
        Previousfeedrate = feedrate_percentage = (int)VSpeed.getValue("printer");
        break;
      case 14:
        coordtoLCD();
        break;
    }

    PreviousPage = PageID;
  }

  void lcd_setstatus(const char* message, bool persist) {
    UNUSED(persist);
    if (lcd_status_message_level > 0 || !NextionON) return;
    strncpy(lcd_status_message, message, 30);
    if (PageID == 2) LcdStatus.setText(lcd_status_message);
  }

  void lcd_setstatusPGM(const char* message, int8_t level) {
    if (level < 0) level = lcd_status_message_level = 0;
    if (level < lcd_status_message_level || !NextionON) return;
    strncpy_P(lcd_status_message, message, 30);
    lcd_status_message_level = level;
    if (PageID == 2) LcdStatus.setText(lcd_status_message);
  }

  void lcd_status_printf_P(const uint8_t level, const char * const fmt, ...) {
    if (level < lcd_status_message_level || !NextionON) return;
    lcd_status_message_level = level;
    va_list args;
    va_start(args, fmt);
    vsnprintf(lcd_status_message, 30, fmt, args);
    va_end(args);
    if (PageID == 2) LcdStatus.setText(lcd_status_message);
  }

  void lcd_setalertstatusPGM(const char * const message) {
    lcd_setstatusPGM(message, 1);
  }

  void lcd_reset_alert_level() { lcd_status_message_level = 0; }

  void lcd_scrollinfo(const char* titolo, const char* message) {
    Pinfo.show();
    InfoText.setText(titolo);
    ScrollText.setText(message);
  }

  void lcd_yesno(const uint8_t val, const char* msg1, const char* msg2, const char* msg3) {
    Vyes.setValue(val, "yesno");
    Pyesno.show();
    Riga0.setText(msg1);
    Riga1.setText(msg2);
    Riga3.setText(msg3);
  }

	void lcd_nextion_kill_msg(const char* lcd_msg)
	{
		Kmsg.setText(lcd_msg);
		Pkill.show();
	}

	// dodana obsluga babystep
	#if ENABLED(BABYSTEPPING)
		void nextion_babystep_z(bool dir) {

			#if ENABLED(PLOSS_SUPPORT)
				_babystep_z_shift += babysteps_done; // ploss
				eeprom_update_dword((uint32_t*)(EEPROM_PANIC_BABYSTEP_Z), _babystep_z_shift); // debug zapisz w eeprom
			#endif
				const int16_t babystep_increment = 8;

				if (dir == true)
				{
					thermalManager.babystep_axis(Z_AXIS, babystep_increment);
					babysteps_done += babystep_increment;
				}
				else if (dir == false)
				{
					thermalManager.babystep_axis(Z_AXIS, -babystep_increment);
					babysteps_done -= babystep_increment;
				}
		}
	#endif


  #if ENABLED(NEXTION_GFX)
    void gfx_origin(const float x, const float y, const float z) {
      gfx.origin(x, y, z);
    }

    void gfx_scale(const float scale) {
      gfx.set_scale(scale);
    }

    void gfx_clear(const float x, const float y, const float z, bool force_clear) {
      if (PageID == 2 && (print_job_counter.isRunning() || IS_SD_PRINTING || force_clear)) {
        Wavetemp.SetVisibility(false);
        show_Wave = !force_clear;
        gfx.clear(x, y, z);
      }
    }

    void gfx_cursor_to(const float x, const float y, const float z, bool force_cursor) {
      if (PageID == 2 && (print_job_counter.isRunning() || IS_SD_PRINTING || force_cursor))
        gfx.cursor_to(x, y, z);
    }

    void gfx_line_to(const float x, const float y, const float z) {
      if (PageID == 2 && (print_job_counter.isRunning() || IS_SD_PRINTING)) {
        #if ENABLED(ARDUINO_ARCH_SAM)
          gfx.line_to(NX_TOOL, x, y, z, true);
        #else
          gfx.line_to(NX_TOOL, x, y, z);
        #endif
      }
    }

    void gfx_plane_to(const float x, const float y, const float z) {
      uint8_t color;
      if (PageID == 2) {
        if (z < 10) color = NX_LOW;
        else color = NX_HIGH;
        gfx.line_to(color, x, y, z, true);
      }
    }
  #endif

#endif
