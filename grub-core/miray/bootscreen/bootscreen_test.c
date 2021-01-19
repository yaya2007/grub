/*
 *  Extention for GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2010,2011 Miray Software <oss@miray.de>
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <grub/dl.h>
#include <grub/misc.h>
#include <grub/command.h>
#include <grub/i18n.h>
#include <grub/term.h>
#include <grub/time.h>

#include "miray_bootscreen.h"
#include "miray_logo.h"
#include "miray_screen.h"
#include "text_progress_bar.h"

GRUB_MOD_LICENSE ("GPLv3+");


#define TEST_NUM_BARS 7

#define TEST_NUM_BARS 7

static grub_err_t
miray_test_activity_bars (grub_command_t cmd __attribute__ ((unused)),
		int argc __attribute__ ((unused)), char **args __attribute__ ((unused)) )
{
   unsigned int i, c;
   const grub_uint32_t step_sleep_ms = 50; // 
   const grub_uint32_t steps_per_sec = 1000 / step_sleep_ms;
   const unsigned int sec = 20;   
   struct text_bar * bars[TEST_NUM_BARS];

   bars[0] = text_activity_bar_new_verbose(grub_term_outputs, 2, 5, 20, 0x2592, 0x2592, "dark-gray/black", "blue/black");
   bars[1] = text_activity_bar_new_verbose(grub_term_outputs, 2, 7, 20, 0x2591, 0x2591, "dark-gray/black", "blue/black");
   bars[2] = text_activity_bar_new_verbose(grub_term_outputs, 2, 9, 20, 0x2588, 0x2588, "dark-gray/black", "blue/black");
   bars[3] = text_activity_bar_new_verbose(grub_term_outputs, 2, 11, 20, 0x2591, 0x2591, "light-gray/black", "light-blue/black");
   bars[4] = text_activity_bar_new_verbose(grub_term_outputs, 2, 13, 20, 0x2588, 0x2588, "light-gray/black", "light-blue/black");
   bars[5] = text_activity_bar_new_verbose(grub_term_outputs, 2, 15, 20, 0x2591, 0x2588, "light-gray/black", "light-blue/black");
   bars[6] = text_activity_bar_new_verbose(grub_term_outputs, 2, 17, 20, 0x2591, 0x2588, "light-gray/black", "blue/black");


   for (i = 0; i < sec * steps_per_sec; ++i)
   {
      grub_millisleep (step_sleep_ms);
      for (c = 0; c < TEST_NUM_BARS; ++c)
      {
         if (bars[c] != 0) text_activity_bar_advance(bars[c]);
      }
   }


   for (c = 0; c < TEST_NUM_BARS; ++c)
   {
      text_bar_destroy (bars[c]);
   }

   return 0;

}


static grub_err_t
miray_test_screen (grub_command_t cmd __attribute__ ((unused)),
                int argc, char **args)
{
  unsigned int sec;

  sec = 3;

  if (argc > 0)
    sec = grub_strtoul (args[0], 0, 10);

  {
    grub_err_t ret = miray_screen_init();

    if(ret != GRUB_ERR_NONE) return ret;
  }

  miray_screen_redraw();

  grub_sleep(sec);

  miray_screen_fini();

  return 0;
}

static grub_err_t
miray_test_redraw (grub_command_t cmd __attribute__ ((unused)),
                int argc __attribute__ ((unused)),
                char **args __attribute__((unused)))
{
  miray_screen_redraw();

  return GRUB_ERR_NONE;
}



static grub_command_t /* cmd_logo,*/ cmd_bar, cmd_screen, cmd_redraw;

GRUB_MOD_INIT(miray_bootscreen_tests)
{
  cmd_bar = grub_register_command ("miray_test_activity_bar",
				 miray_test_activity_bars,
				 0, N_("Draw a activitybar"));


  cmd_screen = grub_register_command ("miray_screen", miray_test_screen,
				 0, N_("Draw miray bootscreen"));

  cmd_redraw = grub_register_command ("miray_redraw", miray_test_redraw, 
                                 0, N_("Redraw bootscreen"));

  grub_dprintf("miray", "Commands registered");
}

GRUB_MOD_FINI(miray_bootscreen_tests)
{
  grub_dprintf("miray", "Unregistering commands");
  //grub_unregister_command (cmd_logo);
  grub_unregister_command (cmd_bar);
  grub_unregister_command (cmd_screen);
  grub_unregister_command (cmd_redraw);
}
