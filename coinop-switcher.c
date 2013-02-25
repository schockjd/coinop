/*
 * coin-op HDMI switcher routines
 */
#include <errno.h>
#include "coinop-config.h"
#include "coinop.h"


/******************************************************************************
 * set_switcher - set the HDMI switcher to the desired output.
 ******************************************************************************/
int set_switcher (unsigned int select) {
  int rc;
  switch (select) { 
    case 0:
      rc = gpio_set_value(HDMI1_GPIO, 0);
      if (rc < 0) return rc;
      rc = gpio_set_value(HDMI2_GPIO, 0);
      if (rc < 0) return rc;
      rc = gpio_set_value(HDMI3_GPIO, 0);
      if (rc < 0) return rc;
      break;
    case 1:
      rc = gpio_set_value(HDMI2_GPIO, 0);
      if (rc < 0) return rc;
      rc = gpio_set_value(HDMI3_GPIO, 0);
      if (rc < 0) return rc;
      rc = gpio_set_value(HDMI1_GPIO, 1);
      break;
    case 2:
      rc = gpio_set_value(HDMI1_GPIO, 0);
      if (rc < 0) return rc;
      rc = gpio_set_value(HDMI3_GPIO, 0);
      if (rc < 0) return rc;
      rc = gpio_set_value(HDMI2_GPIO, 1);
    case 3:
      rc = gpio_set_value(HDMI1_GPIO, 0);
      if (rc < 0) return rc;
      rc = gpio_set_value(HDMI2_GPIO, 0);
      if (rc < 0) return rc;
      rc = gpio_set_value(HDMI3_GPIO, 1);
      if (rc < 0) return rc;
      break;
    default:
      return EINVAL;
  }
  return 0;
}

/******************************************************************************
 * get_switcher - get the current switcher setting.
 ******************************************************************************/
int get_switcher (unsigned int *select) { 
  int rc, value;
  
  rc = gpio_get_value(HDMI1_GPIO, &value);
  if (rc < 0) return rc;
  if (value) {
    *select = 1;
    return 0;
  }

  rc = gpio_get_value(HDMI2_GPIO, &value);
  if (rc < 0) return rc;
  if (value) {
    *select = 2;
    return 0;
  }

  rc = gpio_get_value(HDMI3_GPIO, &value);
  if (rc < 0) return rc;
  if (value) {
    *select = 3;
    return 0;
  }

  *select = 0;
  return 0;
}

