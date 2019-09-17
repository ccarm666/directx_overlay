use strict;
use warnings;
use Win32;
use Win32::OLE;
use Win32::GUI;
use Win32::Process;
#perl2exe_include Win32::Process
use Win32::Process::List;
use Win32::GuiTest;
#use Win32::Capture;
use Image::Size;
#use Win32::Screenshot;
use Image::Size;
#use Imager::Screenshot 'screenshot';
# to get rid of pink,gimp->layer->transparency,select-bycolor, edit->delete,file->overwrite
#my $SetWindowPos = Win32::API->new("user32","SetWindowPos", "LLLLLLL", "L")
#    or die "Failed to load SetWindowPos";
use Win32::GUI::Constants qw(
		WS_CAPTION
		SWP_FRAMECHANGED
		TME_HOVER
		TME_LEAVE
		HOVER_DEFAULT
		SWP_NOMOVE
		SWP_NOSIZE
		SWP_NOZORDER
		SWP_NOACTIVATE
		WS_THICKFRAME
		WS_MINIMIZEBOX
		WS_BORDER
		);		
use Carp qw(cluck longmess shortmess);
import Win32::Process qw(NORMAL_PRIORITY_CLASS);
my $flags = NORMAL_PRIORITY_CLASS();
# are we already running
use Fcntl ':flock';
open my $self, '<', $0 or die "Couldn't open self: $!";
#open my $self, '<', $0 or comm5_already_running();
#flock $self, LOCK_EX | LOCK_NB or die "This script is already running";
flock $self, LOCK_EX | LOCK_NB or comm5_already_running();
my %HOTKEY_IDS = (
  '1' => 121, #globols
  '2' => 122,
  '3' => 123,
  '4' => 124,
  '5' => 125,
  '6' => 126,
  '7' => 127,
  '8' => 128,
  '9' => 129,
  'A' => 130,
  'B' => 131,
  'C' => 132,
  'D' => 133,
  'E' => 134,
  'F' => 135,
  'G' => 136,
  'H' => 137,
  'I' => 138,
  'J' => 139,
  'K' => 140,
  'L' => 141,
  'M' => 142,
  'N' => 143,
  'O' => 144,
  'P' => 145,
  'Q' => 146,
  'R' => 147,
  'S' => 148,
  'T' => 149,
  'U' => 150,
  'V' => 151,
  'W' => 152,
  'X' => 153,
  'Y' => 154,
  'Z' => 155,
);

our @freq=(30, 20, 15, 12, 10,6,5); # 60hz screen allowable frequencies
my $long_message;
#perl2exe_include bytes
#perl2exe_include Win32::OLE
use Win32::ActAcc;

#perl2exe_include Win32::ActAcc
use Data::Dumper;

#perl2exe_include Data::Dumper
use Socket;

#perl2exe_include Socket
use Win32::GuiTest qw(:ALL :SW);

# graphics stuff. It exist in the Win32::GUITEST
use Win32::GUI::Constants qw(
		GW_CHILD
		GW_HWNDNEXT
		SM_CXSCREEN
		SM_CYSCREEN
		);
#perl2exe_include Win32::GuiTest
use Win32::Process::Info;

#perl2exe_include Win32::Process::Info
use Win32::API;
use Win32::API::Callback;

Win32::API::Struct->typedef( RECT => qw{
                                LONG Left; 
                                LONG Top;
                                LONG Right;
                                LONG Bottom; });

Win32::API::Struct->typedef( MONITORINFO => qw{
                                LONG cbSize; 
                                RECT rcMonitor;
                                RECT rcWork;
                                LONG dwFlags; });

