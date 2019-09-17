#!/usr/bin/perl
use File::Basename;
$name=$ARGV[0];
#if(!$ARGV[0]) {
#    print "No file to save\n";
#   exit;
#  }
mkdir "./save" if(!(-e "./save"));
$_=scalar(localtime time);
s/\s+/_/g;
s/:/_/g;
$mydir='.\\save\\' . $_ . '\\';
system("mkdir $mydir");
print "new directory is $mydir\n";
my @files = glob('*.cpp');
foreach my $file (@files){
    print basename($file), "\n";
	$name = basename($file);
	$name2=$mydir . $name;
	system("cp $name $name2");
}
my @files = glob('*.h');
foreach my $file (@files){
    print basename($file), "\n";
	$name = basename($file);
	$name2=$mydir . $name;
	system ("cp $name $name2");
}
#$name2= './save/' . $name  . '_' . $_;
#$msg= "checkpointing $name to $name2";
#print "$msg\n";
#system("cp $name $name2");
