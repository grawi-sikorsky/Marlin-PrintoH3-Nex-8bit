/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Polish for DOGM display - includes accented characters
 */
//dodane:
#define MSG_PREHEAT_PRINTER					_UxGT("Rozgrzej")
#define MSG_PREHEAT_1                       LCD_STR_THERMOMETER _UxGT("Rozgrzej PLA")
#define MSG_PREHEAT_2                       LCD_STR_THERMOMETER _UxGT("Rozgrzej ABS")
#define MSG_PREHEAT_3						LCD_STR_THERMOMETER _UxGT("Rozgrzej PET-G")
#define MSG_PREHEAT_3_SETTINGS				_UxGT("Ustaw. rozg. PET-G")
#define MSG_ADVANCED						_UxGT("Zaawansowane")
#define MSG_MOVING                          _UxGT("Zaczekaj...")

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" gotowa.")
#define MSG_SD_INSERTED                     _UxGT("Karta w³o¿ona")
#define MSG_SD_REMOVED                      _UxGT("Karta usuniêta")
#define MSG_LCD_ENDSTOPS                    _UxGT("Kranców.") // Max length 8 characters
#define MSG_MAIN                            _UxGT("Menu g³ówne")
#define MSG_AUTOSTART                       _UxGT("Autostart")
#define MSG_DISABLE_STEPPERS                _UxGT("Wy³¹cz silniki")
#define MSG_AUTO_HOME                       _UxGT("Pozycja zerowa")
#define MSG_AUTO_HOME_X                     _UxGT("Bazuj X")
#define MSG_AUTO_HOME_Y                     _UxGT("Bazuj Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Bazuj Z")
#define MSG_LEVEL_BED                       _UxGT("Poziom. sto³u")
#define MSG_LEVEL_BED_HOMING                _UxGT("Pozycja zerowa")
#define MSG_LEVEL_BED_WAITING               _UxGT("Kliknij by rozp.")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Nastêpny punkt")
#define MSG_LEVEL_BED_DONE                  _UxGT("Wypoziomowano!")
#define MSG_SET_HOME_OFFSETS                _UxGT("Ust. poz. zer.")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Poz. zerowa ust.")
#define MSG_SET_ORIGIN                      _UxGT("Ustaw punkt zero"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" wsz.")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("Rozgrzej stó³ PLA")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("Ustaw. rozg. PLA")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" wsz.")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("Rozgrzej stó³ ABS")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("Ustaw. rozg. ABS")
#define MSG_COOLDOWN                        _UxGT("Ch³odzenie")
#define MSG_SWITCH_PS_ON                    _UxGT("W³¹cz zasilacz")
#define MSG_SWITCH_PS_OFF                   _UxGT("Wy³¹cz zasilacz")
#define MSG_EXTRUDE                         _UxGT("Ekstruzja")
#define MSG_RETRACT                         _UxGT("Wycofanie")
#define MSG_MOVE_AXIS                       _UxGT("Ruch osi")
#define MSG_BED_LEVELING                    _UxGT("Poziom. sto³u")
#define MSG_MOVE_X                          _UxGT("Przesuñ w X")
#define MSG_MOVE_Y                          _UxGT("Przesuñ w Y")
#define MSG_MOVE_Z                          _UxGT("Przesuñ w Z")
#define MSG_MOVE_E                          _UxGT("Ekstruzja (os E)")
#define MSG_MOVE_01MM                       _UxGT("Przesuñ co .1mm")
#define MSG_MOVE_1MM                        _UxGT("Przesuñ co 1mm")
#define MSG_MOVE_10MM                       _UxGT("Przesuñ co 10mm")
#define MSG_SPEED                           _UxGT("Predkoœæ")
#define MSG_BED_Z                           _UxGT("Stó³ Z")
#define MSG_NOZZLE                          _UxGT("Dysza")
#define MSG_BED                             _UxGT("Stó³")
#define MSG_FAN_SPEED                       _UxGT("Obroty wiatraka")
#define MSG_FLOW                            _UxGT("Przep³yw")
#define MSG_CONTROL                         _UxGT("Ustawienia")
#define MSG_MIN                             LCD_STR_THERMOMETER _UxGT(" Min")
#define MSG_MAX                             LCD_STR_THERMOMETER _UxGT(" Max")
#define MSG_FACTOR                          LCD_STR_THERMOMETER _UxGT(" Mno¿nik")
#define MSG_AUTOTEMP                        _UxGT("Auto. temperatura")
#define MSG_ON                              _UxGT("W³. ")
#define MSG_OFF                             _UxGT("Wy³.")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Select")
#define MSG_ACC                             _UxGT("Przyœpieszenie")
#define MSG_JERK                            _UxGT("Zryw")
#define MSG_VX_JERK                         _UxGT("Zryw Vx")
#define MSG_VY_JERK                         _UxGT("Zryw Vy")
#define MSG_VZ_JERK                         _UxGT("Zryw Vz")
#define MSG_VE_JERK                         _UxGT("Zryw Ve")
#define MSG_VELOCITY						_UxGT("Prêdkoœci")
#define MSG_VMAX                            _UxGT("Vmax ")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("Vskok min")
#define MSG_ACCELERATION                    MSG_ACC
#define MSG_AMAX                            _UxGT("Amax")
#define MSG_A_RETRACT                       _UxGT("A-wycofanie")
#define MSG_A_TRAVEL                        _UxGT("A-przesuñ.")
#define MSG_STEPS_PER_MM                    _UxGT("kroki/mm")
#define MSG_XSTEPS                          _UxGT("krokiX/mm")
#define MSG_YSTEPS                          _UxGT("krokiY/mm")
#define MSG_ZSTEPS                          _UxGT("krokiZ/mm")
#define MSG_ESTEPS                          _UxGT("krokiE/mm")
#define MSG_E1STEPS                         _UxGT("krokiE1/mm")
#define MSG_E2STEPS                         _UxGT("krokiE2/mm")
#define MSG_E3STEPS                         _UxGT("krokiE3/mm")
#define MSG_E4STEPS                         _UxGT("krokiE4/mm")
#define MSG_E5STEPS                         _UxGT("krokiE5/mm")
#define MSG_TEMPERATURE                     _UxGT("Temperatura")
#define MSG_MOTION                          _UxGT("Ruch")
#define MSG_FILAMENT                        _UxGT("Filament")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E w mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Œr. fil.")
#define MSG_CONTRAST                        _UxGT("Kontrast LCD")
#define MSG_STORE_EEPROM                    _UxGT("Zapisz w pamiêci")
#define MSG_LOAD_EEPROM                     _UxGT("Wczytaj z pamiêci")
#define MSG_RESTORE_FAILSAFE                _UxGT("Ustaw. fabryczne")
#define MSG_REFRESH                         _UxGT("Odswie¿")
#define MSG_WATCH                           _UxGT("Ekran g³ówny")
#define MSG_PREPARE                         _UxGT("Przygotuj")
#define MSG_TUNE                            _UxGT("Strojenie")
#define MSG_PAUSE_PRINT                     _UxGT("Pauza")
#define MSG_RESUME_PRINT                    _UxGT("Wznowienie")
#define MSG_STOP_PRINT                      _UxGT("Stop")
#define MSG_CARD_MENU                       LCD_STR_SD _UxGT("Karta SD")
#define MSG_NO_CARD                         _UxGT("Brak karty")
#define MSG_DWELL                           _UxGT("Uœpij...")
#define MSG_USERWAIT                        _UxGT("Oczekiwanie...")
#define MSG_RESUMING                        _UxGT("Wznawianie druku")
#define MSG_PRINT_ABORTED                   _UxGT("Druk przerwany")
#define MSG_NO_MOVE                         _UxGT("Brak ruchu")
#define MSG_KILLED                          _UxGT("Ubity. ")
#define MSG_STOPPED                         _UxGT("Zatrzymany. ")
#define MSG_CONTROL_RETRACT                 _UxGT("Wycofaj mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Z Wycof. mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Wycofaj  V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Skok Z mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("Cof. wycof. mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Z Cof. wyc. mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("Cof. wycof.  V")
#define MSG_AUTORETRACT                     _UxGT("Auto. wycofanie")
#define MSG_FILAMENTCHANGE                  _UxGT("Zmieñ filament")
#define MSG_INIT_SDCARD                     _UxGT("Inicjal. karty SD")
#define MSG_CNG_SDCARD                      _UxGT("Zmiana karty SD")
#define MSG_ZPROBE_OUT                      _UxGT("Sonda Z za sto³em")
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Self-Test")
#define MSG_BLTOUCH_RESET                   _UxGT("Reset BLTouch")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("first")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Offset Z")
#define MSG_BABYSTEP_X                      _UxGT("Babystep X")
#define MSG_BABYSTEP_Y                      _UxGT("Babystep Y")
#define MSG_BABYSTEP_Z                      _UxGT("Babystep Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("B³¹d krañcówki")
#define MSG_HEATING_FAILED_LCD              _UxGT("Rozgrz. nieudane")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("B³¹d temperatury")
#define MSG_THERMAL_RUNAWAY                 _UxGT("Zanik temp.")
#define MSG_ERR_MAXTEMP                     _UxGT("Err max temp")
#define MSG_ERR_MINTEMP                     _UxGT("Err min temp")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Err max temp sto³u")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Err min temp sto³u")
#define MSG_ERR_Z_HOMING                    _UxGT("G28 Z Forbidden")
#define MSG_HALTED                          _UxGT("Drukarka zatrzym.")
#define MSG_PLEASE_RESET                    _UxGT("Proszê zresetowaæ")
#define MSG_SHORT_DAY                       _UxGT("d") // One character only
#define MSG_SHORT_HOUR                      _UxGT("g") // One character only
#define MSG_SHORT_MINUTE                    _UxGT("m") // One character only
#define MSG_HEATING                         _UxGT("Rozgrzewanie...")
#define MSG_HEATING_COMPLETE                _UxGT("Rozgrzano")
#define MSG_BED_HEATING                     _UxGT("Rozgrzewanie sto³u...")
#define MSG_BED_DONE                        _UxGT("Rozgrzano stó³")
#define MSG_DELTA_CALIBRATE                 _UxGT("Kalibrowanie Delty")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Kalibruj X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Kalibruj Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Kalibruj Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Kalibruj œrodek")

