static char *HELP_MSG="\
Spew generates massive sequential lists of passwords, for use as\
\ninput for cracking software. Best suited for single brute-force\
\nattacks. It is your responsibility to use in a safe and legal\
\nmanner.\
\n\nOptions:\
\n\n-c --charset <charset>\
\n\tCharset to use for cracking\
\n\n-n --numeric\
\n\tUse numeric characters only\
\n\n-a --alphanum\
\n\tUse alphanumeric characters only (default)\
\n\n-r --resume [from]\
\n\tStart from an arbitrary start point, given as a number.\
\n\tFor example, if interrupted after 1 million guesses, you\
\n\tcould type --resume 1000000\
\n\n-b --blocksize [number]\
\n\tSpew allows ghetto parallelization, meaning you can set\
\n\tup a cracking cluster limited or no bandwidth between\
\n\tmachines, by dividing the task up beforehand. Spew splits\
\n\tthe password space into \"blocks\", and only computes its\
\n\tshare of passwords (\"lines\") in each block. This option\
\n\tsets the total number of lines per block, across all\
\n\tmachines involved.\
\n\n-l --my-lines [number]\
\n\tHow many lines to compute per block.\
\n\n-o --offset --offset-lines [number]\
\n\tWhere in the block to start computing. Use this to give\
\n\tinstance/machine a different part of the block to work on.\
\n\tBe careful when setting your offsets; incorrect values\
\n\twill result in duplicated effort and/or missed guesses!\
\n\nExamples:\
\n\nSimple cracking on a single machine, for a numeric password\
\n\tspew --numeric\
\n\nDistributed cracking of an alphanumeric password, between 3\
\nmachines, each performing an equal share of the work.\
\n\thost1%% spew --blocksize 3 --my-lines 1\
\n\thost2%% spew --blocksize 3 --my-lines 1 --offset 1\
\n\thost3%% spew --blocksize 3 --my-lines 1 --offset 2\
\n\nDistributed cracking of an alphanumeric password, between 3\
\nmachines, where machine 2 is 4 times as fast as the others. In such\
\na scenario, make use of your cracking software's benchmarking\
\nfeature, if available, to determine this.\
\n\thost1%% spew --blocksize 7 --my-lines 1\
\n\thost2%% spew --blocksize 7 --my-lines 4 --offset 1\
\n\thost3%% spew --blocksize 7 --my-lines 1 --offset 6\
\n\n";
