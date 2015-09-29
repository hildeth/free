#!/usr/bin/env perl
#
# Copyright (C) 1997-2013  Thomas H. Hildebrandt <hilde@cray.com>
# Permission is granted to copy this file in its entirety without modification.
# All other rights reserved.
# (Please contact the author for additional permissions.)
#
# cruiser.pl 1.6.2c
# Set the default directory to '.'.
#
# cruiser.pl 1.6.2b
# Ignore leading blank lines.
#
# CRUISER.pl 1.6.2a
# This version runs under unix again and uses Apache icons.
# <- 
# CRUISER.asp 1.6.2
# This version runs under IIS and uses table formatting.
# <- 
# CRUISER 1.5
#
# Program to list a directory.
# Reads the directory whose name is given as the first argument,
# and formats it for output as HTML.  By default, the "." files is removed
# from the list, directory names are formatted with folder icons,
# and all other file names are formatted as unknown file types.
# 
# Allows you to modify the default behavior by finding a file with the
# extension .@ or .at (case insensitive) in the specified directory.
# The dot-at files contains commands which control the formatting of
# directory entries as follows:
#   A <pattern>
#   <text-lines>
#	Annotate matching files with the given text.
#   B <URL>
#	Use the given URL as a background image.
#   C <pattern>
#   <text-lines>
#       Comments are ignored in the output.
#   F <URL> <name>
#   <text-lines>
#	Fake in a directory entry with the given URL and name.  The text
#	lines are appended as an annotation, just like for the Annotate
#	("A") command.
#   H <header>
#   <text-lines>
#	Creates a table of contents entry with the given title;
#	creates a section header pseudoentry with the same title;
#	and appends the given text.  The pseudo-entry is placed at the
#	end of the current list of files.
#   I <pattern>
#	Ignore filenames matching <pattern>.
#   O <pattern>
#	Move filenames matching pattern to the end of the auxiliary list.
#	The auxiliary list is printed before the main one, and is not 
#	affected by other formatting commands.  Translation: Perform O 
#	operations last of all.
#   S <URL> <name>
#       A signature (author), added as a footnote.
#   T <title>
#   <text-lines>
#	Sets the title of the page to the given <title>;
#	formats the title within the page itself;
#	copies the text lines into a descriptive section at the start
#	of the page.
#   d <pattern>
#	Format as directories files which are directories and also match
#	the given pattern.
#   g <pattern>
#	Format as a GIF (thumbnail icons) files matching the
#	given pattern.
#   s <pattern>
#	Format as symlinks files which are symlinks and also match
#	the given pattern.
#   i <icon> <pattern>
#	Format filenames matching the given pattern using the given icon.
#
# The keyword and <pattern> fields are separated by white space.

# Some utility routines.

format Error_Page =
Content-type: text/html

<HTML><HEAD>
<TITLE>Error</TITLE>
</HEAD><BODY>
<H1>Cruiser Script Error</H1>
^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<~~
$quip
</BODY></HTML>
.

sub error_page ($)
{
    $~ = Error_Page;
    ($quip) = @_;
    write;
    die;
}

sub urlify ($)
{
    local ($url) = @_;
    $url =~ s/ /%20/g;
    $url =~ s/\+/%2b/g;
    return $url;
}

sub deurlify ($)
{
    local ($text) = @_;
    $text =~ s/%20/ /g;
    $text =~ s/%2[bB]/\&/g;
    return $text;
}

# Reads the fields out of the argument array, and stores them as named
# globals.
# We expect two arguments in the query string.
# The first is the directory to display, relative to the page root.
# The second is the absolute path to the page root, as seen by the server.
sub parse_args (@)
{
    ($dirname,		# The path through which the environment of the cgi
     			# script sees the directory of interest.
     $cgi_prefix,       # A prefix to convert this to a path through which
     			# the HTTP daemon can see the same directory.
     ) = @_;

    $title = "Directory of $dirname";

    $dirname = '.' if ($dirname eq '');
}


# Reads the directory into key field of the global hash called %files,
# and sets the value field of each to "".
sub read_directory ()
{
    $longdirname = $dirname;
    if ($cgi_prefix) { $longdirname = "$cgi_prefix$dirname"; }

    if (!opendir(DIR, "$longdirname"))
    {
	my ($gripe) = "Unable to open $longdirname.";
	if ($debug)
	{
	    $gripe .= "<BR>cgi_prefix = $cgi_prefix.\n";
	    $gripe .= "<BR>dirname = $dirname.\n";
	    $gripe .= "<BR>longdirname = $longdirname.\n";
	}
	error_page($gripe);
	return 1;	# Failure.
    }

    # Read a list of files from the directory and sort them.
    @files = sort readdir(DIR);
    closedir(DIR);

    # Create the filename hash.
    @files{@files} = ("") x @files;

    return 0;	# Success.
}


