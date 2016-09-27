#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "setup.h"

void setup(EditorSetup *edsetup, int argc, char *argv[])
{
  edsetup->progName = PACKAGE_STRING;
  /* TODO: parse configuration file */
  /* TODO: parse command line args */
  setupUI(edsetup);
}

void cleanup(EditorSetup *edsetup)
{
  cleanupUI(edsetup);
}
