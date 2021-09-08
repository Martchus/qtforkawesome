#!/usr/bin/perl
use strict;
use warnings;
use utf8;
use feature 'signatures';
no warnings 'experimental::signatures';

# use either YAML::XS or YAML
my $parse_yaml
  =  eval { require YAML::XS; \&YAML::XS::Load }
  // eval { require YAML;     \&YAML::Load     };
die "Unable to locate YAML::XS or YAML: $@" if $@;

# read CLI args
my $yaml_file   = $ARGV[0] // '';
my $output_file = $ARGV[1] // '';
die "The specified file \"$yaml_file\" does not exist.\n" unless -f $yaml_file;
die "No output file specified.\n" unless $output_file;
print "$yaml_file -> $output_file\n";

# read input file manually (instead of using LoadFile) to fix screwed indentation
open my $yaml_fh, '<', $yaml_file or die "Can't open \"$yaml_file\": $!";
read $yaml_fh, my $yaml, -s $yaml_fh;
$yaml =~ s/    - name:/  - name:/g;

# parse YAML
my $icon_defs = $parse_yaml->($yaml);
my $icons     = ref $icon_defs eq 'HASH' ? $icon_defs->{icons} : [];
die "No icons present in \"$yaml_file\".\n" unless ref $icons eq 'ARRAY' && @$icons;
open my $output, '>:encoding(UTF-8)', $output_file or die "Can't open output file \"$output_file\": $!\n";

# define function to camelize IDs
sub camelize ($s) { $s =~ s/(-)?([\w']+)/\u\L$2/g; $s }

# generate enum values
for my $icon (@$icons) {
    my $id         = $icon->{id};
    my $name       = $icon->{name};
    my $unicode    = $icon->{unicode};
    my $created    = $icon->{created} // '?';
    my $categories = ref $icon->{categories} eq 'ARRAY' ? $icon->{categories} : [];
    die "Icon definition in \"$yaml_file\" is incomplete.\n" unless $id && $name && $unicode;

    my $category     = join ',', @$categories;
    my $camelized_id = camelize $id;
    $camelized_id = "Icon$camelized_id" if $camelized_id =~ qr/^\d+.*/;
    print $output "$camelized_id = 0x$unicode, /**< $id: name: $name, created: $created, category: $category */\n";
}