#define MSG_INFO_MENU                       _UxGT("O drukarce")
#define MSG_INFO_PRINTER_MENU               _UxGT("Informacje")
#define MSG_INFO_STATS_MENU                 _UxGT("Statystyki")
#define MSG_INFO_BOARD_MENU                 _UxGT("Sterownik")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Termistory")
#define MSG_INFO_EXTRUDERS                  _UxGT("Ekstrudery")
#define MSG_INFO_BAUDRATE                   _UxGT("Predkoœæ USB")
#define MSG_INFO_PROTOCOL                   _UxGT("Protokó³")
#define MSG_CASE_LIGHT                      _UxGT("Oœwietlenie")

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Wydrukowano")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Ukoñczono")
  #define MSG_INFO_PRINT_TIME               _UxGT("Czas druku")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Najd³. druk")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("U¿yty filam.")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Wydrukowano")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Ukoñczono")
  #define MSG_INFO_PRINT_TIME               _UxGT("Razem")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Najd³. druk")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("U¿yty fil.")
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Min Temp")
#define MSG_INFO_MAX_TEMP                   _UxGT("Max Temp")
#define MSG_INFO_PSU                        _UxGT("Zasilacz")

#define MSG_DRIVE_STRENGTH                  _UxGT("Si³a silnika")
#define MSG_DAC_PERCENT                     _UxGT("Si³a %")
#define MSG_DAC_EEPROM_WRITE                _UxGT("Zapisz DAC EEPROM")

