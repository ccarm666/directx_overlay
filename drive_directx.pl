use strict;
use warnings;
use Socket;
use Win32::OLE;
use Win32::GUI;
use Win32::Process::Info;
use Win32::GuiTest;
use Win32::Capture;
use Imager::Screenshot 'screenshot';
use Win32::GUI::Constants qw(
		GW_CHILD
		GW_HWNDNEXT
		SM_CXSCREEN
		SM_CYSCREEN
		);
use GD;
my $netv_port     = 12000;
my $netv_ip       = "127.0.0.1";
my $ipaddr;
my $portaddr;
my $msg;
socket( SOCKET, PF_INET, SOCK_DGRAM, getprotobyname("udp") )
  or die "socket: $!";
$ipaddr = inet_aton($netv_ip);
$portaddr = sockaddr_in( $netv_port, $ipaddr );

#send_msg_to_netv("testing, 1");
#send_msg_to_netv("testing, 2");
#send_msg_to_netv("testing, 3");
#send_msg_to_netv("testing, 4");
#send_msg_to_netv("Exit");
#exit(0);


my %ignoreList=("WinMover",1,"List_of_Windows",1,"Attributes",1);
my $pi = Win32::Process::Info->new();
my $sprite_id=$ARGV[0];
my $move_x=$ARGV[1];
my $move_y=$ARGV[2];

#send_msg_to_netv("Clear");
&find_sprite_demo_win_2;

#sleep 30;
#my $cap=find_tobii_win();
#print "about to capture\n";
#if($cap) {
#  my $image = CaptureWindow($cap, 0, 1); # hwnds wait flag 0 is entire 1 is no border
#   $image->SaveToFile("mycomm.png");
 #   print "done with capture\n" ;
#	&make_button_inverse();	
#  } else {
#   print "Error!!! no tobii window found\n"
 # }
&find_sprite_demo_win_2;
#send_msg_to_netv("1,3,5,7,9");
send_msg_to_netv("Reset_coordxy");
send_msg_to_netv("Alter_coordxy,1,400,500");
send_msg_to_netv("Alter_coordxy,1,0,0"); 
send_msg_to_netv("Alter_coordxy,2,300,0"); 
send_msg_to_netv("Alter_coordxy,3,600,0"); 
send_msg_to_netv("Alter_coordxy,4,900,0"); 
send_msg_to_netv("Alter_coordxy,5,1200,0"); 
send_msg_to_netv("Alter_coordxy,6,1500,0"); 
send_msg_to_netv("Alter_coordxy,7,0,200");
send_msg_to_netv("Alter_coordxy,8,300,200");
send_msg_to_netv("Alter_coordxy,9,600,200"); 
send_msg_to_netv("Alter_coordxy,10,900,200");
send_msg_to_netv("Alter_coordxy,11,1200,200"); 
send_msg_to_netv("Alter_coordxy,12,1500,200"); 
send_msg_to_netv("Alter_coordxy,13,0,400"); 
send_msg_to_netv("Alter_coordxy,14,300,400"); 
send_msg_to_netv("Alter_coordxy,15,600,400") ;
send_msg_to_netv("Alter_coordxy,16,900,400") ;
send_msg_to_netv("Alter_coordxy,17,1200,400"); 
send_msg_to_netv("Alter_coordxy,18,1500,400"); 
send_msg_to_netv("Alter_coordxy,19,0,600"); 
send_msg_to_netv("Alter_coordxy,20,300,600"); 
send_msg_to_netv("Alter_coordxy,21,600,600"); 
send_msg_to_netv("Alter_coordxy,22,900,600"); 
send_msg_to_netv("Alter_coordxy,23,1200,600"); 
send_msg_to_netv("Alter_coordxy,24,1500,600");
send_msg_to_netv("1,3,5,7,9,11,13,15,17");
sleep (1);
for(my $i=0;$i<3;$i++) {
  send_msg_to_netv("Autoflash 1");
  sleep (2);
  send_msg_to_netv("Conceal");
  sleep(2);
  }

send_msg_to_netv("Clear");


#send_msg_to_netv("Move_relative,$sprite_id,$move_x,$move_y");	
#send_msg_to_netv("$sprite_id");
#for (my $i=0;$i<5;$i++) {
#   send_msg_to_netv("Autoflash 1");
#   sleep(3);
#   send_msg_to_netv("Conceal");
#   sleep(3);
#}

sleep(5);
send_msg_to_netv("Reset_coordxy");
&find_sprite_demo_win_2;
send_msg_to_netv("Change_background_texture,mycomm_mod.png");
#send_msg_to_netv("Change_background_texture,mycomm.png");
&find_sprite_demo_win_2;
sleep(10);
&find_sprite_demo_win_2;
send_msg_to_netv("Exit");
#send_msg_to_netv("End");			  
sub send_msg_to_netv {
    my ($udp_msg)=@_;
	send( SOCKET, $udp_msg, 0, $portaddr ) == length($udp_msg)
          or die "cannot send to $netv_ip($netv_port): $!";
}


