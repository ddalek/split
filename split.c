#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <io.h>
#include <sys/stat.h>
#include <fcntl.h>

void help(char const* name)
{
	printf("split - a simplistic tee replacement for Windows\n");
	printf("Syntax:\n\t%s [options] [output_file]\n", name);
	printf("Options:\n");
	printf("\t--help       - this help message\n\t-a, --append - append to file instead of overwriting it\n");
	printf("If output_file is absent, %s will simply act as a pipe and won't do the splitting.\n", name);
	printf("Only one output_file can be supplied which is a simplification compared to tee.\n");
	exit(1);
}

int main(int argc, char** argv)
{
	if (argc > 1 && !strcmp(argv[1], "--help"))
	{
		help(argv[0]);
	}

	bool append = argc > 1 && (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--append") == 0);
	char const* output_file = append
		? (argc < 3 ? NULL : argv[2])
		: (argc < 2 ? NULL : argv[1]);

	bool errors = false;
	int handles[] = { _fileno(stdout), -1, -1 };
	if (output_file)
	{
		int oflag = (append ? _O_APPEND : _O_TRUNC );
		errno_t result = _sopen_s(&handles[1], output_file, _O_CREAT | _O_WRONLY | oflag, _SH_DENYNO, _S_IREAD);
		if (result)
		{
			printf("Warning: Failed to open '%s' for writing\n", output_file);
			errors = true;
		}
	}

	int bytes = 0;
	char buffer[4096];
	while ((bytes = _read(_fileno(stdin), buffer, sizeof(buffer))) > 0)
	{
		for (int i = 0; handles[i] != -1; ++i)
		{
			int written = 0;
			while (written < bytes)
			{
				int result = _write(handles[i], buffer + written, bytes - written);
				if (result == -1)
				{
					printf("Warning: Failed to write to '%s'\n", (i ? output_file : "stdin"));
					errors = true;
					break;
				}
				written += result;
			}
		}
	}

	if (bytes == -1)
	{
		printf("Warning: Failed to read from stdin\n");
		errors = true;
	}

	if (handles[1] != -1)
	{
		_close(handles[1]);
	}

	return (errors ? 1 : 0);
}

