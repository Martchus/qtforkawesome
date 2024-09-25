#!/usr/bin/perl
use strict;
use warnings;
use utf8;

# use either YAML::XS or YAML
my @errors;
my $parse_yaml = eval { require YAML::XSS; \&YAML::XS::Load };
push @errors, $@ if $@;
if (!defined $parse_yaml) {
    $parse_yaml = eval { require YAML; \&YAML::Load };
    push @errors, $@ if $@;
}
die "Unable to locate YAML::XS or YAML:\n" . join('', @errors) unless defined $parse_yaml;

# read CLI args
my $yaml_file        = $ARGV[0] // '';
my $enum_output_file = $ARGV[1] // '';
my $id_output_file   = $ARGV[2] // '';
die "The specified file \"$yaml_file\" does not exist.\n" unless -f $yaml_file;
die "No enum output file specified.\n" unless $enum_output_file;
die "No ID output file specified.\n"   unless $id_output_file;

# read input file manually (instead of using LoadFile) to fix screwed indentation
open my $yaml_fh, '<', $yaml_file or die "Can't open \"$yaml_file\": $!";
read $yaml_fh, my $yaml, -s $yaml_fh;
$yaml =~ s/    - name:/  - name:/g;

# parse YAML
my $icon_defs = $parse_yaml->($yaml);
my $icons     = ref $icon_defs eq 'HASH' ? $icon_defs->{icons} : [];
die "No icons present in \"$yaml_file\".\n" unless ref $icons eq 'ARRAY' && @$icons;
open my $enum_output, '>:encoding(UTF-8)', $enum_output_file or die "Can't open output file \"$enum_output_file\": $!\n";
open my $id_output,   '>:encoding(UTF-8)', $id_output_file   or die "Can't open output file \"$id_output_file\": $!\n";

# generate enum values
for my $icon (@$icons) {
    my $id         = $icon->{id};
    my $name       = $icon->{name};
    my $unicode    = $icon->{unicode};
    my $created    = $icon->{created} // '?';
    my $categories = ref $icon->{categories} eq 'ARRAY' ? $icon->{categories} : [];
    die "Icon definition in \"$yaml_file\" is incomplete.\n" unless $id && $name && $unicode;

    my $category     = join ',', @$categories;
    my $camelized_id = $id;
    $camelized_id =~ s/(-)?([\w']+)/\u\L$2/g;
    $camelized_id = "Icon$camelized_id" if $camelized_id =~ qr/^\d+.*/;
    print $enum_output "$camelized_id = 0x$unicode, /**< $id: name: $name, created: $created, category: $category */\n"
        or die "Can't write to file \"$enum_output_file\": $!\n";
    print $id_output "\{ QStringLiteral(\"$id\"), Icon::$camelized_id \},\n"
        or die "Can't write to file \"$id_output_file\": $!\n";
}

close $enum_output or die "Can't close to file \"$enum_output_file\": $!\n";
close $id_output   or die "Can't close to file \"$id_output_file\": $!\n";