#define MSG_FILAMENT_CHANGE_HEADER          _UxGT("ZMIANA FILAMENTU")
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("WYBIERZ OPCJE:")
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  _UxGT("Ekstruduj wiêcej")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Wznów drukowanie")

//dodane Printo H3
#define MSG_PID_AUTOTUNE_START				_UxGT("PID rozpoczeto")
#define MSG_PID_AUTOTUNE_DONE				_UxGT("PID ukonczono")
#define MSG_PID_AUTOTUNE_FAIL				_UxGT("PID blad kalibracji")
#define MSG_PID_AUTOTUNE_OVERSHOT			_UxGT("PID blad: overshot")

#define MSG_PRINT_RECOVERING				_UxGT("Przywracanie wydruku")
#define MSG_INFO_VLCS_FAILED_PRINTS			_UxGT("Utraty zasilania")
#define MSG_INFO_PRINTO3D					_UxGT("Wsparcie")

#define MSG_FILAMENT_CHANGE_NOZZLE			_UxGT(" Dysza:")
#define	MSG_FILAMENT_CHANGE_MINTEMP			_UxGT("Min. Temp. to: ")
#define	MSG_BACK							_UxGT("Wstecz")
#define	MSG_FILAMENT_CHANGE_HEATING_1		_UxGT("Rozgrzewanie dyszy")
#define	MSG_FILAMENT_CHANGE_HEATING_2		_UxGT("Prosze czekac")
#define	MSG_FILAMENT_CHANGE_HEAT_1			_UxGT("Wcisnij przycisk")
#define	MSG_FILAMENT_CHANGE_HEAT_2			_UxGT("aby rozgrzac dysze")

// printo3d info menu
#define	MSG_INFO_P3D_1						_UxGT("W razie problemów")
#define	MSG_INFO_P3D_2						_UxGT("skontaktuj siê")
#define	MSG_INFO_P3D_3						_UxGT("z nami: ")
#define	MSG_INFO_P3D_MAIL					_UxGT("support@printo3d.pl")
#define	MSG_INFO_P3D_PHONE					_UxGT("+48 792 880 682")
#define	MSG_INFO_VLCS_SUPPORT				_UxGT("VLCS/Power Panic")
#define	MSG_INFO_FILAMENT_SENSOR_SUPPORT	_UxGT("Filament Sensor")
#define	MSG_INFO_YES											_UxGT("TAK")
#define	MSG_INFO_NO												_UxGT("NIE")

#define MSG_MESH_LEVELING                   _UxGT("M B L")
#define MSG_PRINT_PAUSED										_UxGT("Wydruk wstrzymany")


//#define									_UxGT("O nas")
//#define									_UxGT("O nas")


#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Czekam na ")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("zmianê filamentu")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Czekam na")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("wyjêcie filamentu")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("W³óz filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("i naciœnij przycisk")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("aby kontynuowaæ...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Czekam na")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("w³o¿enie filamentu")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Czekam na")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       _UxGT("ekstruzjê filamentu")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Czekam na")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("wznowienie druku")

#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Proszê czekaæ...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Wysuwanie...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("W³ó¿ i naciœnij prz.")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("£adowanie...")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Ekstruzja...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Wznowienie...")
#endif // LCD_HEIGHT < 4