Win32::API->Import('User32', 'EnumDisplayMonitors', 'NPKN', 'N');
Win32::API->Import('User32', 'int GetMonitorInfoA (int hMonitor, LPMONITORINFO lpmi)');
my $num_mons=0;my $primaryx=0;my $primaryy=0;
my $MonitorEnumProc = Win32::API::Callback->new(
    sub {
        my( $hMonitor, $hdcMonitor, $lprcMonitor, $dwData) = @_;
        
        my $MI     = Win32::API::Struct->new('MONITORINFO');
        my $R      = Win32::API::Struct->new('RECT');
        $MI->{cbSize} = 40;
        GetMonitorInfoA ($hMonitor, $MI);
		$num_mons++;
        print "Monitor resolution : ",
            $MI->{rcMonitor}->{Right} - $MI->{rcMonitor}->{Left}, ' x ',
            $MI->{rcMonitor}->{Bottom} - $MI->{rcMonitor}->{Top}, "\n";
			if(($MI->{rcMonitor}->{Left} ==0)) {
			  $primaryx=($MI->{rcMonitor}->{Right} - $MI->{rcMonitor}->{Left});
			  $primaryy=($MI->{rcMonitor}->{Bottom} - $MI->{rcMonitor}->{Top});
			  print "Found primary monitor\n";
             return 1;
			}
    },
    "NNPN", "N",
  );

EnumDisplayMonitors ( 0x0, 0x0, $MonitorEnumProc, 0x0  );


$SIG{INT} = \&signal_handler;

sub OPER  { "^TfMain" };

#$SIG{TERM} = \&signal_handler;

#log_print defs
use IO::Socket::Multicast;
use constant DESTINATION => '127.0.0.1:3671';
my $s = IO::Socket::Multicast->new( Proto => 'udp', PeerAddr => DESTINATION );

sub udp_log_send {
    my ($log_data) = @_;
    $s->mcast_send( $log_data, DESTINATION );
}


#logfile creation
use Win32 qw(CSIDL_DESKTOPDIRECTORY);
our $DESKTOP= Win32::GetFolderPath(CSIDL_DESKTOPDIRECTORY);
our $LOG_FILE="$DESKTOP\\tobii\logfile.txt";
open (our $LOG_FH, '>>', 'logfile.txt');

sub write_to_log {
   my $message = shift;
   my( $package, $filename, $line ) = caller;
   my $total_message =  localtime() . ":" . $message . " from line $line\n";
   print $LOG_FH $total_message;
#   &udp_log_send($total_message);
   print $LOG_FH $total_message;  
}
write_to_log ("*************************************************new run ******************************");

my $pi = Win32::Process::Info->new( { assert_debug_priv => 1 } );
# data structure for process information(global)
#our $offset_x       = 1500;
#our $offset_y       = 800;
#our $screen_size_x = 1600;
#our $screen_size_y = 900;
our $offset_x       = $primaryx-2;
our $offset_y       = $primaryy-2;
our $screen_size_x = $primaryx;
our $screen_size_y = $primaryy;

our $offset_x_right = $offset_x + $screen_size_x;
our $offset_y_lower = $offset_y + $screen_size_y;
our %tobii_buttons;
my $found         = 0;
my $count         = 0;
my $port          = 20320;
my $port_write    = 20321;
my $bci_selection = 0;
my $netv_port     = 12000;
my $netv_ip       = "127.0.0.1";
my $max_bind_trys = 15;            #maxium nunber of trys before you give up.
my $id;                            #id current selected button
my $hwnd;                          # handle of current communicator 5 window
my $bci_process;
my $bci_prog = 'C:\\BCIHomeSystemFiles\\VA_BCI2000\\batch\\P3Speller_SigGen.bat';
my $bci_prog_dir = 'C:\\BCIHomeSystemFiles\\VA_BCI2000\\prog\\';
my $cmd= $bci_prog;
my $exit_code = 0;
my $prog_dir ='C:\\BCIHomeSystemFiles\\VA_BCI2000\\prog';
my $proc_operat;
my $proc_source;
my $proc_P3SignalProcessing;
my $proc_P3Speller;
my $parm_dir = "C:\\BCIHomeSystemFiles\\VA_BCI2000\\parms\\";  # parms directory
my $parm_file = "";    # parm file use in bci2000;
my $communicator_pid;
my $e;  # for event mobnitoring like mouse clicks
# main
Win32::OLE->Initialize();
# intialize golobal variables for page change detection
my $current_page  = "";
my $old_page ="";


