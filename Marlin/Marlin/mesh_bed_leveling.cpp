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

#include "mesh_bed_leveling.h"

#if ENABLED(MESH_BED_LEVELING)

  mesh_bed_leveling mbl;

  uint8_t mesh_bed_leveling::status;

  float mesh_bed_leveling::z_offset,
        mesh_bed_leveling::z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y],
        mesh_bed_leveling::index_to_xpos[GRID_MAX_POINTS_X],
        mesh_bed_leveling::index_to_ypos[GRID_MAX_POINTS_Y];

  mesh_bed_leveling::mesh_bed_leveling() {
    for (uint8_t i = 0; i < GRID_MAX_POINTS_X; ++i)
      index_to_xpos[i] = MESH_MIN_X + i * (MESH_X_DIST);
    for (uint8_t i = 0; i < GRID_MAX_POINTS_Y; ++i)
      index_to_ypos[i] = MESH_MIN_Y + i * (MESH_Y_DIST);
    reset();
  }

  void mesh_bed_leveling::reset() {
    status = MBL_STATUS_NONE;
    z_offset = 0;
    ZERO(z_values);
  }


	#if ENABLED(MESH_BED_LEVELING) || ENABLED(PROBE_MANUALLY)

		void mesh_bed_leveling::manual_goto_xy(const float &rx, const float &ry) {

	#if MANUAL_PROBE_HEIGHT > 0
			const float prev_z = current_position[Z_AXIS];
			do_blocking_move_to(rx, ry, MANUAL_PROBE_HEIGHT);
			do_blocking_move_to_z(prev_z);
	#else
			do_blocking_move_to_xy(rx, ry);
	#endif

			current_position[X_AXIS] = rx;
			current_position[Y_AXIS] = ry;

	#if ENABLED(LCD_BED_LEVELING)
			//lcd_wait_for_move = false;
	#endif

	}

#endif

#endif // MESH_BED_LEVELING