# Finds and reads the dot-at file.
# The preferred file names are ".@", ".at" (case insensitive), "*.@" or
# "*.at" (case insensitive).  Files which occur earlier in the ASCII sort
# order are given preference, so ".AT" < ".At" < ".aT" < ".at", where < is
# read here as "precedes".
#
sub read_commands ()
{
    # Single-line commands have the syntax
    #   <keyword> <pattern>
    #
    # Multi-line commands have the syntax
    #   <keyword> <pattern>
    #   <lines of text>
    #   <blank line>
    #
    # Which is which is determined by the keyword:
    local @multiline_commands = ('A',
                                 'C',
                                 'F',
                                 'S',
                                 'T',
        );

    local $file;
    for (sort keys %files)
    {
    	next if ! /\.(\@|at)$/i;
		$file = $_;
    }

    if ($file)
    {
        open COMMANDS, "$longdirname/$file" ;
        while (<COMMANDS>)
        {
            local $command = $_;
            next if /^$/;
            local ($keyword, $rest) = split /\s+/, $command, 2;
            if (grep(/$keyword/, @multiline_commands))
            {
                # Collects the lines of a multiline command
                # and appends them to the command.
                # (The terminating blank line is elided.)
                while (<COMMANDS>)
                {
                    last if /^$/;
                    $command .= $_;
                }
            }
            push @commands, $command;
        }
    }

    unshift @commands, "I ^\\.\$\n";
}


# Appends a notation to each file matching the given pattern.
sub annotate_files
{
    local ($pattern, $text) = @_;
    for (keys %files)
    {
	$files{$_} .= "\n<TD>$text</TD>" if /$pattern/;
    }
}

# Sets up the background image for the formatted page.
sub set_background
{
    local ($URL) = @_;
    $background = " BACKGROUND=\"$URL\"";
}

# Comments don't do anything right now, but might be printed to stderr
# in the future if a hypothetical debug flag is enabled.
sub comment
{
}

# Fakes in a web link.
sub web_link
{
    local ($entry, $text) = @_;
    local ($URL, $name) = split /\s+/, $entry, 2;
    push @files, $name;	# Add to files array.
    $files{$name} = "<TD><A HREF=\"$URL\">" .
        "<IMG BORDER=0 SRC=\"/icons/world1.gif\"></A></TD>" .
	"<TD><A HREF=\"$URL\">$name</A></TD>" .
        "<TD>$text</TD>";
}

# Removes from the files hash all entries whose keys match the given pattern.
sub delete_files
{
    local ($pattern) = @_;
    for (keys %files)
    {
	delete $files{$_} if /$pattern/;
#	@files = grep ( ! /$pattern/, @files);
    }
}

# Move matching files to the front of the @files array, preserving their
# relative order.
sub order_files
{
    local @headfiles;
    local @tailfiles;
    for (@files)
    {
	if (/$pattern/) {push @headfiles, $_; next;}
	push @tailfiles, $_;
    }
    @movedfiles = (@movedfiles, @headfiles);
    @files = @tailfiles;
}

# Sets the author text.
sub set_author
{
    local ($entry, $text) = @_;
    ($mailto, $author) = split /\s+/, $entry, 2;
    $author_text = $text;
}

# Sets the page title.
sub set_title
{
    ($title, @title_text) = @_;
}

# Formats as a folder each file name that is a directory.
# Ignores the pattern parameter.
sub format_directories
{
    local ($pattern) = @_;
    for (keys %files)
    {
	if (-d "$longdirname/$_")
	{
	    my ($filename) = $_;
	    my ($dir) = $dirname;
	    if ($filename eq "..")
	    {
		# go up one level
		my (@parts) = split /\//, $dir;
		pop @parts;
		$dir = join "/", @parts;
	    } 
	    else
	    {
		$dir = "$dir/$filename";
	    }
	    my ($file_url) = urlify($dir);
	    $files{$filename} =
                "<TD><A HREF=\"cruiser.pl?$file_url+$cgi_prefix\">" .
                "<IMG BORDER=0 SRC=\"/icons/folder.gif\"></A></TD>" .
		"<TD><A HREF=\"cruiser.pl?$file_url+$cgi_prefix\">$filename</A></TD>"
		    if /$pattern/;
	}
    }
}

# Format each matching file using a thumbnail of the gif itself as the icon.
sub format_gifs
{
    local ($pattern) = @_;
    for (keys %files)
    {
	my ($filename) = $_;
	my ($file_url) = urlify($filename);
	$files{$filename} =
            "<TD><A HREF=\"cruiser.pl?$dirname/$file_url\">" .
            "<IMG BORDER=0 SRC=\"$dirname/$filename\"></A></TD>" .
	    "<TD><A HREF=\"cruiser.pl?$dirname/$file_url\">$filename</A></TD>"
		if /$pattern/;
    }
}