#	&get_closest_parm(25,$parm_dir); system test for get_closest_parm
my %pushButtons;
my $ipaddr;
my $portaddr;
my $msg;
socket( SOCKET, PF_INET, SOCK_DGRAM, getprotobyname("udp") )
  or die "socket: $!";
$ipaddr = inet_aton($netv_ip);
$portaddr = sockaddr_in( $netv_port, $ipaddr );
#system 1, 'C:\BCIHomeSystemFiles\BCIAddons\batch\start_faces_on_netv.bat';

sleep(2);
`C:\\Windows\\System32\\taskkill.exe /f /fi "imagename eq Communicator*"`;  # kill leftover communicator
$communicator_pid = system 1,'C:\Program Files (x86)\Tobii Dynavox\Communicator 5\communicator.exe';
sleep 5;                                           #wait for communicator
wait_for_no_start_page();
&wait_move_window("TOBIICOMMUNICATOR5",$offset_x,$offset_y);
# my $img = screenshot();
sleep(1);
&tobii_capture();
#sleep(2);
#system 1, 'C:\Users\steve\Desktop\SpriteWithTimer\Release\SpriteWithTimer.exe';
#system 1, '.\Release\SpriteWithTimer.exe';
my($globe_x, $globe_y) = imgsize("mycomm.png");
system 1, '.\Release\SpriteWithTimer.exe',  'mycomm.png',$globe_x ,$globe_y;
#exit(0);
while (1) {
    write_to_log "at top of while loop line " ;
	my $win = GetDesktopWindow();
	$old_page = curr_comm5_win();
	&make_windows_top_by_class("SpriteUpdateWithTimer");
 #   eval {
    my $ao = Win32::ActAcc::Desktop();
    %pushButtons = ();
    $found       = 0;
    $count       = 0;
    send_msg_to_netv("Reset_coordxy");
	undef  %tobii_buttons;
	&tobii_capture();
	my $try_count= 2;
	do {
        &tree( $ao, 0 );
        #print "count=$count\n";
        write_to_log "count=$count\n";
        sleep 1 if ($count <= 0);
		$try_count--;
	} until($count or !$try_count);
	&signal_handler if (!$try_count); # try 10 times and quit
    $parm_file = &get_closest_parm( $count, $parm_dir );
#	 &tobii_capture();
    write_to_log "Count is $count parmfile is $parm_file";
#   my $rc_bci2000 = system 1,"C:\\BCIHomeSystemFiles\\BCIAddons\\batch\\start_tobii_with_parm.bat $parm_file";
	start_bci2000($parm_file);
	  &wait_move_window(OPER,-1200,0); # move bci oper windows off of main window
	  &wait_move_window("TVisGraphForm",-1400,0);
	  &wait_move_window("TVisForm",-1500,0);
	  &wait_move_window("TForm",-1450,0);	   	  
	  &make_windows_top_by_class("SpriteUpdateWithTimer");
#	  &make_windows_top_by_class("MozillaWindowClass");
#copy the correct parm file for menu to directory/filename
#send new coordinates of tobii menu and count to faces on netv via udp
#suspend bci2000 force a reload and subsequent setconfig because bci2000 doesn't listen to UDP during suspend
    
	 $bci_selection = &get_selection(); #Wait for selection
      write_to_log "selection is $bci_selection\n";
      ($id) = $bci_selection =~ /^.*\s+(\d+)/;
      write_to_log "selection is $bci_selection id  is $id ".  __LINE__ ;
      kill_bci2000();
      clear_screen();
	  &make_windows_top_by_class("SpriteUpdateWithTimer");
#     kill_bci2000(); # Changed 6/7/2017
      write_to_log "outside if selection is $bci_selection id  is $id";
      if ( $id != 0 ) {
        my $ao_button = $pushButtons{$id}{ao_pointer};
#		write_to_log("at " . __LINE__ );
        if(!page_changed()) {
           $ao_button->accDoDefaultAction() if ( $id <= $count and defined ($ao_button->accDoDefaultAction()) );
		} else {
		    write_to_log("page changed so I need to skip accDoDefaultAction");
			&tobii_capture();
			send_msg_to_netv("Change_background_texture,mycomm_mod.png");
		}
#		write_to_log("at " . __LINE__ );
        write_to_log "inside if selection is $bci_selection id  is $id";
#		sleep(2);
        }
#    };
     clear_screen();
	if ($@) {
      warn("$@\n");
	  write_to_log "fatal error caught by eval $@";
	  sleep 2;
    }	
}


