# Copyright 2001, Phill Wolf.  See README.

# Win32::ActAcc (Active Accessibility) demo: Traverse window hierarchy

use strict;
use Win32::OLE;
#perl2exe_include Win32::OLE
use Win32::ActAcc;
#perl2exe_include Win32::ActAcc
use Win32::ActAcc::aaExplorer;
#perl2exe_include Win32::ActAcc::aaExplorer
#perl2exe_include Data::Dumper

# main
sub main
{
	Win32::OLE->Initialize();
	my $ao = Win32::ActAcc::Desktop();
	print "\naaDigger - Navigates tree of Accessible Objects\n\n";
	Win32::ActAcc::aaExplorer::aaExplore($ao);
}

&main;

