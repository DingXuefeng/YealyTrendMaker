#!/usr/bin/perl -w
use strict;
use warnings;

use File::Path 'rmtree','make_path';
use File::Copy 'move';
use File::Basename qw(dirname);
use Cwd  qw(abs_path);
use lib dirname(dirname abs_path $0);
use basic_scripts qw(createBasicScript replace_file);
my $testNo = @ARGV? $ARGV[0]*6+0 : 0;
my $current_job;
my $period;
my ($fixLY, $fitMax, $withExt, $var, $Krpen, $FV, $comp);
sub say { print @_, "\n"; }
sub fixLY {
  my $fixLY = $_[0];
  $current_job = join('-',$current_job,$fixLY);
  my $LYVALUE;
  if($var eq 'charge') {
    $LYVALUE = 0.5096; #All, see v83
  } elsif($var eq 'charge_geo') {
    $LYVALUE = 0.5091; #All, see v83
  } elsif($var eq 'charge_zcorr') {
    $LYVALUE = 0.5115; #All, see v109
  } else {
    die 'unknown var [$var], cannot fixLY';
  }
  replace_file('fitoptions.cfg','LYVALUE',$LYVALUE);
  if($fixLY eq 'fixLY') {
    replace_file('fitoptions.cfg','LYFIXED','true');
  } else {
    replace_file('fitoptions.cfg','LYFIXED','false');
  } 
  if($fixLY eq 'fixBe7') {
    replace_file('species_lists.icc','free','fixed','Be7');
    replace_file('species_lists.icc','48','48.6','Be7');
  }
#  if ($fixLY) { say "fixLY"; } else { say "not fix LY"; }
}
sub fixg1g2g3 {
  replace_file('fitoptions.cfg','G1',1.3);
  if( $var eq 'charge') {
    replace_file('fitoptions.cfg','G3',0);
    replace_file('fitoptions.cfg','G5',6);
  } else {
    replace_file('fitoptions.cfg','G3',0.184);
    replace_file('fitoptions.cfg','G5',0);
  }
}
sub setFitRange {
  my ($fitmin,$fitmax) = (@_);
  $current_job = join("-",$current_job,$fitmin,$fitmax);
#  say "set fit range to [$fitmin,$fitmax]";
}
sub withExt {
  my $withExt = $_[0];
  $current_job = join("-",$current_job,$withExt?"withExt":"withoutExt");
  if($withExt) {
    replace_file('fitoptions.cfg','G4',2);
    replace_file('fitoptions.cfg','G4MIN',0);
    replace_file('fitoptions.cfg','G4MAX',10);
    replace_file('fitoptions.cfg','MCEXTERNALS','false');
  } else {
    replace_file('fitoptions.cfg','G4',0);
    replace_file('fitoptions.cfg','G4MIN',0);
    replace_file('fitoptions.cfg','G4MAX',0);
    replace_file('species_lists.icc','{','// {','Ext');
  }
#  if ($withExt) { say "withExt"; } else { say "no ext"; }
}
sub Krpen {
  my $Krpen = $_[0];
  $current_job = join("-",$current_job,$Krpen?"Krpen":"noKrpen");
  if($Krpen) {
    replace_file('species_lists.icc','//{','{','Kr85.*penalty');
    replace_file('species_lists.icc','{','//{','Kr85.*free');
  }
#  if ($Krpen) { say "Krpen"; } else { say "no Krpen"; }
}
sub comp {
  my $comp = $_[0];
  $current_job = join("-",$current_job,$comp?"comp":"nocomp");
  if(!$comp) {
    replace_file('species_lists.icc','2.5,      "free", 0, 100','26.2,      "penalty", 26.2, 1.3','C11');
  }
#  if ($comp) { say "comp"; } else { say "no comp"; }
}
sub setFitVar{
  my $var = $_[0];
  $current_job = join("-",$current_job,$var);
  if($comp) {
    replace_file('fitoptions.cfg','false','true','^complementary_histo_fit');
    replace_file('fitoptions.cfg','false','true','^c11_subtracted');
  }
  if($var eq 'charge') {
    if($comp) {
      replace_file('fittest.sh','HIST1','pp/final_charge_pp_0');
      replace_file('fitoptions.cfg','HIST2','pp/final_charge_pp_1');
    } else {
      replace_file('fitoptions.cfg','HIST1','pp/final_charge_pp');
    }
    replace_file('fitoptions.cfg','NUSED1','pp/h_n_used_charge');
    replace_file('fitoptions.cfg','NUSED2','pp/h_n_used_charge_weighted');
  } elsif($var eq 'charge_geo') {
    if($comp) {
      replace_file('fittest.sh','HIST1','pp/geometrical_correction/final_charge_pp_geo_0');
      replace_file('fitoptions.cfg','HIST2','pp/geometrical_correction/final_charge_pp_geo_1');
    } else {
      replace_file('fittest.sh','HIST1','pp/geometrical_correction/final_charge_pp_geo');
    }
    replace_file('fitoptions.cfg','NUSED1','pp/geometrical_correction/h_n_used_charge_pp_geo');
    replace_file('fitoptions.cfg','NUSED2','pp/geometrical_correction/h_n_used_charge_weighted_geo');
  } elsif($var eq 'charge_zcorr') {
    if($comp) {
      replace_file('fittest.sh','HIST1','pp/z_correction/final_charge_pp_zcorr_0');
      replace_file('fitoptions.cfg','HIST2','pp/z_correction/final_charge_pp_zcorr_1');
    } else {
      replace_file('fittest.sh','HIST1','pp/z_correction/final_charge_pp_zcorr');
    }
    # buggy bx_nusol!!! will be fixed later
    replace_file('fitoptions.cfg','NUSED1','pp/z_correction/h_n_used_charge_pp_zcorr');
    replace_file('fitoptions.cfg','NUSED2','pp/z_correction/h_n_used_charge_weighted_zcorr');
  }
#  say "set fit var to [$var]";
}
sub setFitFV{
  my $FV = $_[0];
  $current_job = join("-",$current_job,$FV);
  my $inputRoot;
  if($FV eq 'pep') {
    $inputRoot = "\${BXDATA}/input_final/Period${period}_FVpep_TFCDD.root";
  } elsif ($FV eq 'Be7N') {
    $inputRoot = "\${BXDATA}/input/FVBe7North/Period${period}_FVBe7_TFCDD.root";
  } elsif ($FV eq 'Be7S') {
    $inputRoot = "\${BXDATA}/input/FVBe7South/Period${period}_FVBe7_TFCDD.root";
  } else {
    die "unknown FV [$FV]";
  }
  replace_file('fittest.sh','INPUTROOT',$inputRoot);
#  say "set fit FV to [$FV]";
}
sub generate {
  ($fixLY, $fitMax, $withExt, $var, $Krpen, $FV, $comp) = (@_);
  $current_job = int($testNo/6)+1;
  chdir '/tmp';
  rmtree(['/tmp/generate_scripts_MassiveFit']);
  mkdir '/tmp/generate_scripts_MassiveFit';
  chdir '/tmp/generate_scripts_MassiveFit';
  createBasicScript(".");
  fixLY($fixLY);
  fixg1g2g3;
  setFitRange(140,$fitMax);
  withExt($withExt);
  setFitVar($var);
  Krpen($Krpen);
  setFitFV($FV);
  comp($comp);
  my $destination = '/tmp/massive_scripts'.'/'.$current_job.'/'.$period;
  say $destination;
  make_path $destination;
  move '/tmp/generate_scripts_MassiveFit', $destination;
  say "[$current_job/$period] done";
  $testNo+=1;
}
for my $comp ('','complementary') {
  for my $FV ('pep','Be7N','Be7S') {
    for my $Krpen ('','with Kr penalty') {
      for my $var ('charge','charge_geo','charge_zcorr') {
        for my $ext ('','with External backgrounds') {
          for my $fixLY ('fixLY','fixBe7','free') {
            for my $loopperiod (2012..2016,'All') {
              $period = $loopperiod;
              generate($fixLY,$ext?1500:650,$ext,$var,$Krpen,$FV,$comp);
            }
          }
        }
      }
    }
  }
}