sub tree {
    my $ao    = shift;
    my $level = shift;
    return if ( $level > 6 );    # prune
    my ( $left, $top, $width, $height, $bid );
#       . $ao->get_accRole() . " "
    my $ao_info = ""
      . ( ' ' x $level )
      . ( $ao->describe() ) . " "
      . $ao->get_itemID() . "\n";
    if ( $ao_info =~ /window:Tobii Communicator Run View/ ) { $found = 1; }
    if ( $ao_info =~ /^(?=.*\window:)((?!Tobii).)*$/ )      { $found = 0; }
    if ($found) {
        print "$ao_info";

        #	   $hwnd = $ao->WindowFromAccessibleObject();
        if ( $ao_info =~ /push button:/ ) {
            ( $left, $top, $width, $height ) = $ao->accLocation();
            $bid                                    = $ao->get_itemID;
            $pushButtons{ $ao->get_itemID }{left}   = $left;
            $pushButtons{ $ao->get_itemID }{top}    = $top;
            $pushButtons{ $ao->get_itemID }{width}  = $width;
            $pushButtons{ $ao->get_itemID }{height} = $height;
            $left                                   =int(($left + $width / 2 - 30)-$offset_x);
            $top                                    = int(($top + $height / 2 - 30)-$offset_y);

#		  `c:\\BCIHomeSystemFiles\\BCIAddons\\bin\\UDPSend.exe 10.0.88.1 12000 "Alter_coordxy,$bid,$left,$top"`;
            $msg = "Alter_coordxy,$bid,$left,$top";
			$tobii_buttons{$bid}{"left"}=$left;
			$tobii_buttons{$bid}{"right"}=$left+$width;
			$tobii_buttons{$bid}{"top"}=$top;
			$tobii_buttons{$bid}{"bottom"}=$top+$height;
			print $msg,"\n";
            send( SOCKET, $msg, 0, $portaddr ) == length($msg)
              or die "cannot send to $netv_ip($netv_port): $!";
            $pushButtons{ $ao->get_itemID }{ao_pointer} = $ao;
            $count++;
			$msg = "Alter_ssvep,$bid, " . 1/($freq[($bid%7)]);
			print $msg,"\n";
#            send( SOCKET, $msg, 0, $portaddr ) == length($msg)
#             or die "cannot send to $netv_ip($netv_port): $!";
        } 
    }
    elsif ( !( kill 0, $communicator_pid ) ) {
        $msg = "Clear";
        send( SOCKET, $msg, 0, $portaddr ) == length($msg)
          or die "cannot send to $netv_ip($netv_port): $!";
        $msg = "Exit";
        send( SOCKET, $msg, 0, $portaddr ) == length($msg)
          or die "cannot send to $netv_ip($netv_port): $!";
        exit(1);
    }
    my @ch = $ao->AccessibleChildren();
    foreach (@ch) {
        tree( $_, 1 + $level );

        #		print "current level is$level\n"
    }
}