sub getWinInfo {
    my (%winInfo);
    my $win = Win32::GUI::GetDesktopWindow();
    my $cwin = Win32::GUI::GetWindow( $win, GW_CHILD );
    while ($cwin) {
        if (   &isUserWindow($cwin)
            && ( ( Win32::GUI::GetClassName($cwin) ne 'Shell_TrayWnd' ) )
            && ( Win32::GUI::GetClassName($cwin) ne 'Progman' ) )
        {
            $winInfo{$cwin}{handle} = $cwin;
            $winInfo{$cwin}{text}   = Win32::GUI::Text($cwin);
#			if($winInfo{$cwin}{text}=~/emailclient/) {
#			 print "found emailclient at 443\n";
#			 }
            $winInfo{$cwin}{class}  = Win32::GUI::GetClassName($cwin);
#			print "class is $winInfo{$cwin}{class}\n";
            $winInfo{$cwin}{height} = Win32::GUI::Height($cwin);
            $winInfo{$cwin}{width}  = Win32::GUI::Width($cwin);
            my ( $left, $top, $right, $bottom ) =
              Win32::GUI::GetWindowRect($cwin);
            $winInfo{$cwin}{xpos} = $left;
            $winInfo{$cwin}{ypos} = $top;
            $winInfo{$cwin}{path} = &execPath($cwin);
        }
        $cwin = Win32::GUI::GetWindow( $cwin, GW_HWNDNEXT );
    }
    return (%winInfo);
}
sub isUserWindow {
    my ($twin) = @_;
	my $text = Win32::GUI::Text($twin);
    if ( Win32::GUI::IsVisible($twin) and ( !Win32::GUI::IsIconic($twin) ) ) {
	    if (exists($ignoreList{Win32::GUI::Text($twin)})){return (0)}; #ignore certain windows
        if ( !( &execPath($twin) eq 'C:\WINNT\Explorer.EXE' ) ) {
            return (1);
        }
    }
    return (0);
}

 sub execPath {
    my ($twin) = @_;
    my ( $th, $pr ) = Win32::GUI::GetWindowThreadProcessId($twin);
    $th = 0;
    my (@i) = $pi->GetProcInfo($pr);
    my ($h) = $i[0];
	if(defined $h->{ExecutablePath}) {
	   return ( $h->{ExecutablePath})
	} else {return("")};
} 

sub find_tobii_win {
my %winInfo=&getWinInfo();
my $k;
for(my $i=0;$i<60;$i++) {
 foreach $k ( keys %winInfo ) { 
   if ($winInfo{$k}{class} =~ "TOBIICOMMUNICATOR5") {
     print "found tobii windows $winInfo{$k}{class}\n";
	 return($k)
   }
  }
  sleep 1;
  }
  print  "tobii window not found\n";  
  return("");
 }
sub find_sprite_demo_win {
my %winInfo=&getWinInfo();
my $k;
 foreach $k ( keys %winInfo ) { 
   if ($winInfo{$k}{text} =~ "Updating Sprite") {
     print "found Sprite Demo windows $winInfo{$k}{text}\n";
	 Win32::GuiTest::SetForegroundWindow($k);
#	 Win32::GUITest::Move( $k, -3500, 300 );
	 Win32::GUITest::Move( $k, 0, 0 );
	 return($k)
	 
   }
  }
  print  "Sprite Demo window not found\n";  
  return("");
 }
 
sub find_sprite_demo_win_2 {
     my @whnds=Win32::GuiTest::FindWindowLike(undef,"Updating Sprite");
     if(!@whnds) {
	     print  "Sprite Demo window not found\n";
	  } else {
	     print "Sprite Demo window found\n";
	     Win32::GuiTest::SetForegroundWindow($whnds[0]);
	  }
}

sub make_button_inverse {
my $us_myImage = GD::Image->newFromPng('mycomm.png');
my $myImage=GD::Image->new(1600,900);
$myImage->copyResized($us_myImage,0,0,0,0,1600,900,$us_myImage->getBounds);
my ($i,$j);
for ($i=410;$i<=510;$i++) {
  for($j=420;$j<=520;$j++) {
     my $index = $myImage->getPixel($i,$j);
     my($r,$g,$b) = $myImage->rgb($index);
	 print "$r $g $b\n";
	 $r=255-$r;$g=255-$g;$b=255-$b;
	 print "$r $g $b\n";
	 $index = $myImage->colorClosest($r,$g,$b);
   	 $myImage->setPixel($i,$j,$index);
  }
 }
my $png_data = $myImage->png;
open (my $fh, '>:raw', 'mycomm_mod.png') || die;    
print $fh $png_data;
close $fh;
}

sub tobii_capture {
  my $cap=find_tobii_win();
  print "about to capture\n";
  if($cap) {
    my $image = CaptureWindow($cap, 0, 1); # hwnds wait flag 0 is entire 1 is no border
    $image->SaveToFile("mycomm.png");
     print "done with capture\n" ;
#	&make_button_inverse();	
  } else {
   print "Error!!! no tobii window found\n"
  } 
}