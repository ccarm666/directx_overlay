# will make directx parm files to drive communicator5 from weight file see usage subroutine at bottom for example
use strict;
use warnings;
if (scalar (@ARGV) != 2) { &usage();}
my @necessary_lines=
(
'Application:Checkerboard:P3SpellerTask int ExtApp= 1 0 0 1 // Use external UDP presentation application (0=no, 1=yes) (boolean)',
'Application:Checkerboard:P3SpellerTask string UDPHost= 127.0.0.1:12000 // Host:Port of external presentation application',
'Connector:ConnectorInput string ConnectorInputAddress= localhost:20321 localhost:20320 % % // IP address/port to read from, e.g. localhost:20320',
'Connector:ConnectorInput list ConnectorInputFilter= 1 * // list of state names or signal elements to allow, "*" for any, signal elements as in "Signal(1,0)"',
'Connector:ConnectorOutput string ConnectorOutputAddress= localhost:20320 localhost:20320 % % // IP address/port to write to, e.g. localhost:20320'
);

my $filename = $ARGV[0];
my $dir_name = $ARGV[1];
my $name="communicator_directx_";
my @row=(2,2,3,2,3,4,6,7,7,8,8); # will make communicator_directx_4.prm thru communicator_directx_80.prm
my @col=(2,3,3,5,5,7,6,7,8,8,10);
open (my $handle, '<', $filename)
or die "Could not open file '$filename' $!";
chomp(my @lines = <$handle>);
close $handle;
exit(0) if(&check_nec());
my $num; # number of matrix elements
for (my $j=0;$j<scalar(@row);$j++) {
  $num=$row[$j]*$col[$j];
  my $fullname = $dir_name . '\\' . $name . "$num" . ".prm";
  if(-e $fullname) {
    print "file exists\n";
  }
  open (OUT , ">",$fullname) or die "Cannot open $fullname: $!";
  for(my $i=0;$i< scalar(@lines);$i++) {   
    if ( $lines[$i] =~ /Application:Speller%20Targets:P3SpellerTask intlist NumMatrixColumns= 1/ ) {
	     $lines[$i] =  'Application:Speller%20Targets:P3SpellerTask intlist NumMatrixColumns= 1 ' . $col[$j] . ' 6 1 % // display matrices\' column number(s)';
        print $lines[$i],"\n";
	} elsif ($lines[$i] =~ /Application:Speller%20Targets:P3SpellerTask intlist NumMatrixRows= 1 /) {
	     $lines[$i] =  'Application:Speller%20Targets:P3SpellerTask intlist NumMatrixRows= 1 '    . $row[$j] . ' 6 0 % // display matrices\' row number(s)';
         print $lines[$i],"\n";
	} elsif ($lines[$i] =~ /Application:Speller%20Targets:P3SpellerTask matrix TargetDefinitions= /) {
         $lines[$i]	= 'Application:Speller%20Targets:P3SpellerTask matrix TargetDefinitions= ' . "$num 5 ";
		 my $matrix ="";
		 for (my $k=1;$k<=$num;$k++) {
		   $matrix = $matrix .   "$k $k 1 \% \% ";
		 }
		 $matrix = $matrix . '// speller target properties';
		 $lines[$i] = $lines[$i] . $matrix ;
		 print $lines[$i],"\n";
	} elsif ($lines[$i] =~ /Application:Result%20Processing:StimulusTask int InterpretMode= 2/) {
	         $lines[$i] = "Application:Result%20Processing:StimulusTask int InterpretMode= 1 0 0 2 // interpretation of results: 0 none, 1 online free mode, 2 copy mode (enumeration)";
         print $lines[$i],"\n";
	} elsif ($lines[$i] =~ /Application:Window:ApplicationBase int WindowLeft= /) {
	         $lines[$i] = 'Application:Window:ApplicationBase int WindowLeft= -1400 0 % % // screen coordinate of application window\'s left edge';
			 print $lines[$i],"\n";
	}
	print OUT $lines[$i],"\n";
  }
  close(OUT);
}
sub check_nec {
  my $num_errors=0;
  my @nc = (0) x scalar(@necessary_lines);
  my @qm_necessary_lines = map(quotemeta,@necessary_lines); #so missing line error message are readable
  for(my $i=0;$i< scalar(@lines);$i++){ 
    for(my $k=0;$k<scalar(@necessary_lines);$k++) {
	   if ($lines[$i]=~/$qm_necessary_lines[$k]/){$nc[$k]++;}
	}
   }
   for(my $k=0;$k<scalar(@necessary_lines);$k++) {
     if(!$nc[$k]){print "missing line = " . $necessary_lines[$k] . "\n";$num_errors++;}
	 if($nc[$k]> 1){print "duplicate line = " . $necessary_lines[$k] . "\n";$num_errors++;}
  }
 return($num_errors)  
}
sub usage {
  print "Usage: $0 template_source_filename  output_directory\n";
  print "example: $0 ",  'c:\BCIHomeSystemFiles\VA_BCI2000\parms\weights_communicator_28_5.prm ' ,  ,".\\" ,"\n";
  exit(1);
}
#Application:Speller%20Targets:P3SpellerTask matrix TargetDefinitions= 4 5 1 01 1 % % 2 02 1 % % 3 03 1 % % 4 04 1 % % // speller target properties
#Application:Speller%20Targets:P3SpellerTask matrix TargetDefinitions= 56 5 1 1 1 % % 2 2 1 % % 3 3 1 % % 4 4 1 % % 5 5 1 % % 6 6 1 % % 7 7 1 % % 8 8 1 % % 9 9 1 % % 10 10 1 % % 11 11 1 % % 12 12 1 % % 13 13 1 % % 14 14 1 % % 15 15 1 % % 16 16 1 % % 17 17 1 % % 18 18 1 % % 19 19 1 % % 20 20 1 % % 21 21 1 % % 22 22 1 % % 23 23 1 % % 24 24 1 % % 25 25 1 % % 26 26 1 % % 27 27 1 % % 28 28 1 % % 29 29 1 % % 30 30 1 % % 31 31 1 % % 32 32 1 % % 33 33 1 % % 34 34 1 % % 35 35 1 % % 36 36 1 % % 37 37 1 % % 38 38 1 % % 39 39 1 % % 40 40 1 % % 41 41 1 % % 42 42 1 % % 43 43 1 % % 44 44 1 % % 45 45 1 % % 46 46 1 % % 47 47 1 % % 48 48 1 % % 49 49 1 % % 50 50 1 % % 51 51 1 % % 52 52 1 % % 53 53 1 % % 54 54 1 % % 55 55 1 % % 56 56 1 % % // speller target properties
#Application:Result%20Processing:StimulusTask int InterpretMode= 1 0 0 2 // interpretation of results: 0 none, 1 online free mode, 2 copy mode (enumeration)
#Application:Result%20Processing:StimulusTask int InterpretMode= 2 0 0 2 // interpretation of results: 0 none, 1 online free mode, 2 copy mode (enumeration)
#Application:Speller%20Targets:P3SpellerTask intlist NumMatrixColumns= 1 8 6 1 % // display matrices' column number(s)
#Application:Speller%20Targets:P3SpellerTask intlist NumMatrixColumns= 1 8 6 1 % // display matrices' column number(s)
#Application:Speller%20Targets:P3SpellerTask intlist NumMatrixRows= 1 7 6 0 % // display matrices' row number(s)
#Application:Speller%20Targets:P3SpellerTask intlist NumMatrixColumns= 1 2 6 1 % // display matrices' column number(s)
#Application:Speller%20Targets:P3SpellerTask intlist NumMatrixRows= 1 2 6 0 % // display matrices' row number(s)
#Application:Text%20Window:P3SpellerTask int TextWindowLeft= 640 0 0 % // Text Window X location
#Application:Window:ApplicationBase int WindowLeft= 0 0 % % // screen coordinate of application window's left edge

#necessary lines
#Application:Checkerboard:P3SpellerTask int ExtApp= 1 0 0 1 // Use external UDP presentation application (0=no, 1=yes) (boolean)
#Application:Checkerboard:P3SpellerTask string UDPHost= 127.0.0.1:12000 // Host:Port of external presentation application
#Connector:ConnectorInput string ConnectorInputAddress= localhost:20321 localhost:20320 % % // IP address/port to read from, e.g. localhost:20320
#Connector:ConnectorInput list ConnectorInputFilter= 1 * // list of state names or signal elements to allow, "*" for any, signal elements as in "Signal(1,0)"
#Connector:ConnectorOutput string ConnectorOutputAddress= localhost:20320 localhost:20320 % % // IP address/port to write to, e.g. localhost:20320