sub get_selection {
#    &find_firefox_win_move_resize();
    write_to_log "at top of get_selection line is " .  __LINE__ ;
	system 1, 'c:\\BCIHomeSystemFiles\\BCIAddons\\bin\\set_comm_top';
    my $p3              = 0;
    my $selection_found = 0;    #init udp and selection stuff
    my $curr_selection  = 0;
    socket( UDP, PF_INET, SOCK_DGRAM, getprotobyname("udp") );
    my $bcount = 0;
	my $pcount = 0;
    while ( !( bind( UDP, sockaddr_in( $port, INADDR_ANY ) ) ) ) {
        sleep 1;
        $bcount++;
        if ( $bcount > $max_bind_trys ) {
            write_to_log "can not bind to $port aka bci2000 app connector for states at line " .  __LINE__ ;              
            exit;
        }
    }
    while ( $curr_selection = <UDP> ) {
	 $pcount++;
	 if($pcount == 750) {  # polling interval larger the number the slower the polling.
#	  write_to_log "at while loop about to check if page has changed"
#	   sleep (1);
	  $pcount = 0;  
#	  &make_windows_top_by_class("MozillaWindowClass");
      if(&page_changed()) {		 
	   close(UDP);
	   clear_screen();
	   sleep(1);
	   kill_bci2000();
	   clear_screen();
	   return(0); #make sure you don't do default action since page has been invalidated
	   }
	 }
	    if ( ( $curr_selection =~ /mouse at/ ) ) {
		   print $curr_selection,"\n";
		   my($x,$y)=$curr_selection =~ /mouse at x=(\d+) and y=(\d+)/;
		   print "$x and $y\n" if($x);
		   my $closest_button = &find_closest_button($x,$y);
		   print "closest button is $closest_button\n";
		   return ("SelectedTarget $closest_button") if ($closest_button > 0); 
		}
        if ( ( $curr_selection =~ /PhaseInSequence 3/ ) ) {
            $p3 = 1;
        }
        elsif ($p3
            && ( $curr_selection =~ /SelectedTarget/ )
            && ( !( $curr_selection =~ /SelectedTarget 0/ ) )
            && ( !$selection_found ) )
        {
            write_to_log "found $curr_selection";
            $selection_found = 1;
            close(UDP);
            return ($curr_selection);
        }
        elsif ( ( $curr_selection =~ /PhaseInSequence 2/ ) ) {
            $selection_found = 0;
            $p3              = 0;
        }
        elsif ( ( $curr_selection =~ /PhaseInSequence 1/ ) ) {
            $selection_found = 0;
            $p3              = 0;
        }
    }
	write_to_log "Danger!!! exited get_selection from bottom of loop, that should be impossible";
	close(UDP); # WE SHOULD NOT MAKE IT HERE
	exit(2);
	return(10000);
}

sub get_closest_parm {
    my ( $number_of_buttons, $area ) = @_;
    my ( $file, $diff, $result, $found, $filename );
    $found    = 0;
    $filename = "";
    $result   = 10000000;    # a large number
    opendir( DIR, $area ) or die "Can't read '$area' : $!";
    while ( $file = readdir(DIR) ) {
        if ( $file =~ /^communicator_\d+\.prm/ ) {
#            write_to_log "$file\n";
            my ($num) = $file =~ /^communicator_(\d+)\.prm/;
#            write_to_log "$num\n";
            if ( ( $num >= $number_of_buttons ) and ( $num < $result ) ) {
                $found    = 1;
                $result   = $num;
                $filename = $area . $file;
            }
        }
    }
    if ($found) {
        return ($filename);
    }
    else {
        die "could not find approriate parm file for $number_of_buttons\n";
    }
}

sub signal_handler {
`c:\\BCIHomeSystemFiles\\BCIAddons\\bin\\UDPSend.exe 127.0.0.1 20321 "Running 0"`;
sleep(2);
    `C:\\Windows\\System32\\taskkill.exe /f /fi "imagename eq Communicator.exe"`;
	`C:\\Windows\\System32\\taskkill.exe /f /fi "imagename eq operat.exe"`;
#	`c:\\BCIHomeSystemFiles\\BCIAddons\\batch\\clear_screen.bat`; 
#	`c:\\BCIHomeSystemFiles\\BCIAddons\\batch\\killall_faces.bat`;
    `C:\\Windows\\System32\\taskkill.exe  /f /fi "imagename eq SpriteWithTimer.exe"`;
	`C:\\Windows\\System32\\taskkill.exe  /f /fi "imagename eq Firefox"`;
	`C:\\Windows\\System32\\taskkill.exe /f /fi "imagename eq firefox.exe"`;
    write_to_log "Caught a signal $!";
    exit;
}



