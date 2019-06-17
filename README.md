`split` is a simple `tee` replacement for Windows

# Usage

`split [options] [output_file]`

Options:
* `-a`, `--append`: append to file instead of overwriting it
* `--help`: basic usage information

Output file is optional. If absent, `split` will simply pass everything down the pipe.

# License

Code is provided under the BSD 2-Clause License. See `LICENSE` file for details.

# Building

Open provided solution in Visual Studio 2019 or newer and run Build > Build Solution (F7).
Alternatively, if environment is set up properly, MSBuild can be used to build by pointing
it to the solution file (`split.sln`).

# Differences from tee

There are a few:

* only a single file can be provided as an argument (if more are needed just pipeline
multiple *split*s)
* there's no `-i` parameter to ignore interrupts
* no `--version` parameter
* diagnostic and error behavior parameters present on Linux (but e.g. absent on OpenBSD)
are not implemented

# Known issues

If an application output of which is being split managed to opened a handle to a file
which is supposed to be split's output, append option may not work as expected.

A canonical example of this is splitting `dir` output to a directory that's being listed:

```
dir | split.exe -a dir.txt
```

Perhaps there's a workaround for this issue but I'm not aware of one as it seems that
split is completely on a mercy of whoever grabs the handle first.

No testing on non-ASCII streams have been performed so binary and Unicode/MBCS inputs
may not work properly.

# Why not call it tee?

I don't like the name. It comes from the T-splits used by plumbers and tee sounds like
letter *t* soo... It's convoluted and tries to be clever. We're splitting pipelined output
here, that's all. Besides it doesn't replicate all of the tee functionality so it would
be false advertising to call it `tee`.
