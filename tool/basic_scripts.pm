#!/usr/bin/perl -w
package basic_scripts;

use strict;
use Exporter;
use vars qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);

$VERSION     = 1.00;
@ISA         = qw(Exporter);
@EXPORT      = ();
@EXPORT_OK   = qw(createBasicScript replace_file);
%EXPORT_TAGS = ( DEFAULT => [qw(&createBasicScript)] );

sub createBasicScript {
  my $path = $_[0];
  my $bash = $path.'/'.'fittest.sh';
  my $cfg = $path.'/'.'fitoptions.cfg';
  my $icc = $path.'/'.'species_lists.icc';
  createBash($bash);
  createCFG($cfg);
  createICC($icc);
}
sub createBash {
  my $bash = $_[0];
  open BASH, '>', $bash or die "cannot create file: $bash";
print BASH  <<EOF;
#!/bin/bash
EXEC="./borexino"
\$EXEC INPUTROOT HIST1 CFG ICC | tee data.log
EOF
  close BASH;
}
sub createCFG {
  my $cfg = $_[0];
  open CFG, '>', $cfg  or die "cannot create file: $cfg";
print CFG <<EOF;
fit_variable = npe
norm_variable = true
fiducial_mass = 0
spectra_pdfs = analytical # Options: "montecarlo" or "analytical"(Default)
histogram_binwidth = 1 # Bin width in pe
fit_verbosity = 2	# Options: -1 (quiet) 0 1 2 (extremely verbose)
fit_variable_MC = charge
weighted_response = false
number_of_bins_for_weighted_response = 10
applying_mask = false
mask_applied_in_rf = false
n_used_pmts_histo_name = h_n_used_pmts
n_used_pmts_po210_histo_name = h_n_used_pmts_weighted
n_used_pmts_tfc0_histo_name = h_n_used_pmts
n_used_pmts_po210_tfc0_histo_name = h_n_used_pmts_weighted
n_used_pmts_tfc1_histo_name = h_n_used_pmts
n_used_pmts_po210_tfc1_histo_name = h_n_used_pmts_weighted
n_used_charge_histo_name = NUSED1
n_used_charge_po210_histo_name = NUSED2
beta_response_function = new_modified_gaussian
beta_resolution_0_guess = G1
beta_resolution_0_min = G1
beta_resolution_0_max = G1
beta_resolution_1_guess = 2.2
beta_resolution_1_min = 0
beta_resolution_1_max = 5
beta_resolution_2_guess = G3
beta_resolution_2_min = G3
beta_resolution_2_max = G3
beta_resolution_3_guess = 0.
beta_resolution_3_min = 0
beta_resolution_3_max = 0
beta_resolution_4_guess = G4
beta_resolution_4_min = G4MIN
beta_resolution_4_max = G4MAX
beta_resolution_5_guess = G5
beta_resolution_5_min = G5
beta_resolution_5_max = G5
alpha_subtracted = false
c11_subtracted = false
alpha_response_function = modified_gaussian
use_alpha_response_function = false
alpha_resolution_0_guess = 1.2
alpha_resolution_0_min = 0.
alpha_resolution_0_max = 3.0
alpha_resolution_1_guess = 0.7
alpha_resolution_1_min = 0.
alpha_resolution_1_max = 30.0
alpha_resolution_2_guess = 0
alpha_resolution_2_min = 0.
alpha_resolution_2_max = 0
ly_fixed = LYFIXED
ly_guess = LYVALUE
kb = 0.0109
fCher = 3.85
geometric_correction = 0.122   # 0.122 is the value hard-coded till now.
free_quenching = true
NL_0_guess = 0.0119386
NL_0_name = kb
NL_0_fixed = true
NL_2_guess = 1.47
NL_2_name = p_dn
NL_2_fixed = true
NL_3_guess = 0.011
NL_3_name = p_miscalib
NL_3_fixed = true
NL_4_guess = 2.4
NL_4_name = p_quadr
NL_4_fixed = true
NL_1_name = dummy
NL_1_fixed = true
NL_5_name = fCher
NL_5_guess = 3.615
NL_5_fixed = true
NL_6_name = dummy
NL_6_fixed = true
NL_7_name = dummy
NL_7_fixed = true
quenching_parameterization = newechidna
c11_quenching = 0.88
c11_quenching_fixed = false
po210_quenching = 0.0745631
po210_quenching_fixed = false
ext_bckgd_eff_ly = 1.0228
MC_ext_bkg = MCEXTERNALS
pileup_eff_ly = 0.935
pileup_eff_ly_fixed = true
npe_offset = 0
npe_offset_fixed = true
montecarlo_spectra_file = MCFILE
hybrid_montecarlo_spectra_file = MCHYBRID
montecarlo_interpolation = off
c14_spectrum = universal_screening
c14_shape_factor = 1.24
kr85_spectrum = andrea
bi210_spectrum = aldo
pb214_spectrum = aldo
no_lead_cut = no
meantime_cut = no
meantime_cut_correction_c1 = 0.974
meantime_cut_correction_c2 = -2.11E-3
use_remaining_pb214 = false
minimum_energy = 140
maximum_energy = 750
neutrino_magnetic_moment = no
penalty_factor_Kr = off
minimization_method = likelihood
minimization_routine = minuit
multivariate_ps_fit = false
multivariate_ps_fit_min = 450
multivariate_ps_fit_max = 650
multivariate_ps_fit_bins = 1
multivariate_ps_fit_smooth = 0 
multivariate_rdist_fit = false
multivariate_rdist_fit_min = 500 
multivariate_rdist_fit_max = 900
multivariate_rdist_fit_bins = 16
multivariate_rdist_fit_smooth = 0
complementary_histo_fit = false
complementary_histo_name = HIST2
save_fit_results = true
convolve_dark_noise = false
dark_noise_threshold = 100
dark_noise_window = win1
synthpup_file = P91011_Synthetic_Spectra.root
synthpup_hist = h_synth_spec_5_dt1
sys_errors_scale = 0
fit_results_output = periodAll.root
EOF
  close CFG;
}
sub createICC {
  my $icc = $_[0];
  open ICC, '>', $icc  or die "cannot create file: $icc";
print ICC <<EOF;
    { "synthpup",-1,kViolet,kDashed,2, 135,    "penalty", 135, 4 },
    { "C11",   -1,   kMagenta, kSolid, 	 2, 2.5,      "free", 0, 100 },
    { "C11_2", -1,   kMagenta, kSolid, 	 2, 50,     "free", 0, 200 },
{ "C10_2", -1,   kGreen,   kSolid,    2, 1.1,    "penalty", 1.1, 0.1 },
    //{ "Kr85",  -1,  kBlue,    kSolid,    2, 5,     "penalty", 0, 2.5 },
    { "Kr85",  -1,  kBlue,    kSolid,    2, 5,     "free", 0, 250 },
{ "He6_2", -1,   kRed,     kSolid,    2, 0.4,    "penalty", 4, 0.5 },
    { "Bi210", -1, kSpring,  kSolid,    2, 20,     "free", 0, 100 },
  { "nu(pp)",-1,    kCyan,    kSolid,    2, 133.0,  "fixed",0, 300 },
    { "nu(Be7)",-1,  kRed,     kSolid,    2, 48,     "free", 0, 100 },
    { "nu(pep)",-1,  kCyan,    kSolid,    2, 2.80,   "fixed", 0, 50 },
    { "nu(CNO)",-1,  kCyan,    kSolid,    2, 5.36,   "fixed",0, 50 },
    { "Po210", -1, kOrange,  kSolid,    2, 120,  "free", 0.01,10000 },
    { "Po210_2", -1, kOrange,  kSolid,    2, 120,  "free", 0.01,10000 },
    { "Ext_Tl208",-1,kAzure,kSolid, 2, 3.5,    "free", 0, 100 },
    //{ "Ext_Tl208",-1,kAzure,kSolid, 2, 2.1,    "penalty", 2.1, 0.5},
    { "Ext_Bi214",-1,kAzure,kSolid, 2, 2.0,    "free", 0, 10 },
    { "Ext_K40",-1,kAzure, kSolid,    2, 0.8,   "free", 0, 10 },
EOF
  close ICC
}
sub read_file {
  my ($filename) = @_;

  open my $in, '<:encoding(UTF-8)', $filename or die "Could not open '$filename' for reading $!";
  local $/ = undef;
  my $all = <$in>;
  close $in;

  return $all;
}

sub write_file {
  my ($filename, $content) = @_;

  open my $out, '>:encoding(UTF-8)', $filename or die "Could not open '$filename' for writing $!";;
  print $out $content;
  close $out;

  return;
}
sub replace_file {
  my ($filename, $old, $new, $pre) = @_;
  my $content = read_file($filename);
  if($pre) {
    my @contents = split /^/, $content;
    s/$old/$new/g for grep { /$pre/ } @contents;
    $content = join('',@contents);
  } else {
    $content =~ s/$old/$new/g;
  }
  write_file($filename, $content);
}
1;