sub send_msg_to_netv {
    my ($udp_msg)=@_;
	send( SOCKET, $udp_msg, 0, $portaddr ) == length($udp_msg)
          or die "cannot send to $netv_ip($netv_port): $!";
}

sub make_windows_top_by_class {
     my ($class)=@_;
	 my (@window)=FindWindowLike(undef,"",$class);
	 foreach my $w (@window) {
	    SetActiveWindow($w);
		SetForegroundWindow($w);
	 }
}

sub wait_move_window {
my($class,$x,$y)=@_;
my $count=30;
my $not_found=1;
while ($count and !(FindWindowLike(undef,"",$class))) {
 $count--;
 sleep(1);
}
return(0) if $count == 0;
my (@window)=FindWindowLike(undef,"",$class);
	 foreach my $w (@window) {
	    moveIt($w,$x,$y);  
#$SetWindowPos->Call($w->{-handle}, 1500, 800, 1500, 800,
#			SWP_FRAMECHANGED|SWP_NOMOVE|
#			SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);		
     }
return(1);
}

sub window_wait {
my($class,$count)=@_;
my $not_found=1;
while ($count and !(FindWindowLike(undef,"",$class))) {
 $count--;
 sleep(1);
}
return(0) if $count == 0;
return(1);
}

sub window_wait_not{
my($title,$count)=@_;
write_to_log("In windows_wait_not Entering ");
my $not_found=1;
while ($count) {
   return (1) if((FindWindowLike(undef,'Sono Key Scroll*')));
   $count--;
   sleep(1);
}
return(0)
}


sub moveIt {
  my($twin,$newX,$newY)=@_;
  my($x,$y)=&winPosition($twin);
  return(1) if(($x == $newX) and ($y ==$newY));
  Win32::GUI::Move($twin,$newX,$newY);
#  Win32::GuiTest::SetWindowPos($twin,0,$newX,$newY,0,0);
  return(1);
}
sub winPosition {
  my($winhd)=@_;
  my($left, $top, $right, $bottom)=Win32::GUI::GetWindowRect($winhd);
    return($left,$top);
}
sub pid_exists {
my($pid)=@_;
my $P = Win32::Process::List->new();  #constructor
my %list = $P->GetProcesses();        #returns the hashes with PID and process name
if(exists $list{$pid}) {
write_to_log("$pid still exists");
} else {
   write_to_log("$pid is dead, Jim\n");
}
return(exists $list{$pid});
}

sub wait_for_no_start_page {
for (my $i=1;$i<200;$i++) { 
   my @handles =   grep {IsWindowVisible($_) } 
                FindWindowLike(GetDesktopWindow(),
                               qr/^Page Set/,
                               undef,
                               undef);						   							             		 
   if (@handles < 1) {
      write_to_log("No start page, time to start the faces");
      return(1);
   } else {
      write_to_log("found start page windows");
   }
   sleep 1;
}
return(0);
}
sub clear_screen {
  my( $package, $filename, $line ) = caller;
#  `plink.exe -ssh root\@10.0.88.1 ./clearscreen.sh`;
#  sleep 1;
 # `plink.exe -ssh root\@10.0.88.1 ./clearscreen.sh`;
  write_to_log("just cleared screen called from line $line ");
}

      	     
sub find_pid_from_name {
   my $name =shift;
   my $P = Win32::Process::List->new();
   my %list = $P->GetProcesses();
   foreach my $pid ( keys %list ) {
    my $proc = $list{$pid};
#	write_to_log "process name is $proc";
    if ($proc =~ /\Q$name\E/) {
	 write_to_log "found process name $name";
     return($pid)
	}
  }
return(0);
}
	
## returns the msaa description of current communcator 5 window. returns '' if no window found.
sub curr_comm5_win {
  my @cwin = ();
  @cwin = FindWindowLike(undef,"Communicator"); 
  my $aatxt = '';
  my $wcount=0;
  for my $cwin ( @cwin ) { 
   write_to_log "Null handle in curr_comm5_win", return unless ($cwin); 
   $wcount++; 
   my $ao = Win32::ActAcc::AccessibleObjectFromWindow($cwin);
   $aatxt = '';
   $aatxt = $ao->describe() if defined $ao;
   #print "\n # $wcount aaText= ",$aatxt, "\n";
  }
  write_to_log "more than 1 aa comm5 window, returning last window found" if($wcount > 1) ;
  return ($aatxt);  
} 

