/**
 * \file ayb_usage.h
 * Usage text for AYB.
 * Include as bulk text in options print_usage.
 */
 
//"12345678901234567890123456789012345678901234567890123456789012345678901234567890\n"
"\n"
PROGNAME " Advanced Base Calling for Next-Generation Sequencing Machines\n"
"\n"
"Usage:\n"
"\t" PROGNAME " [-b blockstring] [-d input format] [-e log file] [-f output format]\n"
"\t    [-g generr] [-i input path] [-k] [-l log level] [-n iterations]\n"
"\t    [-o output path] [-p threads] [-q] [-r] [-s header] [-t factor]\n"
"\t    [-w level] [-z limit] [-A Parameter A] [-K spike-in path]\n"
"\t    [-M Crosstalk] [-N Noise] [-Q quality tab] [-S sample name]\n"
"\t    <prefix[+]/lane tile range> [<prefix[+]/lane tile range> ...]\n"
"\t" PROGNAME " --help\n"
"\t" PROGNAME " --licence\n"
"\t" PROGNAME " --version\n"
"\n"
"Example:\n"
"\t" PROGNAME " <intensities>\n"
"will process cif file \'intensities\' in one block using 5 iterations and\n"
"output a fastq file, both in the current directory with log messages to stderr\n"
"\n"
