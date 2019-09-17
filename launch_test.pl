use strict;
use warnings;
#use Data::Dumper;
my $proc_operat;
my $proc_source;
my $proc_P3SignalProcessing;
my $proc_P3Speller;
my $bci_prog = 'C:\\BCIHomeSystemFiles\\VA_BCI2000\\batch\\P3Speller_SigGen.bat';
my $bci_prog_dir = 'C:\\BCIHomeSystemFiles\\VA_BCI2000\\prog\\';
my $cmd= $bci_prog;
my $parm_dir = "C:\\BCIHomeSystemFiles\\VA_BCI2000\\parms\\";  # parms directory
my $prog_dir ='C:\\BCIHomeSystemFiles\\VA_BCI2000\\prog';
my $parm_file = $parm_dir . "weights_directx_36.prm";
start_bci2000($parm_file);

sub mySystem {
my($prog,$progCmd,$progDir)=@_;
my $pid =  system 1,"$prog";
 print "pid=$pid\n";
 
 # print Dumper $job->status;
  sleep (1);
}

#sub mySystem_org {
#  my($prog,$progCmd,$progDir)=@_;
#  my $ProcessObj;
 # print "prog=$prog   progArgs=$progCmd  progDir= $progDir\n";
#  Win32::Process::Create($ProcessObj,
 #                               $prog,
 #                               $progCmd,
#								0,
#$flags,
#                                "$progDir")|| die ErrorReport();
#  return($ProcessObj);

#}

sub start_bci2000 {
  my $cparm_file=shift;
  $proc_operat = mySystem($bci_prog_dir . 'operat.exe ' . "--OnConnect \"-LOAD PARAMETERFILE $cparm_file;SETCONFIG\"  --OnSuspend \"-QUIT\"  --OnSetConfig \"-SET STATE Running 1\"");
#  $proc_source = mySystem($bci_prog_dir . 'gUSBampSource.exe',$bci_prog_dir . 'gUSBampSource.exe' . '127.0.0.1');
  $proc_source = mySystem($bci_prog_dir . 'SignalGenerator.exe ' . '127.0.0.1');
  $proc_P3SignalProcessing = mySystem($bci_prog_dir . 'P3SignalProcessing.exe '. '127.0.0.1');
  $proc_P3Speller = mySystem($bci_prog_dir . 'P3Speller.exe ' .'127.0.0.1');
  sleep(1);
}