# return 1 if page has changed or 0 if paged has not changed and 0 if no page is found;
sub page_changed {
  $current_page = curr_comm5_win();
  if (!$current_page) {
   write_to_log "no page found";
   return (0);
  }   
  if ($current_page ne $old_page) {
   if ($current_page =~/Firefox Main/) { #ignore firefox
     $old_page = $current_page;
     return(0);
   }	 
   write_to_log "page change!!\n new page is $current_page";
   $old_page = $current_page;
   return (1);
  }
}

#launch a process
sub mySystem {
  my($prog,$progCmd,$progDir)=@_;
  my $ProcessObj;
  print "prog=$prog   progArgs=$progCmd  progDir= $progDir\n";
  Win32::Process::Create($ProcessObj,
                                $prog,
                                $progCmd,
								0,
                                $flags,
                                "$progDir")|| die ErrorReport();
  return($ProcessObj);

}

sub start_bci2000 {
  my $cparm_file=shift;
  $proc_operat = mySystem($bci_prog_dir . 'operat.exe',$bci_prog_dir . 'operate.exe ' ."--OnConnect \"-LOAD PARAMETERFILE $cparm_file;SETCONFIG\"  --OnSuspend \"-QUIT\"  --OnSetConfig \"-SET STATE Running 1\"",$bci_prog_dir);
#  $proc_source = mySystem($bci_prog_dir . 'gUSBampSource.exe',$bci_prog_dir . 'gUSBampSource.exe' . '127.0.0.1',$bci_prog_dir);
  $proc_source = mySystem($bci_prog_dir . 'SignalGenerator.exe',$bci_prog_dir . 'SignalGenerator.exe' . '127.0.0.1',$bci_prog_dir);
  $proc_P3SignalProcessing = mySystem($bci_prog_dir . 'P3SignalProcessing.exe',$bci_prog_dir . 'P3SignalProcessing.exe' . '127.0.0.1',$bci_prog_dir);
  $proc_P3Speller = mySystem($bci_prog_dir . 'P3Speller.exe',$bci_prog_dir . 'P3Speller.exe' . '127.0.0.1',$bci_prog_dir);
  sleep(1);
}

sub kill_bci2000 {
  write_to_log "attempting to kill BCI2000";
  system 1, 'c:\\BCIHomeSystemFiles\\BCIAddons\\bin\\UDPSend.exe 127.0.0.1 20321 "Running 0"';
  sleep 1;
  $proc_operat->Kill($exit_code);
   write_to_log "exit_code is $exit_code\n";
   my $result=$proc_operat->Wait(50);
   if(!$result) {
     write_to_log  "operat.exe will not die";
   }
  
  $proc_source->Kill($exit_code);
    write_to_log "exit_code is $exit_code";
   $result=$proc_source->Wait(200);
   if(!$result) {
    write_to_log  "SignalSource.exe will not die";
   }
 
 $proc_P3SignalProcessing->Kill($exit_code);
    write_to_log "exit_code is $exit_code\n";
  $result=$proc_P3SignalProcessing->Wait(50);
  if(!$result) {
   write_to_log  "P3SignalProcessing.exe will not die";
  }
  
  $proc_P3Speller->Kill($exit_code);
    write_to_log "exit_code is $exit_code";
  $result=$proc_P3Speller->Wait(50);
  if(!$result) {
   write_to_log  "P3Speller.exe will not die";
  }
}
sub ErrorReport{
                write_to_log  Win32::FormatMessage( Win32::GetLastError() ) . " the last Window error";
}

sub MsgBox {
    my ($caption, $message, $icon_buttons) = @_;
    my @return = qw/- Ok Cancel Abort Retry Ignore Yes No/;
    my $result = Win32::MsgBox($message, $icon_buttons, $caption);
    return $return[$result];
}