# Format as a text file each filename matching the given pattern.
sub format_with_icon
{
    my ($args) = @_;
    local ($icon_file, $pattern) = split /\s+/, $args, 2;
    for (keys %files)
    {
	my ($filename) = $_;
	my ($file_url) = urlify($filename);
	$files{$filename} =
            "<TD><A HREF=\"$dirname/$file_url\">" .
            "<IMG BORDER=0 SRC=\"$icon_file\"></A></TD>" .
	    "<TD><A HREF=\"$dirname/$file_url\">$filename</A></TD>"
		if /$pattern/;
    }
}

# Formats as a symlink each file name that is a symbolic link.
sub format_symlinks
{
    local ($pattern) = @_;
    for (keys %files)
    {
	if (-l "$dirname/$_")
	{
	    my ($filename) = $_;
	    my ($file_url) = urlify($filename);
	    $files{$filename} = "<TD><A HREF=\"cruiser.pl?$dirname/$file_url\">" .
                "<IMG BORDER=0 SRC=\"/icons/link.gif\"></A></TD>" .
		"<TD><A HREF=\"cruiser.pl?$dirname/$file_url\">$filename</A></TD>"
		    if /$pattern/;
	}
    }
}


# Applies each command in the dot-at file.
sub format_commands()
{
    # This is the dispatch table.  It matches up key letters with
    # references to specific formatting functions.
    local %dispatch_table = ( 'A' => \&annotate_files,
			      'B' => \&set_background,
			      'C' => \&comment,
			      'F' => \&web_link,
			      'I' => \&delete_files,
			      'O' => \&order_files,
			      'S' => \&set_author,
			      'T' => \&set_title,
			      'd' => \&format_directories,
			      'g' => \&format_gifs,
			      'i' => \&format_with_icon,
			      's' => \&format_symlinks,
			      ) ;

    foreach $command (@commands)
    {
	local ($keyword,$rest) = split /\s+/, $command, 2;
	local ($pattern,$text) = split /\n/, $rest, 2;
	next if (!exists $dispatch_table{$keyword});
	&{$dispatch_table{$keyword}}($pattern, $text);
    }
}


# Formats every file as an unknown file.
# Then goes back and reformats directories as folders.
# Then, applies the commands read from the dot-at file.
sub format_directory ()
{
    # Default behavior.
    format_with_icon("/icons/unknown.gif");
    format_directories();
    format_symlinks();
    format_gifs("\\.bmp\$");
    format_gifs("\\.gif\$");
    format_gifs("\\.jpe?g?\$");
    format_gifs("\\.wmf\$");
    format_with_icon("/icons/text.gif \\.(txt|s?html?|man)\$");
    format_with_icon("/icons/layout.gif \\.(doc|pdf)\$");
    format_with_icon("/icons/a.gif \\.e\?ps\$");
    # User-defined behavior.
    format_commands();
}


sub print_header ()
{
    print "Content-Type: text/html\n\n";
    print "<HTML>\n";
    print "<HEAD>\n";
    print "<TITLE>$title</TITLE>\n";
    print "</HEAD>\n";
    print "<BODY$background>\n";
    print "<CENTER><H1>$title</H1></CENTER>\n";
    print "<P>@title_text\n<HR>\n";
}

sub print_debug_info()
{
    print "<BR>script_name = $script_name\n";
    print "<BR>script = $script\n";
    print "<BR>cgi_prefix = $cgi_prefix\n";
    print "<BR>dirname = $dirname\n";
    print "<BR>longdirname = $longdirname\n";
    print "<BR>query_string = $query_string\n";
}

sub print_body ()
{
    # Print the header list if it exists.
    if ($#movedfiles + 1)
    {
	print "<TABLE BORDER=0>\n";
	foreach $key (@movedfiles)
	{
	    if (exists $files{$key})
	    {
		print "<TR VALIGN=top>$files{$key}</TR>\n";
	    }
	}
	print "</TABLE><HR>\n";
    }

    # Print the remaining list of files.
    print "<TABLE BORDER=0>\n";
    foreach $key (@files)
    {
	if (exists $files{$key})
	{
	    print "<TR VALIGN=top>$files{$key}</TR>\n";
	}
    }
    print "</TABLE>\n";
}

sub print_trailer ()
{
    print "<HR>\n";
    if ($mailto)
    {
	print "This page maintained by:\n";
	print "<ADDRESS><A HREF=\"$mailto\">$author</A></ADDRESS>\n";
	print "$author_text\n";
    }
    print "<P>Produced by CRUISER 1.6.2c.<BR>\nCopyright &copy; 1997-2013 ";
    print "<A HREF=\"mailto:hilde\@cray.com\">Thomas H. Hildebrandt</A>\n";
    print "All rights reserved.\n";
    print "</BODY>\n";
    print "</HTML>\n";
}

sub print_directory()
{
    print_debug_info() if $debug;
    print_header();
    print_body();
    print_trailer();
}


########################################################################
# Here's the main behavior.
#
{
    parse_args(@ARGV);
    last if read_directory();
    read_commands();
    format_directory();
    print_directory();
}

1;


