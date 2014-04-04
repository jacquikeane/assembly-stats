#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#include "fasta.h"
#include "fastq.h"
#include "filetype.h"
#include "stats.h"

using namespace std;

struct CmdLineOps
{
    unsigned long minLength;
    int infileStartIndex;
    short outFormat;
};

void parseOptions(int argc, char** argv, CmdLineOps& ops);

int main(int argc, char* argv[])
{
    CmdLineOps ops;
    parseOptions(argc, argv, ops);

    for (int i = ops.infileStartIndex; i < argc; i++)
    {
        Stats s(argv[i], ops.minLength);
        cout << s.toString(ops.outFormat);
        if (ops.outFormat == FORMAT_TAB)
        {
            ops.outFormat = FORMAT_TAB_NO_HEAD;
        }

    }

    return 0;
}


void parseOptions(int argc, char** argv, CmdLineOps& ops)
{
    string usage;
    ops.minLength = 1;
    ops.infileStartIndex = 1;
    ops.outFormat = FORMAT_HUMAN;

    usage = "usage: stats [options] <list of fasta/q files>\n\n\
Reports sequence length statistics from fasta and/or fastq files\n\n\
options:\n\
-l <int>\n\tMinimum length cutoff for each sequence.\n\
\tSequences shorter than the cutoff will be ignored [1]\n\
-s\n\tPrint 'grep friendly' output\n\
-t\n\tPrint tab-delimited output\n\
-u\n\tPrint tab-delimited output with no header line\n\
";

    if (argc < 2)
    {
        cerr << usage;
        exit(1);
    }

    while (argv[ops.infileStartIndex][0] == '-')
    {
        if (strcmp(argv[ops.infileStartIndex], "-s") == 0)
        {
            ops.outFormat = FORMAT_GREPPY;
            ops.infileStartIndex++;
        }
        else if (strcmp(argv[ops.infileStartIndex], "-t") == 0)
        {
            ops.outFormat = FORMAT_TAB;
            ops.infileStartIndex++;
        }
        else if (strcmp(argv[ops.infileStartIndex], "-u") == 0)
        {
            ops.outFormat = FORMAT_TAB_NO_HEAD;
            ops.infileStartIndex++;
        }
        else if (strcmp(argv[ops.infileStartIndex], "-l") == 0)
        {
            ops.minLength = atoi(argv[ops.infileStartIndex + 1]);
            ops.infileStartIndex += 2;
        }
        else
        {
            cerr << "error parsing options, somewhere around this: " << argv[ops.infileStartIndex] << endl;
            exit(1);
        }
    }
}