sub comm5_already_running {
sleep (5);
MsgBox("Already running", "Comm5 Overlay is already running\nplease kill cmd window and run Tobii Start", 48);
exit(0);
}

sub tobii_capture {
  my $cap=find_tobii_win();
  print "about to capture\n";
  sleep(4) if (&find_firefox_win_move_resize());
  if($cap) {
#    my $image = screenshot(hwnd => $cap);
  #    my $image = CaptureWindow($cap, 0, 1); # hwnds wait flag 0 is entire 1 is no border
#    my $image = CaptureWindow($cap);
#	$image->Write('mycomm.png');
#	 $image->SaveToFile('mycomm.png');
      	SetActiveWindow($cap);
		SetForegroundWindow($cap);
	  &wait_move_window("TOBIICOMMUNICATOR5",0,0);
	  SetForegroundWindow($cap);
#	  sleep(1);
#	  SetActiveWindow($cap);
     `c:\\WINDOWS\\nircmd.exe  savescreenshotwin "C:\\Users\\steve\\Desktop\\SpriteWithTimer\\mycomm.png"`;
#     $image->write(file => "mycomm.png", type => "png")
      my($globe_x, $globe_y) = imgsize("mycomm.png");
      print "x is $globe_x, y is $globe_y";
      send_msg_to_netv("Change_background_texture,.\\mycomm.png,$globe_x,$globe_y");    
#	 send_msg_to_netv("Change_background_texture,mycomm_mod.png");
#	 sleep(1);
	 &wait_move_window("TOBIICOMMUNICATOR5",$offset_x,$offset_y);
#	 sleep(1);
	 &make_windows_top_by_class("SpriteUpdateWithTimer");
#	 or die $image->errstr;
     print "done with capture\n" ;
#	&make_button_inverse();	
  } else {
   print "Error!!! no tobii window found\n"
  } 
}
sub find_tobii_win {
  my $count=5;
  if(&window_wait("TOBIICOMMUNICATOR5",10)) {
    return(FindWindowLike(undef,"",'TOBIICOMMUNICATOR5'));
  } else {
    return(0);
  }
}
sub find_firefox_win_move_resize {
  my $class="MozillaWindowClass";
  my $fox=FindWindowLike(undef,"",$class);
  return(0) if(!$fox);
  &moveIt($fox,0,300);
#  SetForegroundWindow($fox);
  return(1);
}
sub find_firefox_win {
   my $class="MozillaWindowClass";
  my $fox=FindWindowLike(undef,"",$class);
  return(0) if(!$fox);
  return(1); 
 }
 
 sub find_closest_button_org {
 my($x,$y)=@_;
 my $key;
 my ($bx,$by); 
 my $bmin;
 my $distance = 1000000000;
 $bmin=-1;
 my $tdistance;
 foreach $key (keys %tobii_buttons) {
   $bmin=$key if($bmin==-1);
   $tdistance = ($x-$tobii_buttons{$key}{"left"})**2+($y-$tobii_buttons{$key}{"top"})**2;
   if($distance > $tdistance) {
     $distance = $tdistance;
	 $bmin=$key;
   }
   print "bmin is $bmin\n";
  }
  return($bmin);
}
   
 sub find_closest_button {
 my($x,$y)=@_;
 my $key;
 my ($bx,$by); 
 my $bmin;
 my $distance = 1000000000;
 $bmin=-1;
 my $tdistance;
 my $x1;
 foreach $key (keys %tobii_buttons) {
 if($tobii_buttons{$key}{"left"} < 1) {$x1 = 1};
 if($tobii_buttons{$key}{"Right"} < 1) {$x1 = 1};
 if($tobii_buttons{$key}{"top"} < 1) {$x1 = 1};
 if($tobii_buttons{$key}{"Bottom"} < 1) {$x1 = 1};
 
 if ((($x >= $tobii_buttons{$key}{"left"}) && ($x <= $tobii_buttons{$key}{"right"})) && ($y <= $tobii_buttons{$key}{"bottom"}) && 
         ($y >= $tobii_buttons{$key}{"top"})) {
	 return($key);
   }
} 
  return(0);
}
   
 
 
 