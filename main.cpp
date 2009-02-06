/* K DVD Author Wizard: The application */

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>

#include "dvdauthorwizard.h"

int main (int argc, char *argv[])
{
	KAboutData aboutData(
		// The program name used internally.
		"kdvdauthorwizard",
		// The message catalog name
		// If null, program name is used instead.
		0,
		// A displayable program name string.
		ki18n("K DVD Author Wizard"),
		// The program version string.
		"0.0.1",
		// Short description of what the app does.
		ki18n("An easy-to-use DVD authoring application"),
		// The license this code is released under
		KAboutData::License_GPL,
		// Copyright Statement
		ki18n("(c) 2009") );

	KCmdLineArgs::init( argc, argv, &aboutData );
	KApplication app;
	
	DVDAuthorWizard* dvdauthorwizard = new DVDAuthorWizard();
	dvdauthorwizard->show();

	return app.exec();
}
